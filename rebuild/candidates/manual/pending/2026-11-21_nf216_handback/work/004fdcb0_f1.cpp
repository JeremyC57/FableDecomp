// CWorldMap::IsSectionToHaveNavigationMap(const CCharString& sectionName, long mapNo) -- a map gets a navigation map when the section is "NULL" or empty, or when a game object or a building of that quest section stands in the map
// Leaf: ?IsSectionToHaveNavigationMap@CWorldMap@@IBE_NABVCCharString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FDCB0, 293 bytes, thiscall, `ret 8`.
// Identity (proven): the PDB name set holds `?IsSectionToHaveNavigationMap@CWorldMap@@IBE_NABVCCharString@@J@Z`, the only
//   CWorldMap member of the (const CCharString&, long) const -> bool shape; it sits between LoadLoadableGlobalThings
//   0x004FDBC0 and SaveGameState 0x004FDE80 (the PDB order: LoadLoadableGlobalThings, SaveGameState,
//   IsSectionToHaveNavigationMap), and its one caller is CWorldMap::ActivateNavMap 0x0050AF10 (call at 0x0050B0A9).
//   The two searches are the PDB's GetPFirstThing<CFilter_Bind2<CThingFilter_InMap, CThingFilter_IsInQuest>, const
//   CThingObject / const CThingBuilding> (catalog 0x00512ED0 / 0x00512F60); Ego_r.pdb CThingFilter_InMap is
//   {CWorldMap* WorldMap, long MapNo}, CThingFilter_IsInQuest {CCharString QuestSectionName}, CFilter_Bind2 {F1*, F2*}.
// Callees: CBasicString<char>::operator==(const char*) const 0x004115A0 (the inlined CCharString::operator==),
//   CCharString(const CCharString&) 0x0099EC30, ~CCharString 0x0099EAE0, the two GetPFirstThing instantiations.
// Strings: "NULL" 0x012393E4, "" 0x0122D70E.
// Shape: CCharString::operator== inline with its null-block `strcmp(text, "")` arm (landed 0x0083BF90); each search is
//   one call over the CFilter_Bind2 temporary built from the two filter temporaries; its bool result is dropped and the
//   out pointer is tested.
#include "fable_compiler.h"
// Name fix (NF216, 2026-11-21, q12378): ported onto fable_string.h: the local CBasicString / CCharStringData / CCharString models
//   and the same-TU CBasicString<char>::operator==(const char*) specialization (auto_inline(off)) are replaced by FABLE_RETAIL_CCHARSTRING
//   with FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ (the inline CCharString::operator==) + FABLE_RETAIL_CBASICSTRING_COMPARE_BODY +
//   FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO (the visible, called 0x004115A0 body); the pick-any
//   ??8?$CBasicString@D@@QBE_NPBD@Z copy this object carries is now the owner's 88-byte body, equal in bytes and relocation targets.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE_COMPARE_ZERO
#include "fable_string.h"
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

#define CWorldMap CWorldMapLayout
#include "engine/CWorldMap.h"
#undef CWorldMap
#define CThingSearchTools CThingSearchToolsLayout
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
#include "engine/CWorld.h"

class CThingObject;
class CThingBuilding;

class CThingFilter_InMap { public:
    CWorldMap* WorldMap;                            // +0x00
    long       MapNo;                               // +0x04
    CThingFilter_InMap(const CWorldMap* worldMap, long mapNo) : WorldMap((CWorldMap*)worldMap), MapNo(mapNo) { }
};
class CThingFilter_IsInQuest { public:
    CCharString QuestSectionName;                   // +0x00
    CThingFilter_IsInQuest(const CCharString& questSectionName) : QuestSectionName(questSectionName) { }
};
template <class A, class B> class CFilter_Bind2 { public:
    const A* F1;                                    // +0x00
    const B* F2;                                    // +0x04
    CFilter_Bind2(const A& f1, const B& f2) : F1(&f1), F2(&f2) { }
};

class CThingSearchTools : public CThingSearchToolsLayout { public:
    template <class TFilter, class TThing> bool GetPFirstThing(const TFilter& filter, TThing** out) const;   // 0x00512ED0 / 0x00512F60
};

class CWorldMap : public CWorldMapLayout { public:
protected:
    bool IsSectionToHaveNavigationMap(const CCharString& sectionName, long mapNo) const;
};

bool CWorldMap::IsSectionToHaveNavigationMap(const CCharString& sectionName, long mapNo) const
{
    if (sectionName == "NULL" || sectionName == "")
        return true;
    const CThingSearchTools* tools = (const CThingSearchTools*)World->PThingSearchTools_ptr;
    {
        const CThingObject* object = 0;
        tools->GetPFirstThing(CFilter_Bind2<CThingFilter_InMap, CThingFilter_IsInQuest>(CThingFilter_InMap(this, mapNo),
                                                                                        CThingFilter_IsInQuest(sectionName)),
                              &object);
        if (object != 0)
            return true;
    }
    {
        const CThingBuilding* building = 0;
        tools->GetPFirstThing(CFilter_Bind2<CThingFilter_InMap, CThingFilter_IsInQuest>(CThingFilter_InMap(this, mapNo),
                                                                                        CThingFilter_IsInQuest(sectionName)),
                              &building);
        if (building != 0)
            return true;
    }
    return false;
}