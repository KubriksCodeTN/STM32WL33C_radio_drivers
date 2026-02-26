#ifndef _MY_PRIMITIVES_H_
#define _MY_PRIMITIVES_H_

#include <stddef.h>
#include <stdint.h>

#define LPAWUR_PAYLOAD_LEN 7

#define ADDRESS_LEN 1 ///< frame_sync len
#define PREAMBLE_LEN 5
#define CRC_LEN 2
#define FRAME_LEN (PREAMBLE_LEN + ADDRESS_LEN + LPAWUR_PAYLOAD_LEN + CRC_LEN)

// decide wether or not the FRAME_SYNC is 1 or 2 bytes
#if ADDRESS_LEN == 1
	typedef uint8_t frame_sync_t;
#else
	typedef uint16_t frame_sync_t;
#endif

/**
 @brief initialises the LPAWUR with the given parameters
 */
void LPAWUR_init(frame_sync_t fs, SLPAWUR_RFConfig* radio_cfg, SLPAWUR_FrameInit* frame_cfg);

/**
 @brief initialises the LPAWUR with the default parameters
 */
void LPAWUR_init_default(frame_sync_t fs);


/**
 @brief initialises the MRSUBG with the given parameters
 */
void MRSUBG_init(SMRSubGConfig* MRSUBG_RadioInitStruct, MRSubG_PcktBasicFields* MRSUBG_PacketSettingsStruct);

/**
 @brief initialises the MRSUBG with the default parameters for LPAWUR communication
 */
void MRSUBG_init_for_LPAWUR_default(void);

/**
 @brief initialises the MRSUBG with the default parameters for data exchange
 */
void MRSUBG_init_default(void);


/**
 @brief sends a WUS msg
 */
void send_wakeup(frame_sync_t dest, const uint8_t* payload, size_t sz);

/**
 @brief read the received WUS msg
 */
LPAWUR_Status recv_wakeup(uint8_t* payload, size_t sz);


/**
 @brief sends data using the MRSUBG
 */
void send_data(const uint8_t* payload, size_t sz);
/**
 @brief receives data using the MRSUBG
 */
uint32_t recv_main_radio(uint8_t *data, size_t sz, uint32_t recv_duration_ms);

#endif
