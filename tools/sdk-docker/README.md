# SDK in Docker

Try the SPS SDK (our [32blit-sdk fork](https://github.com/Schulich-Press-Start/32blit-sdk)) with nothing installed but Docker. Games are built for the browser, so it works the same on Windows, macOS, and Linux. Use it when your native setup is fighting you, or to show someone the SDK quickly.

## Run the examples

```sh
cd tools/sdk-docker
docker compose up --build
```

Open <http://localhost:8080> and pick a game. The first build downloads about 1 GB and takes a few minutes. After that, it starts in seconds. Stop it with Ctrl+C.

Controls: arrows or WASD move, Z, X, C, and V are A, B, X, and Y, and Esc opens the menu.

## Run your own game

Point `GAME_DIR` at your week 1 game, a folder made from our [32blit-boilerplate](https://github.com/Schulich-Press-Start/32blit-boilerplate) fork:

```sh
GAME_DIR=../../practice/aly-pong docker compose up --build
```

In PowerShell:

```powershell
$env:GAME_DIR="../../practice/aly-pong"; docker compose up --build
```

Your game shows up in the list next to the examples. It's rebuilt every time the container starts, so after a change, press Ctrl+C and run the same command again.

## Notes

- You need Docker Desktop, OrbStack, or any Docker with Compose v2.
- Apple Silicon Macs run it through Rosetta, because Emscripten 2.0.18, the version 32blit's own CI uses, only ships for x86. Builds are slower there, but it works.
- This builds for the browser, not for our console's hardware. Firmware work still needs the Zephyr setup in the main [README](../../README.md).
