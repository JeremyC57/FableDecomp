#ifndef FABLE_ENGINE_CNAVIGATIONPOSITION_H
#define FABLE_ENGINE_CNAVIGATIONPOSITION_H
// CNavigationPosition (new header, chunk 736): Ego_r.pdb Position +0 (C2DVector), Layer +8; sizeof 0xC (CNavQuadTree::RegionSeeds
// walk in 0x00A7A0A0: `push [edi+8] ; push edi ; call GetNodeAt ; add edi,0Ch`).
#include "engine/C2DVector.h"
class CNavigationPosition {
public:
    C2DVector Position;                                                  // +0x0
    long      Layer;                                                     // +0x8
#ifdef FABLE_RETAIL_CNAVIGATIONPOSITION_CTORS
    // Nested opt-in (chunk 853, row 0x0050A650): FableWin.pdb ??0CNavigationPosition@@QAE@XZ and ??0CNavigationPosition@@QAE@ABVC2DVector@@J@Z
    // (inline in Ego_r and retail; the copy ??0..@ABV0@@Z is compiler-generated per the PDB method attributes). No layout change.
    CNavigationPosition() { }
    CNavigationPosition(const C2DVector& position, long layer) : Position(position), Layer(layer) { }
#endif
};
#endif // FABLE_ENGINE_CNAVIGATIONPOSITION_H
