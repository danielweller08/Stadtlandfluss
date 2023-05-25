#include <stadt_land_fluss/board.hpp>

#include <stdexcept>
#include <algorithm>

using namespace std;

namespace StadtLandFluss {
    Board::Board() : Board(0) {}

    Board::Board(int id) : _id(id), _status(BoardStatus::Vorbereiten) {
        _settings = BoardSettings();
        _settings.allowedLetters = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'R', 'S', 'T', 'U', 'V', 'W', 'Z' };
        _settings.endRoundManually = true;
        _settings.roundsAmount = 10;
        _settings.timeLimit = 60;
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

    map<char,map<string,map<string,string>>> Board::get_data() { return _data; }

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

        // TODO: Timer.

        // Update the status.
        _status = BoardStatus::Spielen;
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

        // Check if there are no more letters or the amount of rounds is over.
        if (_letters.size() == 0 || _settings.roundsAmount - _currentRound == 0) {
            // Update to the results status.
            _status = BoardStatus::Ergebnisse;
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

        // Insert the category value.
        _data[_currentLetter][userName][category] = value;
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
}