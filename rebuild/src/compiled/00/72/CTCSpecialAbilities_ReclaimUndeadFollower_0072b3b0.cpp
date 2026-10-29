// CTCSpecialAbilities::ReclaimUndeadFollower(CThingGameObject& thing) -- take a raised undead back into the follower list: store its rounded health and (when it has a random-appearance morph) its appearance seed in the follower record, mark the record inactive and kill the thing
// Leaf: ReclaimUndeadFollower
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072B3B0, 99 bytes (112 with padding), `ret 4`.
// Label correction: catalog "CCreatureAction_KillReclaimedUndead" -- `this` goes straight on to
//   CTCSpecialAbilities::GetUndeadFollower(WithThing) (landed 0x0072A910), and Ego_r.pdb names
//   ?ReclaimUndeadFollower@CTCSpecialAbilities@@QAEXAAVCThingGameObject@@@Z (void, one reference argument).
// Callees: CTCSpecialAbilities::GetUndeadFollower 0x0072A910, _ftol2 0x00BFEA70,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (interface 3 = CTCRandomAppearanceMorph,
//   bit 3 of the presence word +0x20: `test byte ptr [edi+20h],8`), CThing::Kill(bool) 0x004C9B80.
// Layout: CUndeadFollower (Ego_r.pdb, 20 bytes): Health +0x00, PThing +0x04, Active +0x0c, AppearanceSeed +0x10;
//   CTCRandomAppearanceMorph::Seed +0x18; the game object's float at +0x0b4 (retail; PDB pads it) is its health.
// Shape: the record pointer is kept in ebx, the thing in edi; the lookup key reuses the dead argument slot.  The
//   interface fetch is the out-pointer GetInterface (as in the landed SetupBerserk 0x0072CBA0: the ternary on the
//   iterator stored through the out pointer) -- a pointer-returning GetTC gives `mov ecx,[eax+4] ; mov edx,[ecx+18h]`
//   where retail reuses eax (`mov eax,[eax+4] ; mov ecx,[eax+18h]`).
#include "engine/CTCBase.h"

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template <class T> __forceinline bool GetInterface(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (const T*)(it == map->last || interfaceType < it->first ? map->last : it)->second;
            return true;
        }
        return false;
    }
    void Kill(bool silent);                     // 0x004C9B80
};
class CThingGameObject : public CThing { public:
    unsigned char _pad4c[0xb4 - 0x4c];
    float Health;                               // +0xb4 (retail)
};
class CTCRandomAppearanceMorph { public:
    unsigned char _pad00[0x18];
    unsigned long Seed;                         // +0x18
};
struct CBaseIntelligentPointer {
    void* m_pItem;
    long  m_handle;
};
struct CUndeadFollower {                        // CTCSpecialAbilities::CUndeadFollower, 20 bytes
    long                    Health;             // +0x00
    CBaseIntelligentPointer PThing;             // +0x04
    bool                    Active;             // +0x0c
    unsigned long           AppearanceSeed;     // +0x10
};
class CTCSpecialAbilities : public CTCBase { public:
    CUndeadFollower* GetUndeadFollower(const CThing* thing);   // 0x0072A910
    void ReclaimUndeadFollower(CThingGameObject& thing);
};

void CTCSpecialAbilities::ReclaimUndeadFollower(CThingGameObject& thing)
{
    CUndeadFollower* follower = GetUndeadFollower(&thing);
    follower->Health = (long)thing.Health;
    const CTCRandomAppearanceMorph* morph;
    if (thing.GetInterface(&morph, 3))
        follower->AppearanceSeed = morph->Seed;
    follower->Active = false;
    thing.Kill(false);
}