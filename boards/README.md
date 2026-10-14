# Boards

Our own board definitions go here, starting with the Gen 1 console board once Embedded Hardware hands over the pin map. Until then we build for Zephyr's upstream boards, like `nucleo_h755zi_q/stm32h755xx/m7`.

Zephyr's [board porting guide](https://docs.zephyrproject.org/latest/hardware/porting/board_porting.html) explains the layout, for example `boards/sps/<board>/board.yml`. Compare `boards/st/nucleo_h755zi_q` in the Zephyr tree for a real example.
