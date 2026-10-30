// CGameScriptInterface::EntitySetOpinionReactionMask(const CScriptThing& thing, long mask) const -- when the script thing's thing exists and has a CTCCreatureOpinionOfHero (interface 0x4F), pass the mask to its SetOpinionReactionMask
// Leaf: ?EntitySetOpinionReactionMask@CGameScriptInterface@@UBEXABVCScriptThing@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089c1f0, 77 bytes, thiscall, `ret 8`.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 612; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb vtable order) names it ?EntitySetOpinionReactionMask@CGameScriptInterface@@UBEXABVCScriptThing@@J@Z; its callee 0x00681CD0 is bound ?SetOpinionReactionMask@CTCCreatureOpinionOfHero@@QAEXJ@Z (Ego_r.pdb); CTCCreatureOpinionOfHero::GetInterfaceType() is 79 (0x4F, landed).
// Callees: CScriptThing virtual slot 11 (+0x2C, GetThing), CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >::
//   LowerBound(const ETCInterfaceType&) 0x0040F020, CTCCreatureOpinionOfHero::SetOpinionReactionMask(long) 0x00681CD0.
// Body: `CThing* t = thing.GetPThing(); if (t && t->GetInterface(&opinion, 0x4F)) opinion->SetOpinionReactionMask(mask);` (the inlined GetInterface: the component lands in eax, then ecx) -- the
//   presence test (bit 15 of bitset word 2) (`test ch,ch ; jns`), the lookup key homed in the dead thing-argument slot.
// Header debt: engine/CScriptThing.h, CThing and engine/CTCCreatureOpinionOfHero.h are flat layouts; modelled locally.
enum ETCInterfaceType { ETCInterfaceType_FIRST };
class CTCBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class TCompare> class CVectorMap {   // retail 0x10 bytes
public:
    typedef std::pair<K, V> value_type;
    value_type* First;                                     // +0x00 (CArray: STLport vector)
    value_type* Last;                                      // +0x04
    value_type* EndOfStorage;                              // +0x08
    TCompare    Compare;                                   // +0x0C
    bool        Dirty;                                     // +0x0D
    value_type* end() { return Last; }
    value_type* LowerBound(const K& key);                  // 0x0040F020 for the interface map
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    virtual ~CThing();
    unsigned char _pad04[0x1C];
    unsigned long PresentInterfaces[9];                    // +0x20 (the bitset of present interface types)
    CInterfaceMap Interfaces;                              // +0x44
    template <class T> __forceinline bool GetInterface(T** out, ETCInterfaceType type)
    {
        if (PresentInterfaces[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = type;
            CInterfaceMap* map = &Interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CCharString;
class CTCCreatureOpinionOfHero {
public:
    void SetOpinionReactionMask(long mask);                             // 0x00681CD0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntitySetOpinionReactionMask(const CScriptThing& thing, long mask) const
{
    CThing* t = thing.GetPThing();
    CTCCreatureOpinionOfHero* opinion;
    if (t && t->GetInterface(&opinion, (ETCInterfaceType)0x4F))
        opinion->SetOpinionReactionMask(mask);
}