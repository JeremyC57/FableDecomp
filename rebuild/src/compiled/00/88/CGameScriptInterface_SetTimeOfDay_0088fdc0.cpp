class CThingDrawInformation
{
public:
    void SetBarPercentage(float percentage);
};

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
    CThingDrawInformation* field1C;
};



void CGameScriptInterface::SetTimeOfDay(float timeOfDay) const
{
    while (timeOfDay >= 24.0f)
    {
        timeOfDay -= 24.0f;
    }

    const SelfOverlay* self = (const SelfOverlay*)this;
    const Field04Overlay* field04 = (const Field04Overlay*)self->field04;
    field04->field1C->SetBarPercentage(
        timeOfDay * (1.0f / 24.0f));
}