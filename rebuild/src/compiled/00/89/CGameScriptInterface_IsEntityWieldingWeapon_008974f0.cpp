// CGameScriptInterface::IsEntityWieldingWeapon(const CScriptThing& thing) const -- a live thing whose carrying component (interface 0x46) has a primary-slot thing holding interface 0x48 (the weapon role)
// Leaf: ?IsEntityWieldingWeapon@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Retail: Fable.exe @ 0x008974F0, 105 bytes, `ret 4`; Ego_r.pdb same symbol.
// Body: t = thing.GetPThing() (slot 11); false unless t, not dead (bit 0 of +0x91) and holding interface 0x46 (bit 6 of
// UsingComponents word 2, +0x28). carrying = PeekTC (const LowerBound 0x004365B0, key in the dead `thing` slot);
// w = carrying->GetThingInPrimarySlot() (0x006A1440, ?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ, engine_api.tsv
// + bsim 1.0); result = w && w holds interface 0x48 (bit 8 of word 2) -- the && is int-typed (`mov eax,1` / `xor eax,eax`)
// while the early-out returns are `xor al,al`.
// Layout: CThing as the matched 0x0089A330 / 0x0089B0B0 rows. Interface 0x46 = CTCCarrying (from the callee's class);
// interface 0x48 as a weapon role is not confirmed.
// Shape: /O2 (16-aligned entry); the PeekTC out-pointer form. The early-return form (`if (!t || dead) return false;
// if (!PeekTC) return false;`) keeps the component in eax and moves `pop esi` before the call; nesting the positive branch matches.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry*       LowerBound(const ETCInterfaceType& type);          // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;    // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char Type;                                  // +0x10 (1 = a creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)               // ??$GetTC@V...@@@CThing@@QAE_NPAPAV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || T::INTERFACE_TYPE < it->first)
            it = map->last;
        *out = (T*)it->second;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || T::INTERFACE_TYPE < it->first)
            it = map->last;
        *out = (const T*)it->second;
        return true;
    }
};
class CTCCarrying {                                     // interface 0x46
public:
    enum { INTERFACE_TYPE = 0x46 };
    CThing* GetThingInPrimarySlot() const;               // 0x006A1440
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool CGameScriptInterface::IsEntityWieldingWeapon(const CScriptThing& thing) const
{
    const CThing* t = thing.GetPThing();
    if (t && !t->IsDead())
    {
        const CTCCarrying* carrying;
        if (t->PeekTC(&carrying))
        {
            const CThing* w = carrying->GetThingInPrimarySlot();
            return w && w->HasTC(0x48);
        }
    }
    return false;
}