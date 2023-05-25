#include <stadt_land_fluss/controller.hpp>
#include <stadt_land_fluss/board.hpp>
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
    
using namespace StadtLandFluss;

    int main () {

        Controller controller;
        Board board;

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
        board = controller.create_game(name_player1, userToken_player1);
        
        for(const auto& categories : board.get_categories()) {
            std::cout << categories << "Hello\n";
        }
    
        char curr_letter = board.get_currentLetter();

         // Users join the game
        controller.join_game(gameid_1, name_player2, userToken_player2);
        

        // Runde startet
        printf("Runde 1: Buchstabe: %c \n", curr_letter);
        
        controller.start_game(gameid_1, userToken_player1);
        controller.create_category(gameid_1, userToken_player1, "Stadt");
        controller.create_category(gameid_1, userToken_player1, "Land");
        controller.create_category(gameid_1, userToken_player1, "Fluss");


        controller.submit_category(gameid_1, userToken_player1, "Stadt", "");
        // controller.stop_game();     // Kein Spieler kann mehr Kategorien schreiben




        // controller.start_game(gameid_1, controller.get_userToken());
        // controller.stop_game();


        // // Methoden
        
        // controller.delete_category();
        // controller.get_game();
        // controller.get_games();
        
        // controller.quit_game();
        

        return 0;
    }