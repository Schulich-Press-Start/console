# console

The Embedded Software (ES) team's home base for the Schulich Press Start Gen 1 handheld. This repo holds the Zephyr workspace manifest and the console firmware, and it's where most ES work happens.

New to the team? See the ES onboarding guide pinned in #embedded-software on Discord.

## How it fits with our forks

console is a [west T2 workspace](https://docs.zephyrproject.org/latest/develop/west/workspaces.html): it holds `west.yml`, and `west update` pulls everything else in next to it. No fork is the baseline.

- **Zephyr** comes straight from upstream, pinned to a release (v4.4.2) in `west.yml`. We only fork it if we ever have to patch Zephyr itself.
- **[32blit-sdk](https://github.com/Schulich-Press-Start/32blit-sdk)** is our fork of the 32blit SDK and the base for our SDK. The Zephyr platform port will live there next to `32blit-pico` and `32blit-stm32`, so we can keep pulling upstream changes. west clones it into `modules/lib/32blit-sdk`.
- **[32blit-boilerplate](https://github.com/Schulich-Press-Start/32blit-boilerplate)** is the game template for Game Design and the week 1 practice games.
- **[DaftBoy32](https://github.com/Schulich-Press-Start/DaftBoy32)** and **[Peanut-GB](https://github.com/Schulich-Press-Start/Peanut-GB)** are for the emulator bake-off. Homebrew and test ROMs only.

SDK changes go to the SDK fork in their own PRs, and console's `west.yml` decides which version we build against.

## Layout

```text
sps-workspace/               made by west init
├── console/                 this repo
│   ├── app/                 the console firmware (hello world for now)
│   ├── boards/              our own board definitions, once the Gen 1 pin map exists
│   ├── drivers/             out-of-tree drivers
│   ├── dts/bindings/        devicetree bindings for those drivers
│   ├── include/app/         headers for our drivers and libraries
│   ├── lib/                 our libraries
│   ├── tests/               ztest suites, run on native_sim in CI
│   ├── practice/            week 1 practice games
│   ├── tools/sdk-docker/    the SDK in Docker, with games running in your browser
│   ├── zephyr/module.yml    makes console a Zephyr module
│   └── west.yml             Zephyr version, modules, and forks
├── zephyr/                  upstream Zephyr, pinned
└── modules/
    ├── hal/                 cmsis_6 and stm32
    └── lib/32blit-sdk/      our SDK fork
```

The `blink` and `example_sensor` drivers, their bindings, and the `custom` library and its test come from Zephyr's [example-application](https://github.com/zephyrproject-rtos/example-application) template. They're a good week 2 reference for devicetree, Kconfig, and a driver end to end. We'll delete them once we have our own.

## Quick start

Install the dependencies for your OS from Zephyr's [Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html). Then follow its steps with our workspace in place of `~/zephyrproject`:

```sh
python3 -m venv ~/sps-workspace/.venv
source ~/sps-workspace/.venv/bin/activate
pip install west

west init -m https://github.com/Schulich-Press-Start/console --mr main ~/sps-workspace
cd ~/sps-workspace
west update

west zephyr-export
west packages pip --install
cd zephyr && west sdk install && cd ..
```

On Windows, use the Getting Started Guide's Windows commands for the venv and the `west packages` step.

## Build

From `~/sps-workspace/console`, with the venv active:

```sh
# the Nucleo H755ZI-Q dev kit, Cortex-M7 core. building doesn't need the board
west build -p always -b nucleo_h755zi_q/stm32h755xx/m7 app
west flash    # uses STM32CubeProgrammer

# Linux or WSL 2 only: run the firmware as a normal program
west build -p always -b native_sim app
west build -t run
```

`native_sim` doesn't run on macOS. CI builds it for you, and Mac users can try QEMU for week 2 (see the ES onboarding guide pinned in #embedded-software on Discord).

Tests (Linux or WSL 2):

```sh
west twister -T tests --integration
```

## CI

Every PR and every push to `main` builds the app for `nucleo_h755zi_q/stm32h755xx/m7` and `native_sim`, and runs the tests on `native_sim` (`.github/workflows/build.yml`). It runs on Linux only to keep it quick.

## Contributing

Read [CONTRIBUTING.md](CONTRIBUTING.md). The short version: one branch per change, small PRs, a teammate approves, then Yassin does the final review and merges.

## License

Apache-2.0 (see [LICENSE](LICENSE)), inherited from Zephyr's example-application template.
