// NGraphicAppearance::CTCGraphicAppearance::SetLightingChannel(const CEnginePrimitiveThemeChannelDesc& channel, bool includeCarried, bool includeWeapons) -- store the lighting channel, optionally pass it on to the graphic appearance of every carried thing (weapons only when asked), then re-render
// Leaf: SetLightingChannel
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
//
// Retail 0x004BFB40, 272 bytes incl. padding (`ret 0Ch`). Ego_r.pdb `?SetLightingChannel@CTCGraphicAppearance@
// NGraphicAppearance@@QAEXABVCEnginePrimitiveThemeChannelDesc@@_N1@Z`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined out-parameter GetInterface whose end/hit choice is ONE
// ternary inside the store -- that lands the component straight in ecx for the next call (an if-assign gives
// `mov eax,[eax+4] ... mov ecx,eax`, HasTC + a by-value GetTC moves the key out of the dead argument slot): CTCCarrying type 0x46 = bit 6
// of the bitset word +0x28, and the carried thing's CTCGraphicAppearance type 0x5B = bit 27; the weapon test is type 0x48 =
// bit 8, `test dh,1`), CTCCarrying::GetAllCarriedThings(vector<CThing*>&) const @ 0x006A2220 (manifest "RemapGroups" is
// propagated), itself recursively, free @ 0x00BFEA14 (the vector buffer), CThing::AddToRenderList @ 0x004C7AC0.
// Layout: retail CTCGraphicAppearance (landed GetAllMeshes model): ParentThing +0x04, LightingChannel +0x40,
// UpdatePrimitiveReasons +0x58; retail CThing bitset +0x20, interface map +0x44.
// Shape: the channel is a one-dword record copied first; the carried-thing vector is an STLport vector declared after the
// carrying lookup (zeroed through edi), walked by index with `*(begin() + i)` against `size()`; the recursion re-reads the
// includeCarried argument from its slot; `this` is spilled to the frame because ebx carries includeWeapons in the loop.
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14

class CTCBase;

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "stlport/fable_containers.h"                // std::vector<CThing*> (STLport 4.6, 12 bytes)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;


class CTCCarrying { public:
    void GetAllCarriedThings(std::vector<CThing*>& things) const;   // 0x006A2220
};

class CEnginePrimitiveThemeChannelDesc { public:                   // one dword
    unsigned long Channel;
};

namespace NGraphicAppearance {
class CTCGraphicAppearance;
}

namespace NGraphicAppearance {

class CTCGraphicAppearance { public:
    void*          __vftable;                               // +0x00 (CTCBase)
    CThing*        ParentThing;                             // +0x04
    unsigned char  _pad08[0x40 - 0x08];
    CEnginePrimitiveThemeChannelDesc LightingChannel;       // +0x40
    unsigned char  _pad44[0x58 - 0x44];
    unsigned char  UpdatePrimitiveReasons;                  // +0x58
    void SetLightingChannel(const CEnginePrimitiveThemeChannelDesc& channel, bool includeCarried, bool includeWeapons);
};

void CTCGraphicAppearance::SetLightingChannel(const CEnginePrimitiveThemeChannelDesc& channel, bool includeCarried,
                                              bool includeWeapons)
{
    LightingChannel = channel;
    CTCCarrying* carrying;
    if (includeCarried && ParentThing->GetTC(TCI_CARRYING, &carrying)) {
        std::vector<CThing*> carried;
        carrying->GetAllCarriedThings(carried);
        for (unsigned int i = 0; i < carried.size(); ++i) {
            CThing* thing = carried[i];
            CTCGraphicAppearance* appearance;
            if ((includeWeapons || !thing->HasTC(TCI_WEAPON)) && thing->GetTC(TCI_GRAPHIC_APPEARANCE_NEW, &appearance))
                appearance->SetLightingChannel(channel, includeCarried, includeWeapons);
        }
    }
    ParentThing->AddToRenderList();
    UpdatePrimitiveReasons |= 4;
}

}