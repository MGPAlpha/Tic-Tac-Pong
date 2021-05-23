//
// Created by mitchell on 5/22/21.
//

#include "sound.h"

void enableSound(void) {
    //turn on sound
    REG_SNDSTAT = SSTAT_ENABLE;

    //set l/r volume to full
    REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR1 | SDMG_SQR2, 7);

    //dmg ratio to 100%
    REG_SNDDSCNT = SDS_DMG100;

    REG_SND1SWEEP = SSW_OFF;

    REG_SND1CNT = SSQR_ENV_BUILD(10, 1, 0) | SSQR_DUTY1_2;

    REG_SND1FREQ = 0;
}
