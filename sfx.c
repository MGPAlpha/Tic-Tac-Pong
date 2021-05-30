//
// Created by mitchell on 5/29/21.
//

#include "sfx.h"
#include "sound.h"

const unsigned short hitNotes[2] = {SND_RATE(NOTE_D, 0), SND_RATE(NOTE_DIS, 0)};
const unsigned short hitTimes[2] = {3, 3};
const sound_effect hit = {hitNotes, hitTimes, 2};

const unsigned short chargePaddleNotes[3] = {SND_RATE(NOTE_D, 0), SND_RATE(NOTE_E, 0), SND_RATE(NOTE_FIS, 0)};
const unsigned short chargePaddleTimes[3] = {3, 3, 5};
const sound_effect chargePaddle = {chargePaddleNotes, chargePaddleTimes, 3};

const unsigned short chargePongNotes[1] = {SND_RATE(NOTE_G, 0)};
const unsigned short chargePongTimes[1] = {5};
const sound_effect chargePong = {chargePongNotes, chargePongTimes, 1};