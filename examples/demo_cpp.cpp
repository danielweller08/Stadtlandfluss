#include <stadt_land_fluss/controller.hpp>
#include <stadt_land_fluss/board.hpp>
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
    
using namespace StadtLandFluss;

    int main () {

        Controller controller;

        // Create players
        int userToken_player1 = controller.get_userToken();
        std::string name_player1 = "Simon";

        int userToken_player2 = controller.get_userToken();
        std::string name_player2 = "Damyan";

        int userToken_player3 = controller.get_userToken();
        std::string name_player3 = "Daniel";

        // Create game
        Board board = controller.create_game(name_player1, userToken_player1);

        // Game Id for game one
        int gameid_1 = board.get_id();

         // Users join the game
        board = controller.join_game(gameid_1, name_player2, userToken_player2);
        board = controller.join_game(gameid_1, name_player3, userToken_player3);

        // Adding categories
        // board = controller.create_category(gameid_1, userToken_player1, "Stadt");
        // board = controller.create_category(gameid_1, userToken_player1, "Land");
        // board = controller.create_category(gameid_1, userToken_player1, "Fluss");

        // Runde 1 startet
        board = controller.start_game(gameid_1, userToken_player1);

        std::cout << "Die Kategorien sind:\n";

        for(const auto& categories : board.get_categories()) {
            std::cout << categories << ", ";
        }

        std::cout << "\n";

        

        char curr_letter = board.get_currentLetter();       // Buchstabe
        printf("Buchstabe: %c \n", curr_letter);

        // Tippen beginnt

        board = controller.submit_category(gameid_1, userToken_player1, "Stadt", std::string(1, curr_letter));
        board = controller.submit_category(gameid_1, userToken_player1, "Land", std::string(1, curr_letter));
        board = controller.submit_category(gameid_1, userToken_player1, "Fluss", std::string(1, curr_letter));

        // board = controller.submit_category(gameid_1, userToken_player2, "Stadt", std::string(1, curr_letter));
        // board = controller.submit_category(gameid_1, userToken_player2, "Land", std::string(1, curr_letter));
        // board = controller.submit_category(gameid_1, userToken_player2, "Fluss", std::string(1, curr_letter));

        // board = controller.submit_category(gameid_1, userToken_player3, "Stadt", std::string(1, curr_letter));
        // board = controller.submit_category(gameid_1, userToken_player3, "Land", std::string(1, curr_letter));
        // board = controller.submit_category(gameid_1, userToken_player3, "Fluss", "");
 
        board = controller.stop_game(gameid_1, userToken_player1);     // Kein Spieler kann mehr Kategorien schreiben

        // Voting beginnt

        // Rate player_1

        board = controller.vote(gameid_1, userToken_player2, name_player1, "Stadt", true);

        // board = controller.vote(gameid_1, userToken_player1, name_player1, "Land", true);        // Upvote
        // board = controller.vote(gameid_1, userToken_player1, name_player1, "Fluss", true);      // Upvote
        // board = controller.vote(gameid_1, userToken_player1, name_player1, "Stadt", true);      // Upvote für Kategorie Stadt für player31
        // board = controller.vote(gameid_1, userToken_player2, name_player1, "Stadt", true);      // Upvote
        // board = controller.vote(gameid_1, userToken_player2, name_player1, "Land", true);      // Upvote
        // board = controller.vote(gameid_1, userToken_player2, name_player1, "Fluss", true);      //Upvote1
        // board = controller.vote(gameid_1, userToken_player3, name_player1, "Stadt", true);      // Upvote
        // board = controller.vote(gameid_1, userToken_player3, name_player1, "Land", true);      // Upvote
        // board = controller.vote(gameid_1, userToken_player3, name_player1, "Fluss", true);      // Upvote


        // Rate player_2

        // board = controller.vote(gameid_1, userToken_player1, name_player2, "Stadt", true);      // Upvote für Kategorie Stadt für player2
        // board = controller.vote(gameid_1, userToken_player1, name_player2, "Land", true);        // Upvote
        // board = controller.vote(gameid_1, userToken_player1, name_player2, "Fluss", false);      // Downvote für Kategorie Stadt für player2

        // board = controller.vote(gameid_1, userToken_player2, name_player2, "Stadt", true);      // Upvote
        // board = controller.vote(gameid_1, userToken_player2, name_player2, "Land", true);      // Upvote
        // board = controller.vote(gameid_1, userToken_player2, name_player2, "Fluss", false);      //Downvote

        // board = controller.vote(gameid_1, userToken_player3, name_player2, "Stadt", true);      // Upvote
        // board = controller.vote(gameid_1, userToken_player3, name_player2, "Land", true);      // Upvote
        // board = controller.vote(gameid_1, userToken_player3, name_player2, "Fluss", false);      // Downvote

        
        // Rating
        board = controller.rate(gameid_1, userToken_player1);

        // Total score
        for(auto player : board.get_players()) {
            std::cout << "Punktzahl von " << player.first << " ist: " << player.second << "\n";
        }      


        // Runde 2 startet
        board = controller.start_game(gameid_1, userToken_player1);

        // Tippen beginnt

        curr_letter = board.get_currentLetter();       // Buchstabe
        printf("Buchstabe: %c \n", curr_letter);

        board = controller.submit_category(gameid_1, userToken_player1, "Stadt", std::string(1, curr_letter));
        board = controller.submit_category(gameid_1, userToken_player1, "Land", std::string(1, curr_letter));
        board = controller.submit_category(gameid_1, userToken_player1, "Fluss", std::string(1, curr_letter));

        board = controller.submit_category(gameid_1, userToken_player2, "Stadt", std::string(1, curr_letter));
        board = controller.submit_category(gameid_1, userToken_player2, "Land", std::string(1, curr_letter));
        board = controller.submit_category(gameid_1, userToken_player2, "Fluss", std::string(1, curr_letter));

        board = controller.submit_category(gameid_1, userToken_player3, "Stadt", std::string(1, curr_letter));
        board = controller.submit_category(gameid_1, userToken_player3, "Land", std::string(1, curr_letter));
        board = controller.submit_category(gameid_1, userToken_player3, "Fluss", std::string(1, curr_letter));

        board = controller.stop_game(gameid_1, userToken_player1);

        // Voting starts

        



        
        controller.quit_game(gameid_1, userToken_player1);       // Ende des Spiels
        

        return 0;
    }