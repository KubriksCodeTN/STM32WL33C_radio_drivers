#include "my_lpawur.h"
#include "my_mrsubg.h"
#include "primitives.h"
#include "crc_4wkup_rf.h"
#include <string.h>

static frame_sync_t my_frame_sync;

static void wakeup_frame_create(uint8_t *lpawur_frame, frame_sync_t addr, const uint8_t *data) {
	/* bit sync */
	memset(&lpawur_frame[0], 0, 5);

	uint8_t idx = 5;

	/* Frame sync */
#if ADDRESS_LEN == 2
	lpawur_frame[idx++] = addr >> 8;
#endif
	lpawur_frame[idx++] = addr;

	/* Payload */
	memcpy(&lpawur_frame[idx], data, LPAWUR_PAYLOAD_LEN);
	idx += LPAWUR_PAYLOAD_LEN;

	/* CRC */
	uint16_t crc = EvaluateCrc(data);
	lpawur_frame[idx++] = crc >> 8;
	lpawur_frame[idx++] = crc & 0xFF;
}

void LPAWUR_init(frame_sync_t fs, SLPAWUR_RFConfig *radio_cfg,
		SLPAWUR_FrameInit *frame_cfg) {
	my_frame_sync = fs;

	frame_cfg->PayloadLength = LPAWUR_PAYLOAD_LEN;

	lpawur_init(radio_cfg);
	lpawur_frame_init(frame_cfg);

#if ADDRESS_LEN == 1
	lpawur_frame_sync_set(0, fs, false);
#else
	lpawur_frame_sync_set(fs >> 8, fs, true);
#endif
	lpawur_wake_up_lvl_set(WAKEUP_FRAME_VALID);
}

inline void LPAWUR_init_default(frame_sync_t addr) {
	LPAWUR_init(addr, &LPAWUR_DEFAULT_CFG(), &LPAWUR_DEFAULT_FRAME_CFG());
}

void MRSUBG_init(SMRSubGConfig *MRSUBG_RadioInitStruct,
		MRSubG_PcktBasicFields *MRSUBG_PacketSettingsStruct) {
	if (MRSUBG_RadioInitStruct->outputPower <= 10)
		MRSUBG_RadioInitStruct->PADrvMode = PA_DRV_TX;
	else if (MRSUBG_RadioInitStruct->outputPower <= 14)
		MRSUBG_RadioInitStruct->PADrvMode = PA_DRV_TX_HP;
	else
		MRSUBG_RadioInitStruct->PADrvMode = PA_DRV_TX_TX_HP;

	mrsubg_init(MRSUBG_RadioInitStruct);
	mrsubg_frame_init(MRSUBG_PacketSettingsStruct);
}

inline void MRSUBG_init_for_LPAWUR_default(void) {
	MRSUBG_init(&MRSUBG_DEFAULT_WAKEUP_CFG(),
			&MRSUBG_DEFAULT_WAKEUP_FRAME_CFG());
}

inline void MRSUBG_init_default(void) {
	MRSUBG_init(&MRSUBG_DEFAULT_CFG(), &MRSUBG_DEFAULT_FRAME_CFG());
}

void send_wakeup(frame_sync_t frame_sync, const uint8_t *payload, size_t sz) {
	// NB application needs to add address of destination in payload (if needed)
	uint8_t lpawur_frame[FRAME_LEN];
	assert(sz == LPAWUR_PAYLOAD_LEN);
	wakeup_frame_create(lpawur_frame, frame_sync, payload);
	/*
	 printf("DEBUG - Transmitting to LPAWUR: [ ");
	 for (uint8_t i = 0; i < FRAME_LEN; i++) {
	 printf("0x%02X ", lpawur_frame[i]);
	 }
	 printf("]\r\n");
	 */
	mrsubg_send(lpawur_frame, FRAME_LEN);
}

LPAWUR_Status recv_wakeup(uint8_t *payload, size_t sz) {
	return lpawur_recv(payload, sz);
}

inline void send_ack(const uint8_t *ack, size_t sz) {
	// NB application needs to add address of destination in payload (if needed)
	mrsubg_send(ack, sz);
}

inline void send_data(const uint8_t *payload, size_t sz) {
	// NB application needs to add address of destination in payload (if needed)
	mrsubg_send(payload, sz);
}

inline uint32_t recv_main_radio(uint8_t *data, size_t sz, uint32_t recv_duration_ms) {
	return mrsubg_recv(data, sz, recv_duration_ms);
}



