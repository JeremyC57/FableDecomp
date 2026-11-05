// Leaf: ?EnableDangerMusic@CGameScriptInterface_Methods@@QBEXW4EMusicSetType@@@Z
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
enum EMusicSetType { EMS_A };

struct CMusicTarget
{
    void CacheMusicSet_006bde50(EMusicSetType isEnabled);
};

class CWorld
{ public:
    char pad_0000[0x6C];
    CMusicTarget* target_006C;
};

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void EnableDangerMusic(EMusicSetType isEnabled) const;
};

void CGameScriptInterface_Methods::EnableDangerMusic(EMusicSetType isEnabled) const
{
    World->target_006C->CacheMusicSet_006bde50(isEnabled);
}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?EnableDangerMusic@CGameScriptInterface@@UBEX_N@Z); evidence: RTTI vtable slot + PDB name set.
