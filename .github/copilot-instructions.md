# Copilot instructions for console

console is the Schulich Press Start Embedded Software team's firmware repo: a Zephyr v4.4.2 west workspace in C, built for `nucleo_h755zi_q/stm32h755xx/m7` (the STM32H755's Cortex-M7 core) and `native_sim`. Gen 1 has since moved to the Allwinner T113-S3 with Linux (Buildroot), and the Zephyr workspace is from before that decision. Practice PRs in `practice/` are usually 32blit games or plain C++ and Linux programs (threads, evdev, uinput, timerfd). Most contributors are students, and some are new to embedded work.

## Reviewing pull requests

- Keep reviews short. Lead with real problems: bugs, undefined behaviour, memory safety, thread and ISR safety (including blocking calls in ISRs), wrong devicetree or Kconfig usage, and unchecked return values from Zephyr or Linux APIs.
- Skip style nits unless they hide a bug. Group repeated issues into one comment instead of flagging every line.
- Say why something is wrong and suggest a fix. A link to the relevant Zephyr doc helps newer teammates.
- Don't guess about registers, devicetree properties, or Kconfig symbols. If you can't confirm one for Zephyr v4.4, say you're unsure.
- Check our PR norms: one idea per PR, a description that says what changed, why, and how it was tested (with the target and logs, or a photo for anything on a screen), and no build output, binaries, or secrets committed.
- Leave comments only. A teammate approves first, then Yassin does the final review and merges.

## Files to skip

- Generated or vendored content: `build*/` and `twister-out*/` output, anything west pulls into the workspace (Zephyr and `modules/`), and binary assets such as images and sprite sheets.
- The template examples (`drivers/blink`, `drivers/sensor/example_sensor`, `lib/custom`, `tests/lib/custom`) unless the PR changes them on purpose.
