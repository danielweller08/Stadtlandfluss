#include <stadt_land_fluss/board.hpp>
#include <iostream>
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
        _data[_currentLetter][userName][category] = pair<string, vector<bool>>(value, {});
    }

    void Board::add_category(string category) {
        // Check if category already exists.
        if (find(_categories.begin(), _categories.end(), category) != _categories.end()) {
            throw invalid_argument("Die Kategorie existiert bereits.");
        }

        _categories.push_back(category);
    }

    void Board::remove_category(string category) {
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

        // Checks if there are any votes left
        if(_data[_currentLetter][userName][category].second.size() > _players.size()) {
            throw invalid_argument("Alle Kategorien wurden schon bewertet.");
        }

        //_data[_currentLetter][userName][category].second.size()
        

        // Player votes with given boolean value
        _data[_currentLetter][userName][category].second.push_back(value);

        std::cout << "Die Anzahl von Votes sind:  " << to_string(_data[_currentLetter][userName][category].second.size()) + "\n";
        std::cout << "Die Anzahl von Spielern sind:  " << to_string(_players.size()) + "\n";
    }

    int Board::rate_players() {
        /*
           In data stehen die guesses der einzelnen User drinnen -> Map(category of player, array<bools>)
           Auswertung: 1) Kein Punkt wenn Buchstabe nicht mit guess übereinstimmt oder Eintrag leer
                       2) 5 Punkte bei gleichem guess und durch votes gültig
                       3) 10 Punkte für alleinigen guess und durch votes gültig

           Wird in _players mit neuem Punktestand aktualisiert
        */

       //_data[_currentLetter][userName][category].second.size();

        std::vector<std::string> players;
       for (auto& player: _players) {
            //for(auto )
            players.push_back(player.first);
            std::cout << "Spieler in Runde sind:" << player.first << ", ";
            std::cout << _currentLetter;

            // Schleifendurchgang durch jede Kategorie und Spielernamen
            //_data[_currentLetter][players[0]][category].second

       }

        


        // Add the user with a score of 0.
        //_players[userName] = 0;

       // Votes von anderen usern aufaddieren -> Mehrheit von true's gewinnt
       // -> In arr von bool reinschreiben [true, true, false], arr von punkten = [0,0,0]
       // Eintrag überprüfen auf 2), in dem man durchiteratiert und auf gleichheit prüft. Wenn gleich -> arr_punkte[eintrag]=(5)
       //                                                                                 Else -> arr_punkte[eintrag] = 10
       // in for schleife punkte aufsummieren
       // Ergebnis returnen
       return 0;
    }
}