import asyncio
import websockets

async def hello():
    async with websockets.connect('ws://<Public IP of Instance>:12345') as websocket:
        name = input("What's your name? ")
        await websocket.send(name)
        print("> {}".format(name))

        greeting = await websocket.recv()
        print("< {}".format(greeting))

asyncio.get_event_loop().run_until_complete(hello())