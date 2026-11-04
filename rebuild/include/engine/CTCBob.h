#ifndef FABLE_ENGINE_CTCBOB_H
#define FABLE_ENGINE_CTCBOB_H
// CTCBob (new, chunk 765): the bobbing-on-water thing component (TC 242, CGameScriptInterface::ForceShipsVisible 0x008A7F60 queries it).
// Base CTCBase at +0. Ego_r.pdb members (struct_layouts_egor.tsv, sizeof 0x34): Rhset (CRightHandedSet) +0x0C, Pos (C3DVector) +0x24,
// ForceVisible (bool) +0x30. Name set (Ego_r.pdb / FableWin.pdb): ?FrameUpdate@CTCBob@@UAEXXZ (CTCBase slot 9),
// ?SetForceVisible@CTCBob@@QAEXXZ (retail 0x00787740), ?OnInitialActivate@CTCBob@@UAEXXZ (landed 0x00787780).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/CRightHandedSet.h"
class CTCBob : public CTCBase {
public:
    virtual void FrameUpdate();                                          // slot 9
    void SetForceVisible();                                              // 0x00787740
protected:
    CRightHandedSet Rhset;                                               // +0x0c
    C3DVector Pos;                                                       // +0x24
    bool ForceVisible;                                                   // +0x30
};
#endif
