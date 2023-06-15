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
from stadt_land_fluss import BoardSettings
import os
from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
import uvicorn

def board_toJson(board: Board):
   return {
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

class CreateGameRequest(BaseModel):
   userName: str

class JoinGameRequest(BaseModel):
   userName: str

class UpdateSettingsRequest(BaseModel):
   roundsAmount: int
   allowedLetters: list
   endRoundManually: bool
   timeLimit: int

class CreateCategoryRequest(BaseModel):
   category: str

class SubmitCategoryRequest(BaseModel):
   value: str

class VoteRequest(BaseModel):
   userName: str # The username of the player to vote for
   value: bool

# Erstelle einen StadtLandFluss Controller
controller = Controller()

# Mit diesem Objekt wird der Webservice konfiguriert
api = FastAPI()

# @api.get("/games")
# async def get_games():
#    try:
#       return {
#          "games": controller.get_games()
#       }
#    except Exception as e:
#       raise HTTPException(status_code=400, detail=str(e))

@api.get("/games/{gameId}")
async def get_games(gameId: int, userToken: int):
   try:
      return board_toJson(controller.get_game(gameId, userToken))
   except Exception as e:
      raise HTTPException(status_code=400, detail=str(e))


@api.post("/games", status_code=201)
async def create_game(request: CreateGameRequest, userToken: int):
   try:
      return board_toJson(controller.create_game(request.userName, userToken))
   except Exception as e:
      raise HTTPException(status_code=400, detail=str(e))

@api.post("/games/{gameId}/join")
async def join_game(gameId: int, userToken: int, request: JoinGameRequest):
   try:
      return board_toJson(controller.join_game(gameId, request.userName, userToken))
   except Exception as e:
      raise HTTPException(status_code=400, detail=str(e))

@api.post("/games/{gameId}/quit", status_code=204)
async def quit_game(gameId: int, userToken: int):
   try:
      controller.quit_game(gameId, userToken)
   except Exception as e:
      raise HTTPException(status_code=400, detail=str(e))

@api.post("/games/{gameId}/start")
async def start_Game(gameId: int, userToken: int):
   try:
      return board_toJson(controller.start_game(gameId, userToken))
   except Exception as e:
      raise HTTPException(status_code=400, detail=str(e))

@api.post("/games/{gameId}/stop")
async def stop_Game(gameId: int, userToken: int):
   try:
      return board_toJson(controller.stop_game(gameId, userToken))
   except Exception as e:
      raise HTTPException(status_code=400, detail=str(e))

@api.put("/games/{gameId}/settings")
async def update_settings(gameId: int, userToken: int, request: UpdateSettingsRequest):
   try:
      boardSettings = BoardSettings()
      boardSettings.set_roundsAmount(request.roundsAmount)
      boardSettings.set_allowedLetters(request.allowedLetters)
      boardSettings.set_endRoundManually(request.endRoundManually)
      boardSettings.set_timeLimit(request.timeLimit)
      return board_toJson(controller.update_settings(gameId, userToken, boardSettings))
   except Exception as e:
      raise HTTPException(status_code=400, detail=str(e))

@api.post("/games/{gameId}/categories", status_code=201)
async def create_category(gameId: int, userToken: int, request: CreateCategoryRequest):
   try:
      return board_toJson(controller.create_category(gameId, userToken, request.category))
   except Exception as e:
      raise HTTPException(status_code=400, detail=str(e))

@api.delete("/games/{gameId}/categories/{category}")
async def delete_category(gameId: int, category: str, userToken: int):
   try:
      return board_toJson(controller.delete_category(gameId, userToken, category))
   except Exception as e:
      raise HTTPException(status_code=400, detail=str(e))

@api.post("/games/{gameId}/categories/{category}/submit")
async def submit_category(gameId: int, category: str, userToken: int, request: SubmitCategoryRequest):
   try:
      return board_toJson(controller.submit_category(gameId, userToken, category, request.value))
   except Exception as e:
      raise HTTPException(status_code=400, detail=str(e))

@api.post("/games/{gameId}/categories/{category}/vote")
async def vote(gameId: int, category: str, userToken: int, request: VoteRequest):
   try:
      return board_toJson(controller.vote(gameId, userToken, request.userName, category, request.value))
   except Exception as e:
      raise HTTPException(status_code=400, detail=str(e))

@api.post("/games/{gameId}/rate")
async def rate(gameId: int, userToken: int):
   try:
      return board_toJson(controller.rate(gameId, userToken))
   except Exception as e:
      raise HTTPException(status_code=400, detail=str(e))

@api.get("/userToken/new")
async def get_userToken():
   try:
      return  {
         "userToken": controller.get_userToken()
      }
   except Exception as e:
      raise HTTPException(status_code=400, detail=str(e))

if __name__ == '__main__':
   this_python_file = os.path.basename(__file__)[:-3]
   instance = uvicorn.run(f"{this_python_file}:api", host="127.0.0.1", port=8000, log_level="info", reload=True)
