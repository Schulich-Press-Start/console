# Contributing to console

These are the ES team's PR norms. For everything else, see the ES onboarding guide pinned in #embedded-software on Discord. Rusty on Git? [GitByBit](https://gitbybit.com/) is a free, interactive course that runs inside VS Code.

## Pull requests

- Nobody pushes straight to `main`. Every change goes through a PR.
- One branch per change, named `yourname/what-it-does`, for example `aly/ili9341-overlay`.
- Keep PRs small: one idea, reviewable in about 15 minutes. Open a draft early if you want feedback.
- In the description, say what changed, why, and how you tested it. For firmware, name the board target (like `nucleo_h755zi_q/stm32h755xx/m7`) and paste logs, or add a photo for anything on a screen. The PR template has the checklist.
- Get one approving review from a teammate, then Yassin does the final review and merges.
- CI has to pass.
- Resolve every review comment before merging. New commits mean the PR needs a fresh approval.
- Never commit build folders, binaries, or secrets.

## AI tools

Use them, but you own every line you commit. If you can't explain it in review, it isn't ready. Check AI answers against the datasheet, the reference manual, or the Zephyr docs, and never paste secrets (API keys, tokens, passwords) into a prompt, a commit, or a screenshot.

## Where things go

- Firmware: `app/`
- Drivers: `drivers/`, with bindings in `dts/bindings/` and headers in `include/app/`
- Our board definition: `boards/`
- Tests: `tests/`
- Week 1 practice games: `practice/`
- Design notes: `docs/`, as a one-page note covering the problem, constraints, options, decision, and open questions
- SDK changes: the [32blit-sdk fork](https://github.com/Schulich-Press-Start/32blit-sdk), not here
