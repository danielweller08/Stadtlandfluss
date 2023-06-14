#include <stadt_land_fluss/board.hpp>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <chrono>

using namespace std;

namespace StadtLandFluss {
    Board::Board() : Board(0) {}

    Board::Board(int id) : _id(id), _status(BoardStatus::Vorbereiten) {
        _settings = BoardSettings();
        _settings.allowedLetters = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'R', 'S', 'T', 'U', 'V', 'W', 'Z' };
        _settings.endRoundManually = true;
        _settings.roundsAmount = 10;
        _settings.timeLimit = 60;
        _categories = {"Stadt", "Land", "Fluss"};
    }

    int Board::get_id() { return _id; }

    map<string, int> Board::get_players() { return _players; }

    void Board::add_player(string userName) {
        // Check if user already exists.
        for (auto player: _players) {
            if (player.first == userName) {
                throw invalid_argument("Ein Nutzer mit dem Namen ist bereits im Spiel.");
            }
        }

        // Add the user with a score of 0.
        _players[userName] = 0;
    }


    void Board::remove_player(string userName) {
        _players.erase(userName);
    }

    BoardStatus Board::get_status() { return _status; }

    BoardSettings Board::get_settings() { return _settings; }

    void Board::set_settings(BoardSettings settings) {
        // Check allowed letters.
        if (settings.allowedLetters.size() == 0) {
            throw invalid_argument("Mindestens ein Buchstabe muss erlaubt sein.");
        }

        _settings = settings;
    }

    map<char, map<string, map<string, pair<string, std::vector<bool>>>>>  Board::get_data() { return _data; }

    vector<string> Board::get_categories() { return _categories; }

    char Board::get_currentLetter() { return _currentLetter; }

    int Board::get_currentRound() { return _currentRound; }

    void Board::start(mt19937 rndGen) {   
        // Check if game is allowed to be started.
        if (_status == BoardStatus::Spielen) {
            throw invalid_argument("Das Spiel ist bereits im Gange.");
        } else if (_status == BoardStatus::Ergebnisse) {
            throw invalid_argument("Das Spiel ist beendet und kann nicht erneut gestartet werden.");
        } else if (_status == BoardStatus::Bewerten) {
            // TODO: Check if every player has been rated.
        }

        // Pull letters from settings if prep phase.
        if (_status == BoardStatus::Vorbereiten) {
            _letters = _settings.allowedLetters;
        }

        // Choose a new letter.
        uniform_int_distribution<unsigned> rndDistrib(0, _letters.size() - 1);
        int newLetterIndex = rndDistrib(rndGen);

        // Get the new letter from the list.
        auto itActiveLetter = _letters.begin() + newLetterIndex;
        _currentLetter = *itActiveLetter;

        // Remove the new letter from the list.
        _letters.erase(itActiveLetter);

        // Update the status.
        _status = BoardStatus::Spielen;

        // Start the timer.
        _startTime = std::chrono::system_clock::now();
    }

    void Board::stop(string userName) {
        // Check if game is allowed to be stopped.
        if (_status != BoardStatus::Spielen) {
          throw invalid_argument("Das Spiel kann zu diesem Zeitpunkt nicht gestoppt werden.");
        }

        // Check if user filled every category.
        if (_data.find(_currentLetter) == _data.end()
            || _data[_currentLetter].find(userName) == _data[_currentLetter].end()
            || _data[_currentLetter][userName].size() < _categories.size()) {
            throw invalid_argument("Um eine Runde beenden zu können, müssen alle Kategorien ausgefüllt worden sein.");
        }
        else {
            // Update to the voting status.
            _status = BoardStatus::Bewerten;
        }
    }

    void Board::submit(string userName, string category, string value) {
        // Check that the game is running.
        if (_status != BoardStatus::Spielen) {
            throw invalid_argument("Die Runde ist vorbei, es können keine weiteren Einträge gemacht werden.");
        }

        // Check that the category exists.
        if (std::find(_categories.begin(), _categories.end(), category) == _categories.end()) {
            throw invalid_argument("Die Kategorie '" + category + "' existiert nicht.");
        }
        
        // Check if the user is adding new category within the time limit. If not change _status to "Bewerten".
        std::chrono::system_clock::time_point momentOfSubmittingACategory = std::chrono::system_clock::now();
        std::chrono::duration<double> measuredDuration = momentOfSubmittingACategory - _startTime;
        std::chrono::system_clock::time_point endTime = _startTime + std::chrono::seconds(_settings.timeLimit);
        
        if (measuredDuration > std::chrono::duration<double>(endTime - _startTime)) {
            _status = BoardStatus::Bewerten;
         // Allow empty entries and check if the starting letter matches.  
        } else if (value[0] == (char) 0) {
            _data[_currentLetter][userName][category] = pair<string, vector<bool>>(value, {});
        } else if (value[0] != _currentLetter) {
            throw invalid_argument("Dein Eintrag beginnt nicht mit dem korrekten Buchstaben.");
        } else {
            _data[_currentLetter][userName][category] = pair<string, vector<bool>>(value, {});
        } 
    }

    void Board::add_category(string category) {
        // Check if game already started.
        if (_status != BoardStatus::Vorbereiten) {
            throw invalid_argument("Kategorie kann nicht erstellt werden: Das Spiel läuft bereits.");
        }

        // Check if category already exists.
        if (find(_categories.begin(), _categories.end(), category) != _categories.end()) {
            throw invalid_argument("Die Kategorie existiert bereits.");
        }

        _categories.push_back(category);
    }

    void Board::remove_category(string category) {
        // Check if game already started.
        if (_status != BoardStatus::Vorbereiten) {
            throw invalid_argument("Kategorie kann nicht gelöscht werden: Das Spiel läuft bereits.");
        }

        // Check if category exists.
        auto itCategory = find(_categories.begin(), _categories.end(), category);
        if (itCategory == _categories.end()) {
            return;
        }

        _categories.erase(itCategory);
    }
    void Board::vote(string userName, string category, bool value) {
        // Checks if player is in voting phase
        if(_status != BoardStatus::Bewerten) {
            throw invalid_argument("Die Kategorien können nicht zu diesem Zeitpunkt bewertet werden.");
        }

        // Checks if the category was evaluated
        if(_data[_currentLetter][userName][category].second.size() > _players.size()-1) {
            throw invalid_argument("Kategorie wurde schon von allen bewertet.");
        }
        
        // Check if category is empty. If so set category bool to false
        if(_data[_currentLetter][userName][category].first == "") {
            _data[_currentLetter][userName][category].second.push_back(false);
        } else {
        // Given boolean value is pushed into the bool array of the player that gets voted on
        _data[_currentLetter][userName][category].second.push_back(value);
        }
    }

    void Board::rate_players() {

        // Check if rating of players can be done.
        if (_status != BoardStatus::Bewerten) {
            throw invalid_argument("Das Spiel kann grade nicht bewertet werden.");
        }

        // Check if there are no more letters or the amount of rounds is over.
        if (_letters.size() == 0 || _settings.roundsAmount - _currentRound == 0) {
            // Update to the results status.
            _status = BoardStatus::Ergebnisse;
        }

        // Loop through each player
        for (auto& player: _players) {
            int number_upvotes = 0;
            // Loop through each category 
            for(auto& category: _categories ) {
                
                // Write the votes of bool in new array 
                std::vector<bool> arr_votes = _data[_currentLetter][player.first][category].second;

                // If vote true, sum the true vote up by one
                for(bool votes: arr_votes) {
                    if(votes) {
                        number_upvotes += 1;
                    }
                }

                // If number of Upvotes at least 50%, category is considered good
                int num_players = _players.size() -1;
                if( (num_players - number_upvotes) <= (num_players/2)) {
                    
                    _players[player.first] += 10;
                }
                // Set back of upvotes per category
                number_upvotes = 0;
            }
        }
    }

    vector<char> Board::get_letters() { return _letters; }

    std::chrono::system_clock::time_point Board::get_startTime() { return _startTime; }
}