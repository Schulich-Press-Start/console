# Practice work

Week 1 games and week 2 Zephyr apps go here, one folder each, named after you and the project, like `practice/aly-pong/` or `practice/aly-pipeline/`. CI doesn't build this folder, so practice code can't break `main`. For the full week plans, see the ES onboarding guide pinned in #embedded-software on Discord.

## Week 1: a game on the 32blit SDK

Make **Pong, Snake, or Brick Breaker**. Game logic goes in `update()`, drawing in `render()`, and you need buttons, a score on screen, one sound effect, and a sprite sheet loaded through `assets.yml`.

1. Make a branch: `yourname/practice-game`.
2. Copy our [32blit-boilerplate](https://github.com/Schulich-Press-Start/32blit-boilerplate) fork into your folder, for example `practice/aly-pong/`. Copy the files only, not its `.git` folder.
3. In its `CMakeLists.txt`, change `project(game)` to your game's name. Then fill in `metadata.yml`.
4. Build it for the PC. Point CMake at any 32blit SDK checkout: the one from week 0, or the copy west cloned into `modules/lib/32blit-sdk`.

   ```sh
   cd practice/aly-pong
   cmake -B build -D32BLIT_DIR=/path/to/32blit-sdk
   cmake --build build
   ```

   On Windows, follow the SDK's [Windows guide](https://github.com/32blit/32blit-sdk/blob/master/docs/Windows.md) for the generator to use.
5. Open a PR. Your pair reviews it first, then Yassin does the final review and merges.

## Week 2: an event pipeline on Zephyr

Make it its own Zephyr app. Start from a Zephyr sample, like the shell sample below, and build it from the `console` folder:

```sh
cp -r ../zephyr/samples/subsys/shell/shell_module practice/aly-pipeline
west build -p always -b native_sim practice/aly-pipeline
west build -t run
```

On a Mac, build with `-b qemu_x86` instead (see the ES onboarding guide pinned in #embedded-software on Discord). Open a PR when it works.

Don't commit your `build/` folder. The repo's `.gitignore` already skips it.
