//
// Created by mitchell on 5/29/21.
//

#include "sfx.h"
#include "sound.h"

const unsigned short hitNotes[2] = {SND_RATE(NOTE_D, 0), SND_RATE(NOTE_DIS, 0)};
const unsigned short hitTimes[2] = {3, 3};
const sound_effect hit = {hitNotes, hitTimes, 2};

const unsigned short chargePaddleNotes[5] = {SND_RATE(NOTE_D, 0), SND_RATE(NOTE_E, 0), SND_RATE(NOTE_FIS, 0), SND_RATE(NOTE_GIS, 0),
                                             SND_RATE(NOTE_A, 0)};
const unsigned short chargePaddleTimes[5] = {5, 5, 5, 5, 5};
const sound_effect chargePaddle = {chargePaddleNotes, chargePaddleTimes, 5};

const unsigned short chargePongNotes[1] = {SND_RATE(NOTE_B, 0)};
const unsigned short chargePongTimes[1] = {5};
const sound_effect chargePong = {chargePongNotes, chargePongTimes, 1};

const unsigned short chargeSwitchNotes[2] = {SND_RATE(NOTE_G, 0), SND_RATE(NOTE_F, 0)};
const unsigned short chargeSwitchTimes[2] = {5, 5};
const sound_effect chargeSwitch = {chargeSwitchNotes, chargeSwitchTimes, 2};

const unsigned short wallNotes[2] = {SND_RATE(NOTE_D, -2), SND_RATE(NOTE_CIS, -2)};
const unsigned short wallTimes[2] = {6, 6};
const sound_effect wall = {wallNotes, wallTimes, 2};

const unsigned short markNotes[2] = {SND_RATE(NOTE_D, 1), SND_RATE(NOTE_DIS, 1)};
const unsigned short markTimes[2] = {2, 2};
const sound_effect mark = {markNotes, markTimes, 2};