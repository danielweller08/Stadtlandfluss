#pragma once

#include <string>
#include <map>
#include <vector>
#include <random>

#include <stadt_land_fluss/board.hpp>

using namespace std;

namespace StadtLandFluss {
    /// @brief API controller. Manages lobbies and offers methods for each API endpoint.
    /// @remark Works as a database and endpoint connector. Actually this should be split into a database and a stateless API controller.
    class Controller {
        public: 
            /// @brief Default constructor.
            Controller();

            /// @brief Get the list of games.
            /// @return The list of games.
            vector<Board> get_games();

            /// @brief Creates a new user token.
            /// @return The generated user token.
            int get_userToken();

            /// @brief Creates the game and assigns the calling user.
            /// @param userName The user name to create the game with.
            /// @param userToken The token of the calling user.
            /// @return The created game.
            Board create_game(string userName, int userToken);

            /// @brief Assigns the calling user to the game.
            /// @param gameId The id of the game to join.
            /// @param userName The user name to join the game with.
            /// @param userToken The token of the calling user.
            /// @return The joined game.
            Board join_game(int gameId, string userName, int userToken);

            /// @brief Unassigns the calling user from the game.
            /// @param gameId The id of the game to quit.
            /// @param userToken The token of the calling user.
            void quit_game(int gameId, int userToken);

            /// @brief Starts or continues a game.
            /// @param gameId The id of the game to start.
            /// @param userToken The token of the calling user.
            /// @return The started game.
            Board start_game(int gameId, int userToken);

            /// @brief Stops a game.
            /// @param gameId The id of the game to stop.
            /// @param userToken The token of the calling user.
            /// @return The stopped game.
            Board stop_game(int gameId, int userToken);

            /// @brief Gets the state of a game.
            /// @param gameId The id of the game to inspect.
            /// @param userToken The token of the calling user.
            /// @return The state of the requested game.
            Board get_game(int gameId, int userToken);

            /// @brief Updates the settings of a game.
            /// @param gameId The if of the game to update.
            /// @param userToken The token of the calling user.
            /// @param settings The updated settings value.
            /// @return The updated game.
            Board update_settings(int gameId, int userToken, BoardSettings settings);

            /// @brief Creates a new category in a game.
            /// @param gameId The id of the game to update.
            /// @param userToken The token of the calling user.
            /// @param category The category to be added.
            /// @return The updated game.
            Board create_category(int gameId, int userToken, string category);

            /// @brief Deletes a category from a game.
            /// @param gameId The id of the game to update.
            /// @param userToken The token of the calling user.
            /// @param category The category to be removed.
            /// @return The updated game.
            Board delete_category(int gameId, int userToken, string category);

            /// @brief Submits a new category entry to a game.
            /// @param gameId The if of the game to update.
            /// @param userToken The token of the calling user.
            /// @param category The category of the entry to be added.
            /// @param value The value of the entry.
            void submit_category(int gameId, int userToken, string category, string value);


            /// @brief Each player gets to vote on the guesses of the others players' answers with a yes or no
            /// @param gameId The id of the game to vote on
            /// @param userToken The token of the calling user
            /// @param username_vote The name of the user to vote on
            /// @param category The category of the entry to be voted on
            /// @param value A true value represents that player agrees on the player's answer, false disagrees
            Board vote(int gameId, int userToken, string username_vote, string category, bool value);


            // Wird aufgerufen nach all den votings
            Board rate(int gameId, int userToken);

        private:
            /// @brief Collection of lobbies and there gameIds.
            map<int, Board> _games;
            /// @brief Relations between players, including their tokens, names and lobbies.
            map<int, pair<int, string>> _players_games;
            /// @brief Random number generator.
            mt19937 _rndGen;
            /// @brief Random user token distributor.
            uniform_int_distribution<unsigned> _rndDistribUserToken;

            /// @brief Checks if the given user is assigned to the given game.
            /// @param gameId The id of the game to be checked.
            /// @param userToken The token of the user to be checked.
            /// @return A value indicating if the user game assignment was found.
            bool userIsAssigned(int gameId, int userToken);
    };
}