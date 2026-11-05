// CThingFilter_IsOccupiableOfType::operator()(const CThing& thing) const -- whether the thing is a single-person occupiable (kind 0xac) whose definition carries occupiable type flag 4
// Leaf: ??RCThingFilter_IsOccupiableOfType
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members m_pFirst/m_pLast are the header CArray base's begin() / end() (STLport vector start +0 / finish +4).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A89E0, length 80 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (const instantiation),
// CTCSinglePersonOccupiable::IsOfType(unsigned long) @ 0x0087C420 (tests the def's type mask at +0x28).
// Identity: the catalog's "CThingCreatureBase::IsAbleToRecoverFromStunned" is a propagated label (Ego_r.pdb's is the
// argument-less 0x00664C60); callers set ecx to a filter object (`ret 4` __fastcall, `this` unused). The class name is
// descriptive (real name unknown).
// Layout (retail, local model): the thing's component-presence bitset at +0x20 (kind k = word k>>5, bit k&31) and the
// sorted {kind, component} vector map at +0x44 {first, last} (no engine header names them).
// Shape: the out-parameter GetInterface template on the const thing (the component comes back in eax: `mov eax,[eax+4]
// ; mov ecx,eax`), key homed in a frame/argument slot and passed by address to the const LowerBound.
// `if (GetInterface(..) && occupiable->IsOfType(4)) return true; return false;` (two exits, the false tail shared);
// kind 0xac = `mov ecx,[eax+34h] ; test ch,10h`.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry;

typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;

class CTCSinglePersonOccupiable { public:              // component kind 0xac
    bool IsOfType(unsigned long typeMask) const;    // 0x0087C420
};

struct CThingFilter_IsOccupiableOfType {
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsOccupiableOfType::operator()(const CThing& thing) const
{
    const CTCSinglePersonOccupiable* occupiable;
    if (thing.PeekTC(TCI_SINGLE_PERSON_OCCUPIABLE, &occupiable) && occupiable->IsOfType(4))
        return true;
    return false;
}