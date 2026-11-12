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
    FABLE_STATIC_ASSERT(sizeof(CModeScreamerAggressive) == 0x84);
    FABLE_STATIC_ASSERT(offsetof(CModeScreamerAggressive, PEffect) == 0x74);
    FABLE_STATIC_ASSERT(offsetof(CModeScreamerAggressive, PTarget) == 0x7c);
}
}
#endif
