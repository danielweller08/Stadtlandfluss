# Start demo_fastapi_stadt_land_fluss in background first

import threading
import time
import requests
from requests import Response
from pprint import pprint # Bei größeren Dicts sollte man pprint(...) statt print(...) verwenden

board = None
user_isInInput = False
statusChangedDuringInput = False

def option(options: list):
    global user_isInInput
    global statusChangedDuringInput
    while True:
        for i, o in enumerate(options):
            print(f"[{i}] {o}")
        
        user_isInInput = True
        auswahl = input("Wähle eine Option: ")
        user_isInInput = False

        if statusChangedDuringInput == True:
            return None

        for i, o in enumerate(options):
            if auswahl == str(i):
                return o
            
        print("Deine Eingabe war ungültig, versuche es erneut")
        print()

def checkSuccess(response : Response):
    if int(response.status_code / 100) != 2:
        print(f"Fehler aufgetreten: {response.json()['detail']}")
        return False
    else:
        return True
    
def poll(args):
    global user_isInInput
    global board
    global statusChangedDuringInput
    base_api_url = args[0]
    gameId = args[1]
    userToken = args[2]
    while True:
        response = requests.get(f"{base_api_url}/games/{gameId}?userToken={userToken}")
        if checkSuccess(response):
            board_new = response.json()
            # Check if status changed to notify user
            if board_new["status"] != board["status"] and user_isInInput == True:
                if board_new["status"] == "Spielen":
                    statusChangedDuringInput = True
                    print("Das Spiel geht los! Drücke schnell Enter um teilzunehmen")
                elif board_new["status"] == "Bewerten":
                    statusChangedDuringInput = True
                    print("Zeit ist um! Drücke Enter um zur Bewertung zu kommen")
                elif board_new["status"] == "Ergebnisse":
                    statusChangedDuringInput = True
                    print("Das Spiel ist vorbei. Drücke Enter um zu den Ergebnissen zu kommen")
            
            board = board_new
        time.sleep(1)
    
def start_thread(function_name, *args, **kwargs):
    t = threading.Thread(target=function_name, args=args, kwargs=kwargs)
    t.daemon = True
    t.start()
    return t

