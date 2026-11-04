// CAnimationSet::GetNoAnims(const CCharString& name, const CAnimFilterBase& filter) const -- how many of the animations stored under the name's CRC the filter accepts
// Leaf: ?GetNoAnims@CAnimationSet@@QBEJABVCCharString@@ABVCAnimFilterBase@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe @ 0x005DC290, 72 bytes (+8 int3), __fastcall `ret 8`. Ego_r.pdb
// `?GetNoAnims@CAnimationSet@@QBEJABVCCharString@@ABVCAnimFilterBase@@@Z`. Callers: CThingCreatureBase::PreloadAnimation
// 0x006631C0, CThingCreatureBase::PreloadBasicAnimations 0x00663610 (twice).
// Callees: CAnimationSet::FindFirstAnimWithName(const CCharString&) const @ 0x005DC020, the filter's virtual
// operator()(const CAnimationEntry&) const (slot 0; Ego_r `??RCAnimFilterOfGroup@@UBE_NABVCAnimationEntry@@@Z` etc.).
// Shape: speed TU; the equal-key run from the first match: the loop is entered under the end() test, the key test
// (against the first match's key, re-read after each virtual call) heads the body, end() is re-read at the bottom.
// Label: the catalog's AnimationDef_CountMatchingPreloads is this method.
class CCharString;
class CAnimationEntry {                                 // 16 bytes (Ego_r): BankIndex, AnimName, GroupName, Components
public:
    long  BankIndex;
    void* AnimName;
    void* GroupName;
    void* Components;
};
class CAnimFilterBase {
public:
    virtual bool operator()(const CAnimationEntry& entry) const = 0;   // slot 0
};
#include "engine/CVectorMap.h"
class CAnimationSet {
public:
    typedef CVectorMap<unsigned long, CAnimationEntry, CKeyPairCompareLess<unsigned long, CAnimationEntry> > TAnimMap;
    TAnimMap Anims;                                     // +0x00
    TAnimMap::const_iterator FindFirstAnimWithName(const CCharString& name) const;   // 0x005DC020
    long GetNoAnims(const CCharString& name, const CAnimFilterBase& filter) const;
};
long CAnimationSet::GetNoAnims(const CCharString& name, const CAnimFilterBase& filter) const
{
    long count = 0;
    TAnimMap::const_iterator first = FindFirstAnimWithName(name);
    for (TAnimMap::const_iterator it = first; it != Anims.end() && it->first == first->first; ++it)
        if (filter(it->second))
            ++count;
    return count;
}