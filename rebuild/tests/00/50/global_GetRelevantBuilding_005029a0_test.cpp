// Behaviour test for GetRelevantBuilding (0x005029A0): among the buildings whose sim definition carries the property (and has a
// free work / living space when asked), the one with the fewest assigned creatures; null defs and full buildings are skipped.
#include <cstdio>
#include <map>
#include <list>
class CSimBuildingDef { public:
    long Property;
    long WorkSpaces;
    long LiveSpaces;
    long RefCount;
};
struct CDefPointer {                                   // counted pointer: the row releases it on every loop exit path
    const CSimBuildingDef* Object;
    explicit CDefPointer(const CSimBuildingDef* o) : Object(o) { if (Object) ++const_cast<CSimBuildingDef*>(Object)->RefCount; }
    ~CDefPointer() { if (Object) --const_cast<CSimBuildingDef*>(Object)->RefCount; }
    bool IsNull() const { return Object == 0; }
    const CSimBuildingDef* operator->() const { return Object; }
};
class CThingBuilding { public:
    CSimBuildingDef* Def;
    CDefPointer PeekPSimDefinition() const { return CDefPointer(Def); }
};
class CThingAICreature { public: int id; };
enum ESimBuildingProperty { SIM_HOME = 1, SIM_WORK = 2, SIM_SHOP = 4 };
typedef std::map<CThingBuilding*, std::list<CThingAICreature*> > BuildingMap;

CThingBuilding* GetRelevantBuilding(BuildingMap& buildings, ESimBuildingProperty property, bool work, bool live)
{
    CThingBuilding* best = 0;
    BuildingMap::iterator end = buildings.end();
    for (BuildingMap::iterator it = buildings.begin(); it != end; ++it)
    {
        CDefPointer def = it->first->PeekPSimDefinition();
        if (!def.IsNull() && (def->Property & property))
        {
            if (!(work || live) || it->second.size() < (unsigned long)(work ? def->WorkSpaces : def->LiveSpaces))
            {
                if (!best || buildings[it->first].size() < buildings[best].size())
                    best = it->first;
            }
        }
    }
    return best;
}

int main()
{
    int fails = 0;
    CSimBuildingDef home = { SIM_HOME, 0, 2, 0 };
    CSimBuildingDef work = { SIM_WORK | SIM_SHOP, 1, 0, 0 };
    CSimBuildingDef both = { SIM_HOME | SIM_WORK, 2, 3, 0 };
    CThingBuilding a = { &home }, b = { &work }, c = { &both }, d = { 0 };
    CThingAICreature v1 = { 1 }, v2 = { 2 }, v3 = { 3 };
    BuildingMap m;
    m[&a].push_back(&v1); m[&a].push_back(&v2);        // home: 2 of 2 living spaces used
    m[&b].push_back(&v3);                              // work: 1 of 1 work space used
    m[&c];                                             // both: empty
    m[&d];                                             // no definition
    // no capacity test: the emptiest building with the property wins
    if (GetRelevantBuilding(m, SIM_HOME, false, false) != &c) { std::printf("case1 fail\n"); ++fails; }
    if (GetRelevantBuilding(m, SIM_SHOP, false, false) != &b) { std::printf("case2 fail\n"); ++fails; }
    // living space: a is full, c has 3
    if (GetRelevantBuilding(m, SIM_HOME, false, true) != &c) { std::printf("case3 fail\n"); ++fails; }
    // work space: b is full, c has 2
    if (GetRelevantBuilding(m, SIM_WORK, true, false) != &c) { std::printf("case4 fail\n"); ++fails; }
    // work beats live when both are asked: b (1 work space, full) skipped, c taken
    if (GetRelevantBuilding(m, SIM_WORK, true, true) != &c) { std::printf("case5 fail\n"); ++fails; }
    // fewest creatures among equals: fill c beyond a
    m[&c].push_back(&v1); m[&c].push_back(&v2); m[&c].push_back(&v3);
    if (GetRelevantBuilding(m, SIM_HOME, false, false) != &a) { std::printf("case6 fail\n"); ++fails; }
    // nothing has the property / everything is full
    if (GetRelevantBuilding(m, (ESimBuildingProperty)8, false, false) != 0) { std::printf("case7 fail\n"); ++fails; }
    if (GetRelevantBuilding(m, SIM_HOME, false, true) != 0) { std::printf("case8 fail\n"); ++fails; }
    // every CDefPointer was released
    if (home.RefCount || work.RefCount || both.RefCount) { std::printf("refcount fail\n"); ++fails; }
    if (fails) { std::printf("005029a0_TEST FAIL\n"); return 1; }
    std::printf("005029a0_TEST PASS\n");
    return 0;
}