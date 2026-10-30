#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

struct CRumbleTarget;
struct CRumbleTargetVTable {
    void* Slots[0x12];
    void (__fastcall *ClearRumbles)(CRumbleTarget*);
};
struct CRumbleTarget {
    CRumbleTargetVTable* __vftable;
    unsigned char _pad_0x04[0x91 - 4];
    unsigned char StateFlags;
};
class CThingPlayerCreature : public CRumbleTarget { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0

struct CRumbleTableEntry {
    CRumbleTarget* Target;
    void* Other;
};
struct CRumbleManager {
    unsigned char _pad_0x00[0x2c];
    CRumbleTableEntry* Table;
};

extern CRumbleManager* g_manager;
class CPlayerManager { public: CPlayer& GetMainPlayer(); }; /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
extern int __fastcall GetIndex(CRumbleTarget* target);

void __fastcall CGameScriptInterface_ClearAllRumbles(CGameScriptInterface* self)
{
    CPlayer* value = (&((CPlayerManager*)self->PlayerManager)->GetMainPlayer());
    CRumbleTarget* target = value->GetPControlledCreature();
    if (target == 0)
        return;
    if (target->StateFlags & 1)
        return;
    int index = GetIndex(target);
    CRumbleTarget* entryTarget = g_manager->Table[index].Target;
    entryTarget->__vftable->ClearRumbles(entryTarget);
}
