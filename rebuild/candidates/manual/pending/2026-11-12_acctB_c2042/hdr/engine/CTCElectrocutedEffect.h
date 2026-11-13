#ifndef FABLE_ENGINE_CTCELECTROCUTEDEFFECT_H
#define FABLE_ENGINE_CTCELECTROCUTEDEFFECT_H
// New shared header (w22 agent2042, row 0x007F2DC0 CTCLightningNode::FrameUpdate): the electrocuted-effect component. Ego_r.pdb / FableWin.pdb
// ?StatGetInterfaceType@CTCElectrocutedEffect@@SI?AW4ETCInterfaceType@@XZ (TCI_ELECTROCUTED_EFFECT = 138: FrameUpdate's inline GetTC reads
// bitset word +0x30 bit 10 and looks up key 0x8A, 0x007F348D..0x007F34BD) and ?Reset@CTCElectrocutedEffect@@QAEXXZ (out of line at 0x007CE7A0,
// the bound spelling). Members not modelled (declarations only). Needs FABLE_RETAIL_CTCBASE.
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CTCElectrocutedEffect : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ELECTROCUTED_EFFECT; }
    void Reset();                                                // 0x007CE7A0
};
#endif
