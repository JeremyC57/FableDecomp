#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWSPIRITTARGETINGCURSOR_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWSPIRITTARGETINGCURSOR_H
// NPlayerGui::CDrawSpiritTargetingCursor (new, wf5, row 0x0064C869 Draw): the player-GUI illustrator that draws, for every non-main
// player with a living hero, the spirit targeting cursor sprite above the thing that hero targets, in the player's multiplayer colour.
// RTTI .?AVCDrawSpiritTargetingCursor@NPlayerGui@@ : CDrawBase (vptr 0x0125A5E8; landed ctor 0x0064DD4D). Ego_r.pdb members
// (struct_layouts_egor.tsv): PTargetedThings (vector<CIntelligentPointer<CThing>>) PDB +0x2c, retail +0x28 (CDrawBase is 0x28 in retail;
// the ctor resizes it to 4 and Draw indexes it as `[this+28h] + i*8`).
// Needs FABLE_RETAIL_CINTELLIGENTPOINTER (the 8-byte retail pointer).
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/CIntelligentPointer.h"
class CThing; class CEnginePrimitivePrepareInfo;
namespace NPlayerGui {
class CDrawSpiritTargetingCursor : public CDrawBase {
public:
    void Draw(const CEnginePrimitivePrepareInfo& info);                  // 0x0064C869 ?Draw@CDrawSpiritTargetingCursor@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
private:
    std::vector<CIntelligentPointer<CThing> > PTargetedThings;           // +0x28
};
}
FABLE_STATIC_ASSERT(sizeof(NPlayerGui::CDrawSpiritTargetingCursor) == 0x34);
#endif
