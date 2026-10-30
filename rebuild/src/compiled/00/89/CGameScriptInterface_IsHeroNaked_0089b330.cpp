// CGameScriptInterface::IsHeroNaked  @ 0x0089b330  (VC7.1 /O2 /Oy)
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
typedef unsigned char u8;
typedef unsigned int u32;

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> TCEntry;

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


template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    const TCEntry* LowerBound(const ETCInterfaceType& key) const;   // __fastcall: this in ecx, key pushed
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;

// engine callees (addresses reloc-masked)
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
class CPlayerManager { public: CPlayer& GetMainPlayer(); }; /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
extern "C" bool        __fastcall TCShop_GetName(void* self);

bool __fastcall CGameScriptInterface_IsHeroNaked(const CGameScriptInterface* self)
{
    HeroTarget* target = (&((CPlayerManager*)self->PlayerManager)->GetMainPlayer())->GetPControlledCreature();

    if (target != 0 &&
        (target->flags91 & 0x1) == 0 &&
        (target->field28 & 0x40000000) != 0)
    {
        char* base = (char*)target + 0x44;
        ETCInterfaceType key = (ETCInterfaceType)(0x5E);
        const TCEntry* e = ((TCMap*)base)->LowerBound(key);
        TCEntry* end = *(TCEntry**)(base + 4);
        if (e == end || e->first > 0x5E)
            e = end;
        return TCShop_GetName(((void*)e->second));
    }
    return false;
}