#ifndef FABLE_ENGINE_NCREATUREMODE_CMODESCREAMERAGGRESSIVE_H
#define FABLE_ENGINE_NCREATUREMODE_CMODESCREAMERAGGRESSIVE_H
// New header (row 0x008DA410 OnActivate): NCreatureMode::CModeScreamerAggressive : CModeStandBase (Ego_r.pdb layout,
// ghidra_out/struct_layouts_egor.tsv: base 0x68, Filter CAnimFilterOfGroup +0x68, PEffect CIntelligentPointer<CThing> +0x74,
// PTarget CIntelligentPointer<CThing> +0x7C; sizeof 0x84). vtable 0x012681EC: slot 0 dtor 0x00741370, 1 GetType 0x00741350,
// 3 FrameUpdate 0x008DA610, 4 GetAnimFilter 0x00741360, 5 OnActivate 0x008DA410, 6 OnDeactivate 0x008DA690.
// Needs FABLE_RETAIL_CMODEBASE_VIRTUALS, FABLE_RETAIL_CMODESTANDBASE and FABLE_RETAIL_CINTELLIGENTPOINTER. No default path.
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CIntelligentPointer.h"
namespace NCreatureMode {
class CModeScreamerAggressive : public CModeStandBase {
public:
#ifdef FABLE_RETAIL_NCREATUREMODE_CMODESCREAMERAGGRESSIVE_VTABLE
    // NF247 opt-in (q12682, row 0x00741390, the implicit destructor that the ctor's vtable emission materialises): the (CThing&)
    // constructor ??0CModeScreamerAggressive@NCreatureMode@@QAE@AAVCThing@@@Z, slot 1 ?GetType@CModeScreamerAggressive@NCreatureMode@@UBEKXZ
    // (0x00741350) and slot 4 ?GetAnimFilter@CModeScreamerAggressive@NCreatureMode@@UBEABVCAnimFilterBase@@XZ (0x00741360), each in
    // Ego_r.pdb / FableWin.pdb / Ego_d.pdb; vtable 0x012681EC slots 1 / 4. Declarations only: no layout change.
    CModeScreamerAggressive(CThing& thing);
    virtual unsigned long GetType() const;                               // 0x00741350
    virtual const CAnimFilterBase& GetAnimFilter() const;                // 0x00741360
#endif
    virtual void FrameUpdate();                                          // 0x008DA610
    virtual void OnActivate();                                           // 0x008DA410
    virtual void OnDeactivate();                                         // 0x008DA690
private:
    CAnimFilterOfGroup Filter;                                           // +0x68
    CIntelligentPointer<CThing> PEffect;                                 // +0x74
    CIntelligentPointer<CThing> PTarget;                                 // +0x7c
    static void CheckRetailLayout();
};
inline void CModeScreamerAggressive::CheckRetailLayout() {
    FABLE_STATIC_ASSERT_AT(25, sizeof(CModeScreamerAggressive) == 0x84);
    FABLE_STATIC_ASSERT_AT(26, offsetof(CModeScreamerAggressive, PEffect) == 0x74);
    FABLE_STATIC_ASSERT_AT(27, offsetof(CModeScreamerAggressive, PTarget) == 0x7c);
}
}
#endif
