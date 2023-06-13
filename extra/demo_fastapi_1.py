# Run with
#   uvicorn demo_fastapi_1:meine_coole_rest_api --port 8000 --reload
# or, if uvicorn is not in PATH, run as
#   python3 -m uvicorn demo_fastapi_1:meine_coole_rest_api --port 8000  --reload

import os
from fastapi import FastAPI
import uvicorn

# Mit diesem Objekt wird der Webservice konfiguriert
meine_coole_rest_api = FastAPI()

# Füge den Pfad '/' hinzu
# Wenn dieser Pfad ausgewählt wird, soll die darunter stehende Funktion ausgeführt werden
# Die Rückgabe der Funktion wird den Nutzer:innen (typischerweise als) JSON-Objekt übertragen
@meine_coole_rest_api.get("/")
async def wurzel_pfad():
    return {"coole_nachricht" : "Fast API funktioniert"}


# Füge den Pfad '/user/current_user' hinzu
@meine_coole_rest_api.get("/user/current_user")
async def get_current_user():
    return {"user_id" : "The current user"}


# Füge den unspezifischeren Pfad '/user/{user id}' hinzu
@meine_coole_rest_api.get("/user/{uid}")
async def get_user_by_id(uid: int):
    return {"user_id" : uid}

if __name__ == '__main__':
  this_python_file = os.path.basename(__file__)[:-3]
  instance = uvicorn.run(f"{this_python_file}:meine_coole_rest_api", host="127.0.0.1", port=8000, log_level="info", reload=True)
