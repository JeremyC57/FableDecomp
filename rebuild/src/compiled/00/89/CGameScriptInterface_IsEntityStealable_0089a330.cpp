// CGameScriptInterface::IsEntityStealable(const CScriptThing& thing) const -- a valid thing whose interface-0x54 component has its +0xd flag set
// Leaf: ?IsEntityStealable@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x0089A330, 94 bytes, `ret 4`; Ego_r.pdb ?IsEntityStealable@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z.
// Body: if thing.IsAlive() (slot 75, +0x12c): t = thing.GetPThing() (slot 11, +0x2c); if t holds interface 0x54 (bit 20 of
// UsingComponents word 2, +0x28) the component from the const Components map (LowerBound 0x004365B0, key in the dead
// `thing` slot) answers its +0xd byte; otherwise false.
// Layout: CThing as the matched 0x0089B0B0 / 0x00897AA0 rows. The interface-0x54 component class and the +0xd member name
// are not confirmed.
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
class CTCInterface54 {                                  // interface 0x54 (class not confirmed)
public:
    enum { INTERFACE_TYPE = 0x54 };
    unsigned char _pad00[0xd];
    bool          Stealable;                             // +0x0d (name not confirmed)
};
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool CGameScriptInterface::IsEntityStealable(const CScriptThing& thing) const
{
    if (thing.IsAlive())
    {
        const CThing* t = thing.GetPThing();
        const CTCInterface54* tc;
        if (t->PeekTC(&tc) && tc->Stealable)
            return true;
    }
    return false;
}