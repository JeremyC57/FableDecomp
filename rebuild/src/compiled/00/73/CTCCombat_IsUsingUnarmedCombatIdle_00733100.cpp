// CTCCombat::IsUsingUnarmedCombatIdle() @ 0x00733100 (retail): false while the owner's player is in mode 0x1b
// or 0x2f (CPlayer::HasMode @ 0x00634160 on CThing::GetPlayer() @ 0x004C7A10 -- the mode argument is pushed
// before the GetPlayer call, which takes none), or while the owner carries a weapon (IsThingCarryingWeapon
// @ 0x006A1540, __fastcall); true when the player's byte +0x217 is set; otherwise false while
// g_bAggressiveModeAvailable (0x013B85F3) is set, else whether the combat count (+0x1c) is positive.
// __fastcall, this in ecx.
// Engine headers for the callee classes (PDB data layouts, renamed out of the way): the local declarations
// below carry the retail identities the relocations bind.
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
extern unsigned char g_bAggressiveModeAvailable;   // 0x013B85F3

class CPlayer { public:
    bool HasMode(long mode);                     // 0x00634160
    unsigned char buf[0x300];
};

class CThing { public:
    CPlayer* GetPlayer();                        // 0x004C7A10
};
bool __fastcall IsThingCarryingWeapon(const CThing* thing);   // 0x006A1540

class CTCCombat { public:
    void*   pad0;
    CThing* obj;                                 // +0x04 owner
    char    pad2[0x14];
    int     count;                               // +0x1c
};

bool __fastcall CTCCombat_IsUsingUnarmedCombatIdle(CTCCombat* combat)
{
    if (combat->obj->GetPlayer()->HasMode(0x1b))
        return false;
    if (combat->obj->GetPlayer()->HasMode(0x2f))
        return false;
    if (IsThingCarryingWeapon(combat->obj))
        return false;
    if (combat->obj->GetPlayer()->buf[0x217])
        return true;
    if (g_bAggressiveModeAvailable)
        return false;
    return combat->count > 0;
}
