// Leaf: ?CacheMusicSet@CGameScriptInterface@@UBEXW4EMusicSetType@@@Z
#define FABLE_RETAIL_NSCRIPT   // alias route: engine/CGameScriptInterface.h declares CacheMusicSet under this class-model switch
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
enum EMusicSetType { EMS_A };

struct CMusicTarget
{
    void CacheMusicSet(EMusicSetType t);
};

class CWorld
{ public:
    char pad_0000[0x6C];
    CMusicTarget* target_006C;
};


void CGameScriptInterface::CacheMusicSet(EMusicSetType t) const
{
    World->target_006C->CacheMusicSet(t);
}