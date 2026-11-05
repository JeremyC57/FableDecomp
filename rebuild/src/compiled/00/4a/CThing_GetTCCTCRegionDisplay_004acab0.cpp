// CThing::GetTC<CTCRegionDisplay>(CTCRegionDisplay** out) -- fetch the thing's region-display component (interface 0x8E) when the thing uses one
// Leaf: ??$GetTC@VCTCRegionDisplay@@@CThing@@QAE_NPAPAVCTCRegionDisplay@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe @ 0x004ACAB0, 72 bytes, __fastcall `ret 4`, bool in al.
// Identity (proven by interface number + PDB name set): CThing::GetTC<T>(T**); 0x8E is CTCRegionDisplay's interface
// number (GetInterfaceType, vtable 0x0125A0EC slot 21 = 0x0063A1F0 `mov eax,8Eh`), and the PDB name sets hold the only
// instantiation ??$GetTC@VCTCRegionDisplay@@@CThing@@QAE_NPAPAVCTCRegionDisplay@@@Z. No caller in retail (gap-scan COMDAT).
// Callee: CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> >::LowerBound(const ETCInterfaceType&) @
// 0x0040F020 (bound ?LowerBound@...@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z).
// Shape: UsingComponents[0x8E] through the STLport bitset reference proxy (bit 14 of word 4 = `mov eax,[ecx+30h] ;
// test ah,40h`), the key copy in the pushed ecx slot, a miss (end or greater key) yields the end entry, whose value is
// handed out; true.
// Layout: retail CThing UsingComponents (bitset<274>) +0x20, Components (CVectorMap, 0x10) +0x44 (the landed
// CPlayer::InitModeNavigateInventoryExperienceScreen 0x00635080 model; no CThing engine header exists).
#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "engine/CVectorMap.h"


enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };   // enumerators not needed
class CTCBase;

class CTCRegionDisplay {
public:
    enum { INTERFACE_TYPE = 0x8E };                         // GetInterfaceType 0x0063A1F0
};

class CThing {                                              // partial view, retail offsets
public:
    typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;
    unsigned char      Header[0x20];                        // CClass / CBaseObject / CBaseClass part, UniqueID (+0x18)
    std::bitset<274>   UsingComponents;                     // +0x20
    TCMap              Components;                          // +0x44
    template <class T> bool GetTC(T** out)
    {
        if (UsingComponents[T::INTERFACE_TYPE]) {
            ETCInterfaceType type = (ETCInterfaceType)T::INTERFACE_TYPE;
            TCMap* map = &Components;
            TCMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || T::INTERFACE_TYPE < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

template bool CThing::GetTC<CTCRegionDisplay>(CTCRegionDisplay** out);