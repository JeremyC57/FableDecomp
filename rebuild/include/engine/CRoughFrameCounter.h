#ifndef FABLE_ENGINE_CROUGHFRAMECOUNTER_H
#define FABLE_ENGINE_CROUGHFRAMECOUNTER_H
// CRoughFrameCounter (chunk 1022, row 0x00417747): Ego_r.pdb layout (8 bytes: FrameStart, FPS) and
// ?NewFrame@CRoughFrameCounter@@QAEXXZ (Ego_r 0x00B4BE10), out of line in retail at 0x009F8BA0 (landed).
#include "rebuild_abi.h"
class CRoughFrameCounter {
public:
    void NewFrame();                                         // 0x009F8BA0
    unsigned long FrameStart;                                // +0x00
    float         FPS;                                       // +0x04
};
FABLE_STATIC_ASSERT(sizeof(CRoughFrameCounter) == 0x8);
#endif // FABLE_ENGINE_CROUGHFRAMECOUNTER_H
