class C3DVector
{
public:
    float x;
    float y;
    float z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) {}
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CGameScriptInterface_Overlay
{
    char pad_0x00[0x04];
    void* field_0x04;
};

struct GuildSealRecallPosSource_Overlay
{
    char pad_0x00[0x154];
    C3DVector field_0x154;
};

C3DVector CGameScriptInterface::GetGuildSealRecallPos() const
{
    const GuildSealRecallPosSource_Overlay* const source =
        reinterpret_cast<const GuildSealRecallPosSource_Overlay*>(
            reinterpret_cast<const CGameScriptInterface_Overlay*>(this)->field_0x04);
    return source->field_0x154;
}