// CThing::GetTC<const CTCCoopSpirit>(const CTCCoopSpirit** out) -- fetch the thing's co-op spirit component (interface 0xC7) when the thing uses one
// Leaf: ??$GetTC@$$CBVCTCCoopSpirit@@@CThing@@QAE_NPAPBVCTCCoopSpirit@@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's CArray member Data is the header's CArray base (Ego_r: CVectorMap derives CArray<pair<K,V> >): Data.end() / begin() / size() read through the base; the local std::bitset operator[] / reference is the STLport model of stlport/fable_containers.h under FABLE_RETAIL_STLP_BITSET_INDEX (retail tests the word inline, e.g. 0x0062D032 test dword ptr [ebx+0x24],0x10000).
// Retail: Fable.exe @ 0x0044BFA0, 71 bytes, __fastcall `ret 4`, bool in al.
// Identity (probable): CThing::GetTC<T>(T**) member template (PDB name sets: 267 instantiations). The interface number
// 0xC7 is CTCCoopSpirit's (its GetInterfaceType, vtable 0x0125B264 slot 21 = 0x00670B50 `mov eax,0C7h`). The PDB holds
// both ??$GetTC@VCTCCoopSpirit@@@CThing@@QAE_NPAPAVCTCCoopSpirit@@@Z and the const-T twin
// ??$GetTC@$$CBVCTCCoopSpirit@@@CThing@@QAE_NPAPBVCTCCoopSpirit@@@Z; this body calls the CONST
// CVectorMap::LowerBound 0x004365B0 (the non-const GetTC<CTCRegionDisplay> 0x004ACAB0 calls the non-const 0x0040F020),
// so it is taken as the const-T instantiation. No caller in retail (gap-scan COMDAT).
// Callee: CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> >::LowerBound(const ETCInterfaceType&) const
// @ 0x004365B0 (bound ?LowerBound@...@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z).
// Shape: UsingComponents[0xC7] through the STLport bitset reference proxy (bit 7 of word 6 = the sign of byte +0x38:
// `mov al,[ecx+38h] ; test al,al ; jns`), the key copy in the pushed ecx slot, a miss (end or greater key) yields the
// end entry, whose value is handed out; true. The const map view is a model device for the const overload the retail
// instantiation reaches (how Lionhead's template selects it is not recoverable from the bytes).
// Layout: retail CThing UsingComponents (bitset<274>) +0x20, Components (CVectorMap, 0x10) +0x44 (the landed
// CPlayer::InitModeNavigateInventoryExperienceScreen 0x00635080 model; no CThing engine header exists).
#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "engine/CVectorMap.h"


enum ETCInterfaceType { ETCInterfaceType_Force32 = 0x7fffffff };   // enumerators not needed
class CTCBase;

class CTCCoopSpirit {
public:
    enum { INTERFACE_TYPE = 0xC7 };                         // GetInterfaceType 0x00670B50
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
            const TCMap* map = &Components;
            const TCMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || T::INTERFACE_TYPE < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

template bool CThing::GetTC<const CTCCoopSpirit>(const CTCCoopSpirit** out);