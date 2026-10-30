// CGameScriptInterface::IsDiggingSpotEnabled(const CScriptThing& thing) const -- whether a valid thing's CTCDiggingSpot (interface 0xEF) is enabled (false without one)
// Leaf: ?IsDiggingSpotEnabled@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089F1E0, thiscall, virtual.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 576; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb
//   vtable order) names it ?IsDiggingSpotEnabled@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z.
// Guard: the script thing is valid (slot 75); the component is CTCDiggingSpot (interface 0xEF: GetInterfaceType of its RTTI vtable, or its Ego_r.pdb method).
// Callees: CScriptThing slots 75/11, CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >::LowerBound(const ETCInterfaceType&) 0x0040F020 (the inlined CThing::GetInterface), none (an inline load).
// Shape: the landed L083/L084 GetInterface model (2026-10-27_linux_hand955); the lookup key is homed in the dead thing-argument slot.
// Layout: local models -- engine/CGameScriptInterface.h carries its vptr as a data member and the CThing/CScriptThing/TC headers are flat
//   Ego_r layouts (Dinkumware offsets); component offsets are retail (Ego_r minus 4 per STLport container before the member).
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
    unsigned char _pad04[0xC];
    unsigned char ThingType;                               // +0x10 (1 = creature; Ego_r +0x10 is unnamed)
    unsigned char _pad11[0xF];
    unsigned long UsingComponents[9];                      // +0x20 (bitset<274> of present interface types)
    CInterfaceMap Components;                              // +0x44
    unsigned char _pad54[0x91 - 0x54];
    unsigned char Dead : 1;                                // +0x91 bit 0 (Ego_r +0xA1)
    template <class T> __forceinline bool GetInterface(T** out, ETCInterfaceType type)
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = type;
            CInterfaceMap* map = &Components;
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
class CTCDiggingSpot {
public:
    unsigned char _pad00[0xC];
    bool          Enabled;                                // +0x0C (Ego_r)
    bool          Hidden;                                 // +0x0D
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool CGameScriptInterface::IsDiggingSpotEnabled(const CScriptThing& thing) const
{
    if (thing.IsAlive()) {
        CTCDiggingSpot* spot;
        if (thing.GetPThing()->GetInterface(&spot, (ETCInterfaceType)0xEF))
            return spot->Enabled;
    }
    return false;
}