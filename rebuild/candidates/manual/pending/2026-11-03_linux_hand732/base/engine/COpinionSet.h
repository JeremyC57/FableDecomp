#ifndef FABLE_ENGINE_COPINIONSET_H
#define FABLE_ENGINE_COPINIONSET_H
// COpinionSet (chunk 728 round 2, row 0x006D1E30 CTCWife::FrameUpdate): Ego_r.pdb CArray<float> OpinionValues at +0 (retail CArray 0xC);
// FrameUpdate reads OpinionValues[3] / [4] through the begin pointer (`mov ebx,[ebx+48h] ; mov eax,[ebx+0Ch]`).
#include "engine/CArray.h"
class COpinionSet {
public:
    CArray<float> OpinionValues;                             // +0x00
};
#endif