def main():
    global board
    global statusChangedDuringInput
    global user_isInInput

    base_api_url = "http://127.0.0.1:8000"

    print("-------------------------------------------------------------")
    print("                      Stadt Land Fluss                       ")
    print("-------------------------------------------------------------")
    print()

    # Get userToken
    try:
        response = requests.get(base_api_url + "/userToken/new")
        response_json = response.json()
        userToken = response_json["userToken"]
        print(f"UserToken: {userToken}")
    except Exception:
        print("Fehler beim Verbinden mit der API. Drücke Enter um das Programm zu verlassen")
        input()
        return

    userName = input("Bitte gebe deinen Username ein: ")

    while True:

        # Spiel beitreten oder erstellen
        while True:
            print()
            option1 = option(["Spiel erstellen", "Spiel beitreten", "Spiel verlassen"])

            if option1 == "Spiel erstellen":
                response = requests.post(f"{base_api_url}/games?userToken={userToken}", json = { "userName" : userName })
                if checkSuccess(response):    
                    board = response.json()
                    gameId = board["id"]
                    print("Spiel erfolgreich erstellt")
                    break
            elif option1 == "Spiel beitreten":
                gameId = input("Bitte gebe die game id ein: ")

                response = requests.post(f"{base_api_url}/games/{gameId}/join?userToken={userToken}", json = { "userName" : userName })
                if checkSuccess(response):
                    board = response.json()
                    print("Spiel erfolgreich beigetreten")
                    break
            elif option1 == "Spiel verlassen":
                return

        # Start polling
        pollingThread = start_thread(poll, [base_api_url, gameId, userToken])

        # Vorbereitung
        while True:
            print()
            print(f"GameId: {board['id']}")
            print(f"Status: {board['status']}")
            print(f"Einstellungen")
            print(f"    Anzahl Runden: {board['settings']['roundsAmount']}")
            print(f"    Buchstaben: {' '.join(board['settings']['allowedLetters'])}")
            print(f"    Runde manuell beenden: {'Erlaubt' if board['settings']['endRoundManually'] == 'true' else 'Nicht erlaubt'}")
            print(f"    Zeit pro Runde [Sekunden]: {board['settings']['timeLimit']}")
            print(f"Kategorien: {board['categories']}")
            players = []
            for key, val in board['players'].items():
                players.append(key)
            print(f"Spieler: {', '.join(players)}")
            print()

            option2 = option(["Spiel starten", "Einstellungen ändern", "Kategorien hinzufügen", "Kategorien entfernen"])
            if statusChangedDuringInput == True:
                statusChangedDuringInput = False
                break

            if option2 == "Spiel starten":
                response = requests.post(f"{base_api_url}/games/{gameId}/start?userToken={userToken}")
                if checkSuccess(response):
                    board = response.json()
                    break
            elif option2 == "Einstellungen ändern":
                option3 = option(["Anzahl Runden", "Buchstaben hinzufügen", "Buchstaben entfernen", "Runde manuell beenden", "Zeit pro Runde [Sekunden]"])
                if statusChangedDuringInput == True:
                    statusChangedDuringInput = False
                    break

                if option3 == "Anzahl Runden":
                    user_isInInput = True
                    board['settings']['roundsAmount'] = input("Gib eine neue Anzahl Runden ein: ")
                    user_isInInput = False
                    if statusChangedDuringInput == True:
                        statusChangedDuringInput = False
                        break

                    response = requests.put(f"{base_api_url}/games/{gameId}/settings?userToken={userToken}", json = board['settings'])
                    if checkSuccess(response):    
                        board = response.json()
                if option3 == "Runde manuell beenden":
                    user_isInInput = True
                    endRoundManually = input("Runde manuell beenden [Ja/Nein]: ")
                    user_isInInput = False
                    if statusChangedDuringInput == True:
                        statusChangedDuringInput = False
                        break

                    if endRoundManually.lower() == "ja":
                        response["settings"]["endRoundManually"] = True
                    elif endRoundManually.lower() == "nein":
                        response["settings"]["endRoundManually"] = False
                    else:
                        print("Kein passender Wert eingegeben!")    
                        continue

                    response = requests.put(f"{base_api_url}/games/{gameId}/settings?userToken={userToken}", json = board['settings'])
                    if checkSuccess(response):    
                        board = response.json()
                if option3 == "Zeit pro Runde [Sekunden]":
                    user_isInInput = True
                    board['settings']['timeLimit'] = input("Gib ein Zeitlimit in Sekunden ein: ")
                    user_isInInput = False
                    if statusChangedDuringInput == True:
                        statusChangedDuringInput = False
                        break

                    response = requests.put(f"{base_api_url}/games/{gameId}/settings?userToken={userToken}", json = board['settings'])
                    if checkSuccess(response):    
                        board = response.json()
                if option3 == "Buchstaben hinzufügen":
                    while True:
                        user_isInInput = True
                        letter = input("Gebe einen Buchstaben ein: ")
                        user_isInInput = False
                        if statusChangedDuringInput == True:
                            statusChangedDuringInput = False
                            break

                        if len(letter) > 1:
                            print("Du hast mehr als nur einen Buchstaben eingegeben, versuche es erneut.")
                        else:
                            allowedLetters = list(board["settings"]["allowedLetters"])
                            allowedLetters.append(letter.upper())
                            board["settings"]["allowedLetters"] = allowedLetters
                            response = requests.put(f"{base_api_url}/games/{gameId}/settings?userToken={userToken}", json = board['settings'])
                            if checkSuccess(response):    
                                board = response.json()

                        endRoundManually = input("Weiteren Buchstaben hinzufügen [Ja/Nein]: ")
                        if endRoundManually.lower() != "ja":
                            break
                if option3 == "Buchstaben entfernen":
                    while True:
                        user_isInInput = True
                        letter = input("Gebe einen Buchstaben ein: ")
                        user_isInInput = False
                        if statusChangedDuringInput == True:
                            statusChangedDuringInput = False
                            break

                        allowedLetters = list(board["settings"]["allowedLetters"])
                        allowedLetters.remove(letter.upper())
                        board["settings"]["allowedLetters"] = allowedLetters
                        response = requests.put(f"{base_api_url}/games/{gameId}/settings?userToken={userToken}", json = board['settings'])
                        if checkSuccess(response):    
                            board = response.json()

                        user_isInInput = True
                        endRoundManually = input("Weiteren Buchstaben entfernen [Ja/Nein]: ")
                        user_isInInput = False
                        if statusChangedDuringInput == True:
                            statusChangedDuringInput = False
                            break

                        if endRoundManually.lower() != "ja":
                            break
            if option2 == "Kategorien hinzufügen":
                while True:
                    category = input("Gebe eine Kategorie ein: ")
                    response = requests.post(f"{base_api_url}/games/{gameId}/categories?userToken={userToken}", json = {"category":category})
                    if checkSuccess(response):    
                        board = response.json()

                    user_isInInput = True
                    endRoundManually = input("Weitere Kategorie hinzufügen [Ja/Nein]: ")
                    user_isInInput = False
                    if statusChangedDuringInput == True:
                        statusChangedDuringInput = False
                        break

                    if endRoundManually.lower() != "ja":
                        break
            if option2 == "Kategorien entfernen":
                while True:
                    user_isInInput = True
                    category = input("Gebe eine Kategorie ein: ")
                    user_isInInput = False
                    if statusChangedDuringInput == True:
                        statusChangedDuringInput = False
                        break

                    response = requests.delete(f"{base_api_url}/games/{gameId}/categories/{category}?userToken={userToken}")
                    if checkSuccess(response):    
                        board = response.json()

                    user_isInInput = True
                    endRoundManually = input("Weitere Kategorie entfernen [Ja/Nein]: ")
                    user_isInInput = False
                    if statusChangedDuringInput == True:
                        statusChangedDuringInput = False
                        break

                    if endRoundManually.lower() != "ja":
                        break

        # Spielen + Bewerten
        print()
        i = 0
        # Runde i startet
        while True:
            i += 1
            print("-------------------------------------------------------------")
            print(f"                    Runde {i}, Buchstabe {board['currentLetter']}")
            print("-------------------------------------------------------------")
            print()

            # Eingabe Kategorie
            while True:
                user_isInInput = True
                options = list(board["categories"])
                options.append("Runde beenden")
                option4 = option(options)
                user_isInInput = False
                if statusChangedDuringInput == True:
                    statusChangedDuringInput = False
                    break

                if option4 == "Runde beenden":
                    response = requests.post(f"{base_api_url}/games/{gameId}/stop?userToken={userToken}")
                    if checkSuccess(response):    
                        board = response.json()
                        break
                    else:
                        print()
                        continue

                user_isInInput = True
                value = input(f"{option4}: ")
                user_isInInput = False
                if statusChangedDuringInput == True:
                    statusChangedDuringInput = False
                    break

                try:
                    response = requests.post(f"{base_api_url}/games/{gameId}/categories/{option4}/submit?userToken={userToken}", json = { "value": value })
                    if checkSuccess(response):    
                        board = response.json()
                except:
                    continue

                print()
                print("Bereits eingegebene Kategorien:")
                for element in board["data"][board['currentLetter']][userName]:
                    try:
                        print(f"{element}: {board['data'][board['currentLetter']][userName][element][0]}")
                    except:
                        None
                print()

            # Bewertung
            print()
            print("Bewertung mit [J/N]:")
            for category in board["categories"]:
                print(f"Kategorie: {category}")
                for user in board["players"]:
                    if user != userName:
                        while True:
                            try: 
                                retry = False
                                user_isInInput = True
                                vote = input(f"  {user}: {board['data'][board['currentLetter']][user][category][0]}: ")
                                user_isInInput = False
                                if statusChangedDuringInput == True:
                                    break

                                if vote.upper() == "J":
                                    vote_bool = True
                                elif vote.upper() == "N":
                                    vote_bool = False
                                else:
                                    print("Falsche Bewertung, bitte versuche es erneut")
                                    retry = True

                                response = requests.post(f"{base_api_url}/games/{gameId}/categories/{category}/vote?userToken={userToken}", json = { "userName": user, "value": vote_bool})
                                if checkSuccess(response):    
                                    board = response.json()
                            except:
                                if retry == False:
                                    break
                if statusChangedDuringInput == True:
                    break


            if statusChangedDuringInput == True:
                statusChangedDuringInput = False
                # Only break for Ergebnisse
                # Spielen results in another iteration
                if board["status"] == "Spielen":
                    continue
                if board["status"] == "Ergebnisse":
                    break

            # Rate the game
            response = requests.post(f"{base_api_url}/games/{gameId}/rate?userToken={userToken}")
            checkSuccess(response)

            # Continue or quit
            option5 = option(["Nächste Runde", "Spiel Verlassen"])
            if statusChangedDuringInput == True:
                statusChangedDuringInput = False
                if board["status"] == "Ergebnisse":
                    break

            if option5 == "Nächste Runde":
                response = requests.post(f"{base_api_url}/games/{gameId}/start?userToken={userToken}")
                if checkSuccess(response) == False:    
                    try:
                        requests.post(f"{base_api_url}/games/{gameId}/quit?userToken={userToken}")
                        break
                    except:
                        break
                else:
                    board = response.json()
            elif option5 == "Spiel Verlassen":
                try:
                    requests.post(f"{base_api_url}/games/{gameId}/quit?userToken={userToken}")
                    break
                except:
                    break

        # Punkteausgabe
        print()
        print("-------------------------------------------------------------")
        print(f"                      Punktestand")

        for user in board["players"]:
            try:
                print(f"{user}: {board['players'][user]}")
            except:
                None

        print("-------------------------------------------------------------")

        pollingThread.stop()


  

if __name__ == '__main__':
    main()
