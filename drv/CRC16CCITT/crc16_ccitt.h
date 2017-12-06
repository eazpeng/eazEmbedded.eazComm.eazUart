#ifndef __CRC16_CCITT_H__
#define __CRC16_CCITT_H__

#ifdef __cplusplus
extern "C" {
#endif

    void update_crc(unsigned short ch);
	unsigned short cal_crc(char* data, int size);

#ifdef __cplusplus
}
#endif

#endif
