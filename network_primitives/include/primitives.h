#ifndef _MY_PRIMITIVES_H_
#define _MY_PRIMITIVES_H_


#include <stddef.h>
#include <stdint.h>

#define LPAWUR_PAYLOAD_LEN 7

#define ADDRESS_LEN 1
#define PREAMBLE_LEN 5
#define CRC_LEN 2
#define FRAME_LEN (PREAMBLE_LEN + ADDRESS_LEN + LPAWUR_PAYLOAD_LEN + CRC_LEN)

#if ADDRESS_LEN == 1
	typedef uint8_t frame_sync_t;
#else
	typedef uint16_t frame_sync_t;
#endif

void LPAWUR_init(frame_sync_t fs, SLPAWUR_RFConfig* radio_cfg, SLPAWUR_FrameInit* frame_cfg);
void LPAWUR_init_default(frame_sync_t fs);

void MRSUBG_init(SMRSubGConfig* MRSUBG_RadioInitStruct, MRSubG_PcktBasicFields* MRSUBG_PacketSettingsStruct);
void MRSUBG_init_for_LPAWUR_default(void);
void MRSUBG_init_default(void);

void send_wakeup(frame_sync_t dest, const uint8_t* payload, size_t sz);
LPAWUR_Status recv_wakeup(uint8_t* payload, size_t sz);

void send_ack(const uint8_t* ack, size_t sz);
void send_data(const uint8_t* payload, size_t sz);
uint32_t recv_main_radio(uint8_t *data, size_t sz, uint32_t recv_duration_ms);

#endif
