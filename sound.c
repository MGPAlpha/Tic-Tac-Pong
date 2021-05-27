//
// Created by mitchell on 5/22/21.
//

#include "sound.h"

const unsigned short __snd_rates[12] = {
    8013, 7566, 7144, 6742, // C , C#, D , D#
    6362, 6005, 5666, 5346, // E , F , F#, G
    5048, 4766, 4499, 4246  // G#, A , A#, B
};

#define SND_RATE(note, oct) ( 2048-(__snd_rates[note]>>(4+(oct))) )

void enableSound(void) {
    //turn on sound
    REG_SNDSTAT = SSTAT_ENABLE;

    //set l/r volume to full
    REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR1 | SDMG_SQR2, 7);

    //dmg ratio to 100%
    REG_SNDDSCNT = SDS_DMG100;

    REG_SND1SWEEP = SSW_OFF;

    REG_SND1CNT = SSQR_ENV_BUILD(15, 0, 0) | SSQR_DUTY1_2;

    REG_SND1FREQ = SFREQ_RESET | SND_RATE(NOTE_C, 0);
}
