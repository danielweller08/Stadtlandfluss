# Import magic
try:
  import stadt_land_fluss
except ImportError as e:
  print(f"Importing the shared library 'stadt_land_fluss' did not work.")
  print(f"Is (a link to) the shared library 'stadt_land_fluss.____.so' in the same directory as this python script?")
  print(f"The import caused the following exception '{e}'")
  print(f"Exiting")
  exit(1)
else:
  print(f"Importing the shared library 'stadt_land_fluss' did work.")


from stadt_land_fluss import Controller
import pydoc


def main():
    print(f"Module name: '{stadt_land_fluss.__doc__}'")
    print(f"Controller doc: {pydoc.render_doc(Controller)}")

    # Erstelle Controller
    controller = Controller();

    # Erstelle user token für zwei Spieler
    userToken_player1 = controller.get_userToken()
    userToken_player2 = controller.get_userToken()

    # Erstelle Spiel
    board = controller.create_game("Damyan", userToken_player1)

    # Hole GameId
    gameid_1 = board.get_id()

    # Zweiter Spieler tritt bei
    board = controller.join_game(gameid_1, "Felix", userToken_player2)

    # Starte das Spiel
    board = controller.start_game(gameid_1, userToken_player1)

    # Hole ersten Buchstaben
    curr_letter = board.get_currentLetter()

    # Spieler 1 submitted seine Einträge
    board = controller.submit_category(gameid_1, userToken_player1, "Stadt", curr_letter)
    board = controller.submit_category(gameid_1, userToken_player1, "Land", curr_letter)
    board = controller.submit_category(gameid_1, userToken_player1, "Fluss", curr_letter)

    # Spieler 1 submitted seine Einträge
    board = controller.submit_category(gameid_1, userToken_player2, "Stadt", curr_letter)
    board = controller.submit_category(gameid_1, userToken_player2, "Land", curr_letter)
    board = controller.submit_category(gameid_1, userToken_player2, "Fluss", curr_letter)

    # Spieler 1 stoppt
    board = controller.stop_game(gameid_1, userToken_player1)

    # Spieler 1 rated
    board = controller.vote(gameid_1, userToken_player1, "Felix", "Stadt", True)

    # Spieler 1 beendet
    controller.quit_game(gameid_1, userToken_player1)

    print(board.get_id());
    print(board.get_status());
    print(board.get_settings().get_roundsAmount());
    print(board.get_data());
    print(board.get_categories());
    print(board.get_players());
    print(board.get_letters());
    print(board.get_currentLetter());
    print(board.get_currentRound());
    print(board.get_startTime());


if __name__ == '__main__':
  main()
