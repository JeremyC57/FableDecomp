#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
enum EMusicSetType { EMS_A };

struct CMusicTarget
{
    void CacheMusicSet_006bde50(EMusicSetType isEnabled);
};

struct CWorld
{
    char pad_0000[0x6C];
    CMusicTarget* target_006C;
};

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void CacheMusicSet(EMusicSetType isEnabled) const;
};

void CGameScriptInterface_Methods::CacheMusicSet(EMusicSetType isEnabled) const
{
    World->target_006C->CacheMusicSet_006bde50(isEnabled);
}