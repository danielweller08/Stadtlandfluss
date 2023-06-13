#include <stadt_land_fluss/controller.hpp>

#include <chrono>
#include <stdexcept>

using namespace std;

namespace StadtLandFluss {
    Controller::Controller() {
        // Initialize number generator.
        random_device rd;
        mt19937::result_type seed = rd() ^ (
            (mt19937::result_type)
            chrono::duration_cast<chrono::seconds>(
                chrono::system_clock::now().time_since_epoch()
                ).count() +
            (mt19937::result_type)
            chrono::duration_cast<chrono::microseconds>(
                chrono::high_resolution_clock::now().time_since_epoch()
                ).count() );

        _rndGen = mt19937(seed);
        _rndDistribUserToken = uniform_int_distribution<unsigned>(1000000, 9999999);
    }

    vector<Board> Controller::get_games()
    {
        // Push the first elements of the games pairs into a vector and return it.
        vector<Board> returnValue;
        for (auto game: _games) {
            returnValue.push_back(game.second);
        }
        return returnValue;
    }

    int Controller::get_userToken() {
        // Return a new random number as token.
        return _rndDistribUserToken(_rndGen);
    }

    Board Controller::create_game(string userName, int userToken)
    {
        // Check if user is already in a game.
        for (auto player_game: _players_games) {
            if (player_game.first == userToken) {
                throw invalid_argument("Du bist aktuell bereits in einem Spiel.");
            }
        }
        
        // Determine max game id.
        int maxGameId = -1;
        for (auto game: _games) {
            if (maxGameId < game.first) {
                maxGameId = game.first;
            }
        }

        // Increment max game id.
        maxGameId++;

        // Create the game.
        _games[maxGameId] = Board();

        // Assign the calling user to it.
        _players_games[userToken] = pair(maxGameId, userName);
        _games[maxGameId].add_player(userName);
        
        return _games[maxGameId];
    }

    Board Controller::join_game(int gameId, string userName, int userToken)
    {
        // Check if user is assigned to any game.
        for (auto player_game: _players_games) {
            if (player_game.first == userToken) {
                if (player_game.second.first == gameId) {
                    throw invalid_argument("Du bist diesem Spiel bereits beigetreten.");
                } else {
                    throw invalid_argument("Du bist aktuell bereits in einem Spiel.");
                }
            }
        }

        // Assign the player to the game.
        // Add the name first to catch duplicate user names.
        _games[gameId].add_player(userName);
        _players_games[userToken] = pair(gameId, userName);

        return _games[gameId];
    }

    void Controller::quit_game(int gameId, int userToken)
    {
        // Check if user is assigned to the game.
        if (!userIsAssigned(gameId, userToken)) {
            // Nope, early return.
            return;
        }

        // Remove the player from the board.
        _games[gameId].remove_player(_players_games[userToken].second);
        // Remove the player game assignment.
        _players_games.erase(userToken);
        // Check if no players were left in the game.
        if (_games[gameId].get_players().size() == 0) {
            // Yes, remove the game completely.
            _games.erase(gameId);
        }
    }

    Board Controller::start_game(int gameId, int userToken)
    {
        // Check if user is assigned to the game.
        if (!userIsAssigned(gameId, userToken)) {
            throw invalid_argument("Du bist diesem Spiel noch nicht beigetreten.");
        }

        // Start the game.
        _games[gameId].start(_rndGen);

        return _games[gameId];
    }

    Board Controller::stop_game(int gameId, int userToken)
    {
        // Check if user is assigned to the game.
        if (!userIsAssigned(gameId, userToken)) {
            throw invalid_argument("Du bist diesem Spiel noch nicht beigetreten.");
        }

        // Stop the game.
        _games[gameId].stop(_players_games[userToken].second);

        return _games[gameId];
    }

    Board Controller::get_game(int gameId, int userToken)
    {
        // Check if user is assigned to the game.
        if (!userIsAssigned(gameId, userToken)) {
            throw invalid_argument("Du bist diesem Spiel noch nicht beigetreten.");
        }

        return _games[gameId];
    }

    Board Controller::update_settings(int gameId, int userToken, BoardSettings settings)
    {
        // Check if user is assigned to the game.
        if (!userIsAssigned(gameId, userToken)) {
            throw invalid_argument("Du bist diesem Spiel noch nicht beigetreten.");
        }

        _games[gameId].set_settings(settings);

        return _games[gameId];
    }

    Board Controller::create_category(int gameId, int userToken, string category)
    {
        // Check if user is assigned to the game.
        if (!userIsAssigned(gameId, userToken)) {
            throw invalid_argument("Du bist diesem Spiel noch nicht beigetreten.");
        }

        // Create the category.
        _games[gameId].add_category(category);

        return _games[gameId];
    }

    Board Controller::delete_category(int gameId, int userToken, string category)
    {
        // Check if user is assigned to the game.
        if (!userIsAssigned(gameId, userToken)) {
            throw invalid_argument("Du bist diesem Spiel noch nicht beigetreten.");
        }

        // Remove the category.
        _games[gameId].remove_category(category);

        return _games[gameId];
    }

    Board Controller::submit_category(int gameId, int userToken, string category, string value)
    {
        // Check if user is assigned to the game.
        if (!userIsAssigned(gameId, userToken)) {
            throw invalid_argument("Du bist diesem Spiel noch nicht beigetreten.");
        }

        // Submit the entry.
        _games[gameId].submit(_players_games[userToken].second, category, value);

        return _games[gameId];
    }

    bool Controller::userIsAssigned(int gameId, int userToken) {
        // Check if user game relation exists.
        bool userGameRelationFound = false;
        for (auto player_game: _players_games) {
            if (player_game.first == userToken && player_game.second.first == gameId) {
                userGameRelationFound = true;
                break;
            }
        }
        return userGameRelationFound;
    }

    Board Controller::vote(int gameId, int userToken, string username_vote, string category, bool value) {

        // Check if user is assigned to the game.
        if (!userIsAssigned(gameId, userToken)) {
            throw invalid_argument("Du bist diesem Spiel noch nicht beigetreten.");
        }

        // Falsch weil übergebene Spieler nicht derjenige ist, der wählt, sondern der, der bewertet wird 
        //_games[gameId].vote(_players_games[userToken].second, category, value);

        
        _games[gameId].vote(username_vote, category, value);


        return _games[gameId];

    }

    Board Controller::rate(int gameId, int userToken) {
        _games[gameId].rate_players();
        return _games[gameId];
    }
}