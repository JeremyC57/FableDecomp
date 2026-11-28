#ifndef FABLE_ENGINE_NPLAYERGUI_CDRAWTARGETINFO_H
#define FABLE_ENGINE_NPLAYERGUI_CDRAWTARGETINFO_H
#ifdef FABLE_RETAIL_TARGET_INFO_PASS30
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/CFadeInFadeOutBase.h"
class CEnginePrimitivePrepareInfo;
namespace NPlayerGui {
// The two donor PDBs identify the fields. Native Draw updates them at +0x28 and +0x44.
class CDrawTargetInfo : public CDrawBase {
public:
    void Draw(const CEnginePrimitivePrepareInfo& info);
private:
    CFadeInFadeOutBase AttackAlphaFade;
    CFadeInFadeOutBase BlockAlphaFade;
};
}
#endif
#endif
