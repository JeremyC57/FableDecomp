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
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)

#define CWorldMap CWorldMapLayout
#include "engine/CWorldMap.h"
#undef CWorldMap
#define CThingSearchTools CThingSearchToolsLayout
#include "engine/CThingSearchTools.h"
#undef CThingSearchTools
#include "engine/CWorld.h"

#pragma pack(push, 1)
template <class T> class CBasicString { public:
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;        // 0x004115A0
};
class CCharStringData { public:
    CBasicString<char> Data;
    long             NoRefs;
};
#pragma pack(pop)
// The CBasicString<char>::operator== body (landed 0x004115A0) compiled in this TU under auto_inline(off): retail's caller
// keeps PStringData in ebp across the first call, which needs VC7.1 to see that the comparison writes no memory.
#pragma auto_inline(off)
template <> bool CBasicString<char>::operator==(const char* text) const
{
    const char* a = PData;
    const char* b = text;
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return true;
        if (ca != *b)
            return false;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                                      // 0x0099EC30
    ~CCharString();                                                             // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};

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