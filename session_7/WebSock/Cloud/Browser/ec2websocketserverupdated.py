import asyncio
import datetime
import random
import websockets

@asyncio.coroutine
def time(websocket, path):
    while True:
        now = datetime.datetime.utcnow().isoformat() + 'Z'
        yield from websocket.send(now)
        yield from asyncio.sleep(random.random() * 3)

start_server = websockets.serve(time, '<Public IP of Instance>', 12345)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()





