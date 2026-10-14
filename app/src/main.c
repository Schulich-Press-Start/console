/*
 * SPS console firmware.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/app_version.h>

int main(void)
{
	/* hello world for now. the 32blit platform port takes over this file later */
	printf("SPS console %s: hello from %s\n", APP_VERSION_STRING, CONFIG_BOARD_TARGET);

	return 0;
}
