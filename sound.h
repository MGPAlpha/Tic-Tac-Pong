//
// Created by mitchell on 5/22/21.
//

#ifndef TIC_TAC_PONG_SOUND_H
#define TIC_TAC_PONG_SOUND_H

#include <stddef.h>

//Sound DMG Control
#define REG_SNDDMGCNT (*(volatile unsigned short *) 0x4000080)

#define SDMG_SQR1    0x01
#define SDMG_SQR2    0x02
#define SDMG_WAVE    0x04
#define SDMG_NOISE   0x08

#define SDMG_BUILD(_lmode, _rmode, _lvol, _rvol)    \
    ( ((_lvol)&7) | (((_rvol)&7)<<4) | ((_lmode)<<8) | ((_rmode)<<12) )

#define SDMG_BUILD_LR(_mode, _vol) SDMG_BUILD(_mode, _mode, _vol, _vol)

//Sound DS Control
#define REG_SNDDSCNT (*(volatile unsigned short *) 0x4000082)

#define SDS_DMG25 0x0
#define SDS_DMG50 0x1
#define SDS_DMG100 0x2

//Sound Status
#define REG_SNDSTAT (*(volatile unsigned short *) 0x4000084)
#define SSTAT_ENABLE (1<<7)

//Sound Control Regs
#define REG_SND1SWEEP (*(volatile unsigned short *) 0x4000060)
#define SSW_OFF 0

#define REG_SND1CNT (*(volatile unsigned short *) 0x4000062)
#define REG_SND1FREQ (*(volatile unsigned short *) 0x4000064)
#define REG_SND2CNT (*(volatile unsigned short *) 0x4000068)
#define REG_SND2FREQ (*(volatile unsigned short *) 0x400006C)

#define SSQR_ENV_BUILD(vol, dir, step) ( (((vol) & 0xf) << 0xC) | (((dir)&1) << 0xB) | (((step)&7) << 0x8) )

#define SSQR_DUTY1_8 (0<<6)
#define SSQR_DUTY1_4 (1<<6)
#define SSQR_DUTY1_2 (2<<6)
#define SSQR_DUTY3_4 (3<<6)

#define SFREQ_RESET 0x8000

#define NOTE_C 8013
#define NOTE_CIS 7566
#define NOTE_D 7144
#define NOTE_DIS 6742
#define NOTE_E 6362
#define NOTE_F 6005
#define NOTE_FIS 5666
#define NOTE_G 5346
#define NOTE_GIS 5048
#define NOTE_A 4766
#define NOTE_BES 4499
#define NOTE_B 4246

#define SND_RATE(note, oct) ( 2048-((note)>>(4+(oct))) )

typedef struct {
    const unsigned short *notes, *times;
    const size_t size;
} sound_effect;



void enableSound(void);

void soundUpdate(void);

void playSoundEffect(const sound_effect *effect);

#endif //TIC_TAC_PONG_SOUND_H
