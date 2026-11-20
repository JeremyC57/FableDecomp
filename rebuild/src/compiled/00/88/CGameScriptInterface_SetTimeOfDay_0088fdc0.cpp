// NF166 (round 10, q11765): the callee 0x006BB810 is CGameTimeManager::SetTimeOfDay (FableWin.pdb 0x02461020 / Ego_r.pdb 0x006F70A0
//   ?SetTimeOfDay@CGameTimeManager@@QAEXM@Z) on the world's PGameTimeManager (+0x1C), engine/CGameTimeManager.h; was the local stand-in
//   CThingDrawInformation::SetBarPercentage (a label propagated from a byte-identical body).
#define FABLE_RETAIL_CGAMETIMEMANAGER
#define FABLE_RETAIL_CGAMETIMEMANAGER_SETTIMEOFDAY
#include "engine/CGameTimeManager.h"

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct SelfOverlay
{
    unsigned char pad00[0x04];
    void* field04;
};

struct Field04Overlay
{
    unsigned char pad00[0x1C];
    CGameTimeManager* field1C;
};



void CGameScriptInterface::SetTimeOfDay(float timeOfDay) const
{
    while (timeOfDay >= 24.0f)
    {
        timeOfDay -= 24.0f;
    }

    const SelfOverlay* self = (const SelfOverlay*)this;
    const Field04Overlay* field04 = (const Field04Overlay*)self->field04;
    field04->field1C->SetTimeOfDay(
        timeOfDay * (1.0f / 24.0f));
}