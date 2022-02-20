//
// Created by mitchell on 5/22/21.
//

#include "sound.h"
#include "gba.h"

static const sound_effect *currSound;
static unsigned short noteIndex;
static unsigned short currNoteElapsed;


static void startNote(unsigned short index) {
  REG_SND1CNT = SSQR_ENV_BUILD(15, 0, 7) | SSQR_DUTY1_2;
  REG_SND1FREQ = SFREQ_RESET | currSound->notes[index];
  currNoteElapsed = 0;
}

static void stopNote(void) {
  REG_SND1CNT = SSQR_ENV_BUILD(0, 0, 1) | SSQR_DUTY1_2;
  // REG_SND1FREQ = 0;
}

void enableSound(void) {
    //turn on sound
    REG_SNDSTAT = SSTAT_ENABLE;

    //set l/r volume to full
    REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR1 | SDMG_SQR2, 7);

    //dmg ratio to 100%
    REG_SNDDSCNT = SDS_DMG100;

    REG_SND1SWEEP = SSW_OFF;

    REG_SND1CNT = SSQR_ENV_BUILD(15, 0, 0) | SSQR_DUTY1_2;
}

void soundUpdate(void) {
  if (currSound && ++currNoteElapsed >= currSound->times[noteIndex]) {
    if (noteIndex >= currSound->size - 1) {
      stopNote();
      currSound = NULL;
      noteIndex = 0;
    } else {
      noteIndex++;
      startNote(noteIndex);
    }
  }
}

void playSoundEffect(const sound_effect *effect) {
    currSound = effect;
    noteIndex = 0;
    startNote(0);
}
