#include <stadt_land_fluss/controller.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>

namespace py = pybind11;
using namespace StadtLandFluss;

PYBIND11_MODULE(stadt_land_fluss, m) {
  m.doc() = "StadtLandFluss Beispiel";

  py::enum_<BoardStatus>(m, "BoardStatus")
    .value("Vorbereiten", Vorbereiten)
    .value("Spielen", Spielen)
    .value("Bewerten", Bewerten)
    .value("Ergebnisse", Ergebnisse)
    .export_values();

  py::class_<BoardSettings>(m, "BoardSettings")
    .def(py::init<>())
    .def("get_roundsAmount", &BoardSettings::get_roundsAmount)
    .def("get_allowedLetters", &BoardSettings::get_allowedLetters)
    .def("get_endRoundManually", &BoardSettings::get_endRoundManually)
    .def("get_timeLimit", &BoardSettings::get_timeLimit);
  
  py::class_<Controller>(m, "Controller")
    .def(py::init<>())
    .def("get_games", &Controller::get_games)
    .def("get_userToken", &Controller::get_userToken)
    .def("create_game", &Controller::create_game)
    .def("join_game", &Controller::join_game)
    .def("quit_game", &Controller::quit_game)
    .def("start_game", &Controller::start_game)
    .def("stop_game", &Controller::stop_game)
    .def("get_game", &Controller::get_game)
    .def("update_settings", &Controller::update_settings)
    .def("create_category", &Controller::create_category)
    .def("delete_category", &Controller::delete_category)
    .def("submit_category", &Controller::submit_category)
    .def("vote", &Controller::vote)
    .def("rate", &Controller::rate);

  py::class_<Board>(m, "Board")
    .def("get_id", &Board::get_id)
    .def("get_status", &Board::get_status)
    .def("get_settings", &Board::get_settings)
    .def("get_data", &Board::get_data)
    .def("get_categories", &Board::get_categories)
    .def("get_players", &Board::get_players)
    .def("get_letters", &Board::get_letters)
    .def("get_currentLetter", &Board::get_currentLetter)
    .def("get_currentRound", &Board::get_currentRound)
    .def("get_startTime", &Board::get_startTime);
}
