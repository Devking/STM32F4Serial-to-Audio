// speaker (audio out) functions 
// utilizing library from http://www.mind-dump.net/configuring-the-stm32f4-discovery-for-audio  by  A.Finkelmeyer

#include "speaker.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "codec.h"

// initialize the speaker (audio output)
void init_speaker() {
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	codec_init();
	codec_ctrl_init();
	I2S_Cmd(CODEC_I2S, ENABLE);
}

// send one audio sample to the speaker
void send_to_speaker(int16_t audio_sample) {
	// same sample for both left and right speakers in stereo speakers
	if (SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE)) {
			SPI_I2S_SendData(CODEC_I2S, audio_sample);
		}
	if (SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE)) {
			SPI_I2S_SendData(CODEC_I2S, audio_sample);
		}
}
