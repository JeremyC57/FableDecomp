#ifndef FABLE_ENGINE_CGAMETIMEMANAGER_H
#define FABLE_ENGINE_CGAMETIMEMANAGER_H
// CGameTimeManager (chunk 728 round 2, row 0x006D0830 CTCWife::GlobalUpdate): CWorld::PGameTimeManager (+0x1C). Ego_r.pdb layout
// (sizeof 0x40 there; retail 0x3C: the STLport set StopTimeIndexes is 12 bytes). Only the member GlobalUpdate calls is declared:
// Ego_r.pdb / FableWin.pdb ?FastForwardTimeTo@CGameTimeManager@@QAEXMM@Z (out of line, 0x006BB720; the only spelling bound there).
#include "rebuild_abi.h"
class CWorld;
class CMainGameComponent;
class CGameTimeManager {
public:
    void FastForwardTimeTo(float time, float duration);                 // 0x006BB720
};
#endif
