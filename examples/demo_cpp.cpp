#include <stadt_land_fluss/controller.hpp>
#include <stadt_land_fluss/board.hpp>
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
    
using namespace StadtLandFluss;

    int main () {

        Controller controller;

        //Game Id for game one
        int gameid_1;
        

        //Create players
        int userToken_player1 = controller.get_userToken();
        std::string name_player1 = "Simon";

        int userToken_player2 = controller.get_userToken();
        std::string name_player2 = "Damyan";

        int userToken_player3 = controller.get_userToken();
        std::string name_player3 = "Daniel";

        // Create game
        Board board = controller.create_game(name_player1, userToken_player1);

         // Users join the game
        controller.join_game(gameid_1, name_player2, userToken_player2);
        controller.join_game(gameid_1, name_player3, userToken_player3);

        // Runde 1 startet
        controller.start_game(gameid_1, userToken_player1);

        
        // Adding categories
        controller.create_category(gameid_1, userToken_player1, "Stadt");
        controller.create_category(gameid_1, userToken_player1, "Land");
        board = controller.create_category(gameid_1, userToken_player1, "Fluss");

        std::cout << "Die Kategorien sind:\n";

        for(const auto& categories : board.get_categories()) {
            std::cout << categories << ", ";
        }

        std::cout << "\n";

        char curr_letter = board.get_currentLetter();       // Buchstabe
        printf("Buchstabe: %c \n", curr_letter);

        // Tippen beginnt

        controller.submit_category(gameid_1, userToken_player1, "Stadt", "Nottingham");
        controller.submit_category(gameid_1, userToken_player1, "Land", "Niederlande");
        controller.submit_category(gameid_1, userToken_player1, "Fluss", "Neckar");

        controller.submit_category(gameid_1, userToken_player2, "Stadt", "Nürnberg");
        controller.submit_category(gameid_1, userToken_player2, "Land", "Niederlande");
        controller.submit_category(gameid_1, userToken_player2, "Fluss", "");

        controller.submit_category(gameid_1, userToken_player3, "Stadt", "");
        controller.submit_category(gameid_1, userToken_player3, "Land", "Nigeria");
        controller.submit_category(gameid_1, userToken_player3, "Fluss", "Nil");
 
        controller.stop_game(gameid_1, userToken_player1);     // Kein Spieler kann mehr Kategorien schreiben


        // Runde 2 startet
        board = controller.start_game(gameid_1, userToken_player1);

        // Tippen beginnt

        curr_letter = board.get_currentLetter();       // Buchstabe
        printf("Buchstabe: %c \n", curr_letter);

        controller.submit_category(gameid_1, userToken_player1, "Stadt", "Berlin");
        controller.submit_category(gameid_1, userToken_player1, "Land", "Brasilien");
        controller.submit_category(gameid_1, userToken_player1, "Fluss", "");

        controller.submit_category(gameid_1, userToken_player2, "Stadt", "");
        controller.submit_category(gameid_1, userToken_player2, "Land", "Bulgarien");
        controller.submit_category(gameid_1, userToken_player2, "Fluss", "");

        controller.submit_category(gameid_1, userToken_player3, "Stadt", "");
        controller.submit_category(gameid_1, userToken_player3, "Land", "Bangladesh");
        controller.submit_category(gameid_1, userToken_player3, "Fluss", "Blauer Nil");

        controller.stop_game(gameid_1, userToken_player1);
        
        controller.quit_game(gameid_1, userToken_player1);       // Ende des Spiels
        

        return 0;
    }