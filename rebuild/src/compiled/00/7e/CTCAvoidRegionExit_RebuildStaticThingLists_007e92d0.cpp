// CTCAvoidRegionExit::RebuildStaticThingLists() -- clear the two static thing lists, then refill them from the thing search tools: every thing with TC type 99 in the current map, and every thing carrying a CTCActionUseScriptedHook (TC type 100)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E92D0, length 144 bytes.
// Leaf: RebuildStaticThingLists
// Label correction: the manifest's "CCreatureAction_UnsheatheToShowToSheatheTrophy::FinishAction" is propagated; the only
// callers are CTCAvoidRegionExit::vfunc_9 @ 0x007E9460 (behind the one-shot static flag 0x013BAEF0) and 0x007E9913, both
// with a CTCAvoidRegionExit in ecx, so this is a CTCAvoidRegionExit member that builds its class-static lists.
// Callees: the static list clear @ 0x004B8050 (STLport list<CIntelligentPointer<CThing>>::clear: 4-byte list objects at
// 0x013BAEFC / 0x013BAEF8), CTCBase::PeekWorldMap @ 0x00686D20 (`[g_pMainGameComponent]+0x28`, landed naming), the world
// map's current-map getter @ 0x004FB150 (`[ecx+9Ch]`), CTCBase::GetThingSearchTools @ 0x00686D90 (tail jump through the
// main game component), CThingSearchTools::GetAllThings<CThing, list, CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap>>
// @ 0x007EA040 and GetAllThings<..., CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>> @ 0x007EA0E0.
// Shape: the filters are temporaries in the call (frame: HasTC type +0, InMap {world map, map} +4, Bind2 {&HasTC, &InMap}
// +0xc; the second call's leaf-type filter reuses the +0 slot). VC evaluates the InMap arguments right to left, so the
// current-map chain (kept in edi) is fetched before the plain world-map fetch, and the search-tools fetch comes last.
class CThing;

struct CThingList {                         // std::list<CIntelligentPointer<CThing>> (STLport: one node pointer)
    void* m_node;
    void clear();                           // 0x004B8050
};

class CWorldMap { public:
    long GetLoadedRegion() const;             // 0x004FB150 (`mov eax,[ecx+9Ch]`)
};

class CThingFilter_HasTC { public:
    long m_type;
    CThingFilter_HasTC(long type) : m_type(type) { }
};
class CThingFilter_InMap { public:
    const CWorldMap* m_worldMap;
    long m_map;
    CThingFilter_InMap(const CWorldMap& worldMap, long map) : m_worldMap(&worldMap), m_map(map) { }
};
struct CFilter_Bind2 {                      // CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap>
    const CThingFilter_HasTC* m_first;
    const CThingFilter_InMap* m_second;
    CFilter_Bind2(const CThingFilter_HasTC& first, const CThingFilter_InMap& second) : m_first(&first), m_second(&second) { }
};
struct CThingFilter_HasTCOfLeafType {       // CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>
    long m_type;
    CThingFilter_HasTCOfLeafType(long type) : m_type(type) { }
};

class CThingSearchTools { public:
    void GetAllThings(const CFilter_Bind2& filter, CThingList& out) const;            // 0x007EA040
    void GetAllThings(const CThingFilter_HasTCOfLeafType& filter, CThingList& out) const;   // 0x007EA0E0
};

extern CThingList g_AvoidRegionThings;      // 0x013BAEFC (CTCAvoidRegionExit static)
extern CThingList g_ScriptedHookThings;     // 0x013BAEF8 (CTCAvoidRegionExit static)

// CTCBase: engine/CTCBase.h (FABLE_RETAIL_CTCBASE): PeekWorldMap 0x00686D20 ?PeekWorldMap@CTCBase@@IBEABVCWorldMap@@XZ and
//   PeekThingSearchTools 0x00686D90 ?PeekThingSearchTools@CTCBase@@IBEABVCThingSearchTools@@XZ (Ego_r.pdb / FableWin.pdb; NF28: was
//   spelled GetThingSearchTools() const returning a pointer).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"

class CTCAvoidRegionExit : public CTCBase { public:
    void RebuildStaticThingLists();
};

void CTCAvoidRegionExit::RebuildStaticThingLists()
{
    g_AvoidRegionThings.clear();
    g_ScriptedHookThings.clear();
    PeekThingSearchTools().GetAllThings(
        CFilter_Bind2(CThingFilter_HasTC(99), CThingFilter_InMap(PeekWorldMap(), PeekWorldMap().GetLoadedRegion())),
        g_AvoidRegionThings);
    PeekThingSearchTools().GetAllThings(CThingFilter_HasTCOfLeafType(100), g_ScriptedHookThings);
}