// CTCInventoryMap::GetRegionTeleporter(const CRegion& region) const -- the first active teleporter thing found in any of the region's maps, or 0
// Grade: DIFFER(249v248) -- PDB leaf (private const, const CRegion&) on retail CThing/CTCBase (GetTC<CTCTeleporter>) + the landed same-TU FindTeleports 0x005FC620 world model (PeekWorldMap pointer, inline world forwarding) + a same-TU GetPFirstThing stand-in: function-scope filters bound once give retail length and no aligned frame (filter temporaries in the loop force the and esp,-8 frame); left: the Bind2 address stores sit before the size test (retail after), the 0xDD constant is not shared through esi, the lookup key takes the world-map slot (retail the found slot) and the region lives in esi (retail ebx); 12 grades
// Leaf: ?GetRegionTeleporter@CTCInventoryMap@@ABEPAVCThing@@ABVCRegion@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FBEA0, 246 bytes (+ int3), `ret 4`, esp frame.
// Callees: CTCBase::PeekWorld() const @ 0x00686A80 (once for the world map, once per map for the search tools), the world's
//   const virtual slot 13 (+0x34, PeekWorldMap), CThingSearchTools::GetPFirstThing<CFilter_Bind2<CThingFilter_HasTC,
//   CThingFilter_InMap>, CThing> @ 0x00615750 (landed), the const component-map lower bound @ 0x004365B0 (the inline retail
//   CThing::GetTC<CTCTeleporter>, TCI_TELEPORTER 221).
// Layout: Ego_r.pdb CRegion::Maps vector<long> +0; CTCTeleporter::Active +0x10; the world's search tools +0x20; retail CThing.
// Shape: per map the HasTC / InMap filters and their Bind2 are argument temporaries (VC7.1 hoists the two loop-invariant
//   address stores); the found thing starts at 0 per map. A read-only same-TU GetPFirstThing stand-in (the found slot does
//   not escape) lets VC7.1 reuse the found slot for the component lookup key. The teleporter component pointer is an
//   uninitialised local GetTC writes only when the bit is set.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
class CThingFilter_HasTC { public:
    ETCInterfaceType Type;
    CThingFilter_HasTC() {}
    CThingFilter_HasTC(ETCInterfaceType type) : Type(type) {}
};
class CThingFilter_InMap { public:
    const CWorldMap* WorldMap;
    long MapNumber;
    CThingFilter_InMap() {}
    CThingFilter_InMap(const CWorldMap& worldMap, long mapNumber) : WorldMap(&worldMap), MapNumber(mapNumber) {}
};
template <class A, class B> class CFilter_Bind2 { public:     // Ego_r.pdb: both filters held by pointer
    const A* F1;
    const B* F2;
    CFilter_Bind2(const A& f1, const B& f2) : F1(&f1), F2(&f2) {}
};
inline CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap> operator&&(const CThingFilter_HasTC& a, const CThingFilter_InMap& b)
{
    return CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap>(a, b);
}
class CThingSearchTools { public:
    template <class TFilter, class TThing> bool GetPFirstThing(const TFilter& filter, TThing** out) const;   // 0x00615750
};
void* FindFirstThing_00615750(const CThingSearchTools* tools, const void* filter);
#pragma auto_inline(off)
template <class TFilter, class TThing>
__declspec(noinline) bool CThingSearchTools::GetPFirstThing(const TFilter& filter, TThing** out) const
{
    TThing* thing = (TThing*)FindFirstThing_00615750(this, &filter);
    if (thing) {
        *out = thing;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual const CWorldMap* PeekWorldMap() const;  // slot 13 (+0x34) (FableWin.pdb: const)
    unsigned char _pad04[0x20 - 0x04];
    CThingSearchTools* PThingSearchTools;           // +0x20 (scoped_ptr)
    template <class TFilter, class TThing> bool GetPFirstThing(const TFilter& filter, TThing** out) const { return PThingSearchTools->GetPFirstThing(filter, out); }
};
class CRegion { public:
    std::vector<long> Maps;                         // +0x00
};
class CTCTeleporter : public CTCBase { public:      // TCI_TELEPORTER
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_TELEPORTER; }
    unsigned char _pad[0x10 - sizeof(CTCBase)];
    bool Active;                                    // +0x10
};
class CTCInventoryMap : public CTCBase {
    CThing* GetRegionTeleporter(const CRegion& region) const;
};

CThing* CTCInventoryMap::GetRegionTeleporter(const CRegion& region) const
{
    const CWorldMap* worldMap = PeekWorld().PeekWorldMap();
    CThingFilter_HasTC hasTC;
    CThingFilter_InMap inMap;
    CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap> filter(hasTC, inMap);
    for (unsigned int i = 0; i < region.Maps.size(); ++i) {
        CThing* found = 0;
        inMap.WorldMap = worldMap;
        inMap.MapNumber = region.Maps[i];
        hasTC.Type = TCI_TELEPORTER;
        if (PeekWorld().GetPFirstThing(filter, &found)) {
            CTCTeleporter* teleporter;
            found->GetTC(&teleporter);
            if (teleporter && teleporter->Active)
                return found;
        }
    }
    return 0;
}
