// CGameScriptInterface::EntityFadeIn(const CScriptThing& thing, float seconds) const -- start a fade-in on a valid thing with a CTCFadeOutAndIn (interface 0xF9)
// Leaf: ?EntityFadeIn@CGameScriptInterface@@UBEXABVCScriptThing@@M@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A07C0, thiscall, virtual.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 535; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb
//   vtable order) names it ?EntityFadeIn@CGameScriptInterface@@UBEXABVCScriptThing@@M@Z.
// Guard: the script thing is valid (slot 75); the component is CTCFadeOutAndIn (interface 0xF9: GetInterfaceType of its RTTI vtable, or its Ego_r.pdb method).
// Callees: CScriptThing slots 75/11, CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >::LowerBound(const ETCInterfaceType&) 0x0040F020 (the inlined CThing::GetInterface), CTCFadeOutAndIn::FadeIn(float, bool) 0x006C3BD0 (Ego_r.pdb, landed).
// Shape: the landed L083/L084 GetInterface model (2026-10-27_linux_hand955); the lookup key is homed in the dead thing-argument slot.
// Layout: local models -- engine/CGameScriptInterface.h carries its vptr as a data member and the CThing/CScriptThing/TC headers are flat
//   Ego_r layouts (Dinkumware offsets); component offsets are retail (Ego_r minus 4 per STLport container before the member).
enum ETCInterfaceType { ETCInterfaceType_FIRST };
class CTCBase;
#include "engine/CVectorMap.h"
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
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CTCFadeOutAndIn {
public:
    void FadeIn(float seconds, bool immediately);          // 0x006C3BD0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntityFadeIn(const CScriptThing& thing, float seconds) const
{
    if (thing.IsAlive()) {
        CTCFadeOutAndIn* fader;
        if (thing.GetPThing()->GetInterface(&fader, (ETCInterfaceType)0xF9))
            fader->FadeIn(seconds, false);
    }
}