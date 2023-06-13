#pragma once

#include <string>
#include <map>
#include <vector>
#include <random>

using namespace std;

namespace StadtLandFluss {
    /// @brief Status of the board.
    enum BoardStatus {
        Vorbereiten,
        Spielen,
        Bewerten,
        Ergebnisse
    };

    struct BoardSettings {
        int roundsAmount;
        vector<char> allowedLetters;
        bool endRoundManually;
        int timeLimit; // seconds.
    };

    class Board {
        public:
            /// @brief Default constructor. 
            /// @remark Should not be used because it creates a board with a id of 0!
            Board();

            /// @brief Constructor.
            /// @param id The id of the game.
            Board(int id);

            /// @brief Gets the gameId of the board.
            /// @return The boards gameId.
            int get_id();

            /// @brief Adds a new player to the game.
            /// @param userName The name of the user to be added.
            void add_player(string userName);

            /// @brief Removes an existing player from the game.
            /// @param userName The name of the user to be removed.
            void remove_player(string userName);

            /// @brief Gets the games players.
            /// @return The players including their scores.
            map<string, int> get_players();

            /// @brief Gets the boards status.
            /// @return The current status of the board.
            BoardStatus get_status();

            /// @brief Gets the boards settings.
            /// @return The boards settings.
            BoardSettings get_settings();

            /// @brief Sets the settings.
            /// @param settings The new settings.
            void set_settings(BoardSettings settings);

            /// @brief Gets the boards data.
            /// @return The boards data.
            map<char, map<string, map<string, pair<string, std::vector<bool>>>>> get_data();

            /// @brief Gets the boards categories.
            /// @return The boards categories.
            vector<string> get_categories();

            /// @brief Gets the boards current letter.
            /// @return The current letter.
            char get_currentLetter();

            /// @brief Gets the boards current round.
            /// @return The current round.
            int get_currentRound();

            /// @brief Starts or continues the game.
            /// @param rndGen The random number generator to get a new letter from.
            void start(mt19937 rndGen);

            /// @brief Stops the game.
            /// @param userName Name of the stopping user.
            void stop(string userName);

            /// @brief Adds a new entry to the chosen category of the user.
            /// @param userName The name of the user.
            /// @param category The category to be updated.
            /// @param value The value of the entry.
            void submit(string userName, string category, string value);

            /// @brief Adds a category.
            /// @param category The category to be added.
            void add_category(string category);

            /// @brief Removes a category.
            /// @param category The category to be removed.
            void remove_category(string category);
            
            /// @brief Player has one vote for every category of each player except itself
            /// @param userName The name of the user.
            /// @param category The category to be voted on.
            /// @param value boolean value of the vote, yes for an upvote and no for a downvote.
            void vote(string userName, string category, bool value);
        

            /// @brief After voting points get added up and assigned to the players' score
            void rate_players();

            // Hilfsmethode
            void voting();

        private:
            /// @brief Id of the board. Equivalent to gameId.
            int _id;
            /// @brief State of the board.
            BoardStatus _status;
            /// @brief Board settings.
            BoardSettings _settings;
            /// @brief Board data.
            map<char, map<string, map<string, pair<string, std::vector<bool>>>>> _data;
            /// @brief Categories.
            vector<string> _categories;
            /// @brief Player names and their scores.
            map<string, int> _players;
            /// @brief Remaining letters.
            vector<char> _letters;
            /// @brief Letter of the current round.
            char _currentLetter;
            /// @brief Number of the current round.
            int _currentRound;
    };
}