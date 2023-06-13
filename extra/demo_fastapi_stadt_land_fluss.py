# Run with
#   uvicorn demo_fastapi_stadt_land_fluss:api --port 8000 --reload
# or, if uvicorn is not in PATH, run as
#   python3 -m uvicorn stadt_land_fluss:api --port 8000  --reload

# Import magic
try:
  import stadt_land_fluss
except ImportError as e:
  print(f"Importing the shared library 'stadt_land_fluss' did not work.")
  print(f"Is (a link to) the shared library 'stadt_land_fluss.____.so' in the same directory as this python script?")
  print(f"The import caused the following exception '{e}'")
  print(f"Exiting")
  exit(1)

from stadt_land_fluss import Controller
from stadt_land_fluss import Board
import os
from fastapi import FastAPI
from pydantic import BaseModel
import uvicorn

def board_toJson(board: Board):
   return {
      "board": {
         "id": board.get_id(),
         "status": board.get_status().name,
         "settings": {
            "roundsAmount": board.get_settings().get_roundsAmount(),
            "allowedLetters": board.get_settings().get_allowedLetters(),
            "endRoundManually": board.get_settings().get_endRoundManually(),
            "timeLimit": board.get_settings().get_timeLimit()
         },
         "data": board.get_data(),
         "categories": board.get_categories(),
         "players": board.get_players(),
         "letters": board.get_letters(),
         "currentLetter": board.get_currentLetter(),
         "currentRound": board.get_currentRound(),
         "startTime": board.get_startTime()
      }
   }

class CreateGameRequest(BaseModel):
    userName: str

# Erstelle einen StadtLandFluss Controller
controller = Controller()

# Mit diesem Objekt wird der Webservice konfiguriert
api = FastAPI()

@api.get("/games")
async def get_games():
    return {
       "games": controller.get_games()
    }

@api.get("/userToken/new")
async def get_userToken():
    return  {
       "userToken": controller.get_userToken()
    }

@api.post("/games")
async def create_game(request: CreateGameRequest, userToken: int):
    return board_toJson(controller.create_game(request.userName, userToken))

if __name__ == '__main__':
  this_python_file = os.path.basename(__file__)[:-3]
  instance = uvicorn.run(f"{this_python_file}:api", host="127.0.0.1", port=8000, log_level="info", reload=True)
