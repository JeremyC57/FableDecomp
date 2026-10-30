// CGameScriptInterface::EnableGuards(const CScriptThing& village, bool enable) const -- script call: forwards to CTCVillage::EnableGuards on a live village thing
// Leaf: ?EnableGuards@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Retail: Fable.exe @ 0x00896270, 85 bytes, thiscall `ret 8`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 443 holds this body; slot 443 is
//   ?EnableGuards@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z (ghidra_out/cgsi_slots_aligned.tsv). Callee 0x0065C45A
//   is ?EnableGuards@CTCVillage@@QAEX_N@Z (both PDBs; the manifest labels it CTCVillage::EnableGuards).
// Body: t = thing.GetPThing() (CScriptThing slot 11, +0x2c, no validity check); if t, not dead (+0x91 bit 0) and holding
//   interface 0x22 (UsingComponents word 1 bit 2, `test byte [eax+24h],4`): GetTC through the non-const LowerBound
//   0x0040F020 (key in the dead `thing` argument slot), then the CTCVillage call. `this` is unused.
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
    unsigned char _pad00[0x20];
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
    template <class T> __forceinline bool GetTC(T** out)
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
    template <class T> __forceinline bool PeekTC(const T** out) const
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
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CCharString;
class CTCVillage {                                      // interface 0x22 (CTCVillage::GetInterfaceType not landed; the PDB names of the callee pin the class)
public:
    enum { INTERFACE_TYPE = 0x22 };
    unsigned char _pad00[0xd4];
    long HeroEnemyOfGuardsUntil;                         // +0xd4 (descriptive: the frame IncreaseHeroEnemyOfGuardsUntil extends)
    void SetVillageLimbo(bool limbo);                    // 0x0065C36E
    void EnableGuards(bool enable);                      // 0x0065C45A
    void EnableVillagerDefTypes(bool enable, const CCharString& defType);   // 0x0065B142
    void ClearHeroEnemyOfGuards() { HeroEnemyOfGuardsUntil = 0; }
};
class CGameScriptInterface {
public:
    virtual void EnableGuards(const CScriptThing& village, bool enable) const;
};
void CGameScriptInterface::EnableGuards(const CScriptThing& village, bool enable) const
{
    CThing* t = village.GetPThing();
    CTCVillage* tc;
    if (t && !t->IsDead() && t->GetTC(&tc))
        tc->EnableGuards(enable);
}