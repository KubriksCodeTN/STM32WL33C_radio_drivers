/*
 * lpawur.c
 *
 *  Created on: Jun 10, 2025
 *      Author: sdagn
 */
#include "stm32wl3x_hal.h"

#include "my_lpawur.h"
#include <assert.h>
#include "stm32_lpm.h"
#include "stm32wl3x_ll_usart.h"

static uint16_t manchester_encode(uint8_t in) {
	uint16_t ret = 0;
	for (int i = 0; i < 8 * sizeof(in); ++i) {
		if ((in >> i) & 0x1) {
			ret |= (1 << (2 * i + 1));
		} else {
			ret |= (1 << (2 * i));
		}
	}

	return ret;
}

void lpawur_init(SLPAWUR_RFConfig *radio_cfg) {
	assert(radio_cfg != NULL);
	HAL_LPAWUR_RFConfigInit(radio_cfg);
}

void lpawur_frame_init(SLPAWUR_FrameInit *frame_cfg) {
	assert(frame_cfg != NULL);
	HAL_LPAWUR_FrameInit(frame_cfg);
}

void lpawur_wake_up_lvl_set(WakeUpLevel lv) {
	LL_LPAWUR_SetWakeUpLevel(lv);
}

void lpawur_enable() {
	LL_LPAWUR_SetState(ENABLE);
}

void lpawur_disable() {
	LL_LPAWUR_SetState(DISABLE);
}

void lpawur_frame_sync_set(uint8_t fs_high, uint8_t fs_low, bool fs_16bit) {
	HAL_LPAWUR_SetSync(
			(uint32_t) (manchester_encode(fs_high) << 16)
					| (uint32_t) manchester_encode(fs_low));
	LL_LPAWUR_SetSyncLength(fs_16bit);
	LL_LPAWUR_SetSyncThrCnt(16);

	if (fs_16bit) {
		LL_LPAWUR_SetSyncThrCnt(32);
	}
}

LPAWUR_Status lpawur_recv(uint8_t *data, size_t sz) {
	assert(sz < LPAWUR_PAYLOAD_LEN_MAX);

	uint8_t length = LL_LPAWUR_GetPayloadLength();
	if (sz < length) {
		return NO_STATUS;
	}

	HAL_LPAWUR_GetPayload(data);

	LPAWUR_Status ret = HAL_LPAWUR_GetStatus();
	HAL_LPAWUR_ClearStatus();

	return ret;
}
