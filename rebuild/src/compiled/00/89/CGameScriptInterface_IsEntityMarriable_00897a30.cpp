// CGameScriptInterface::IsEntityMarriable(const CScriptThing& thing) const -- a live creature whose wife component (interface 0xd7) answers IsWillingToMarry()
// Leaf: ?IsEntityMarriable@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x00897A30, 101 bytes, `ret 4`; Ego_r.pdb ?IsEntityMarriable@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z.
// Body: t = thing.GetPThing() (slot 11, +0x2c); if t, not dead (bit 0 of +0x91), a creature (type byte +0x10 == 1) and
// holding interface 0xd7 (bit 23 of UsingComponents word 6, +0x38), the component from the const Components map
// (LowerBound 0x004365B0, key in the dead `thing` slot) answers IsWillingToMarry() (0x006CFEC0, a chain of state tests; that name is a model, not confirmed); else false.
// The byte twin of 0x008979C0 IsEntityMarriedToHero (only the callee differs).
// Layout: CThing as the matched 0x0089A330 / 0x0089B0B0 rows. Interface 0xd7 is modelled as CTCWife (Ego_r.pdb
// ?IsMarriedToThePlayer@CTCWife@@QBE_NXZ / ?IsWillingToMarry@CTCWife@@QBE_NXZ); the interface number is not confirmed.
// Shape: /O2 (16-aligned entry); the PeekTC out-pointer form.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char Type;                                  // +0x10 (1 = a creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad54[0x3d];
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
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || T::INTERFACE_TYPE < it->first)
            it = map->end();
        *out = (T*)it->second;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || T::INTERFACE_TYPE < it->first)
            it = map->end();
        *out = (const T*)it->second;
        return true;
    }
};
class CTCWife {                                         // interface 0xd7
public:
    enum { INTERFACE_TYPE = 0xd7 };
    bool IsMarriedToThePlayer() const;                   // 0x006CEC80
    bool IsWillingToMarry() const;                       // 0x006CFEC0
};
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool CGameScriptInterface::IsEntityMarriable(const CScriptThing& thing) const
{
    const CThing* t = thing.GetPThing();
    if (t && !t->IsDead() && t->Type == 1)
    {
        const CTCWife* wife;
        if (t->PeekTC(&wife) && wife->IsWillingToMarry())
            return true;
    }
    return false;
}