#ifndef FABLE_ENGINE_CNAVIGATIONPOSITION_H
#define FABLE_ENGINE_CNAVIGATIONPOSITION_H
// CNavigationPosition (new header, chunk 736): Ego_r.pdb Position +0 (C2DVector), Layer +8; sizeof 0xC (CNavQuadTree::RegionSeeds
// walk in 0x00A7A0A0: `push [edi+8] ; push edi ; call GetNodeAt ; add edi,0Ch`).
#include "engine/C2DVector.h"
class CNavigationPosition {
public:
    C2DVector Position;                                                  // +0x0
    long      Layer;                                                     // +0x8
};
#endif // FABLE_ENGINE_CNAVIGATIONPOSITION_H
