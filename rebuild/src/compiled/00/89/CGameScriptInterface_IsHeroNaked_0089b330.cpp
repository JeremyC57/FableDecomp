// CGameScriptInterface::IsHeroNaked  @ 0x0089b330  (VC7.1 /O2 /Oy)
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
typedef unsigned char u8;
typedef unsigned int u32;

struct TCEntry {
    int   key;        // +0x00
    void* iface;      // +0x04
};

struct HeroTarget {
    char  pad00[0x28];
    u32   field28;    // +0x28
    char  pad2C[0x18];
    char  mapStore[4];// +0x44 (vector-map object)
    TCEntry* mapEnd;  // +0x48
    char  pad4C[0x45];
    u8    flags91;    // +0x91
};
class CThingPlayerCreature : public HeroTarget { };


struct TCMap {
    TCEntry* LowerBound(int* key);   // __fastcall: this in ecx, key pushed
};

// engine callees (addresses reloc-masked)
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
extern "C" CPlayer* __fastcall GSI_Helper_A(void* p);               // 0x00449970
extern "C" bool        __fastcall TCShop_GetName(void* self);

bool __fastcall CGameScriptInterface_IsHeroNaked(const CGameScriptInterface* self)
{
    HeroTarget* target = GSI_Helper_A(self->PlayerManager)->GetPControlledCreature();

    if (target != 0 &&
        (target->flags91 & 0x1) == 0 &&
        (target->field28 & 0x40000000) != 0)
    {
        char* base = (char*)target + 0x44;
        int key = 0x5E;
        TCEntry* e = ((TCMap*)base)->LowerBound(&key);
        TCEntry* end = *(TCEntry**)(base + 4);
        if (e == end || e->key > 0x5E)
            e = end;
        return TCShop_GetName(e->iface);
    }
    return false;
}