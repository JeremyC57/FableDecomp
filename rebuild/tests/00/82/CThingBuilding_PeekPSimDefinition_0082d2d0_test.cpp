// Behaviour test for 0082d2d0: body copied from src/0082d2d0.cpp (CThingBuilding::PeekPSimDefinition), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_deleted = 0;
class CDefClassBase { public:
    virtual void v00() {}
    virtual void Delete() { ++g_deleted; }
    long RefCount;
};
#pragma pack(push, 1)
class CThingBuildingDef : public CDefClassBase { public:
    unsigned char _pad08[0x74 - 0x08];
    unsigned long SimBuildingDef;
};
#pragma pack(pop)
class CSimBuildingDef : public CDefClassBase { public: };
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CDefPointer()
    {
        CDefClassBase* old = Object;
        if (old) {
            if (--old->RefCount == 0)
                old->Delete();
        }
    }
};
static CThingBuildingDef* g_buildingDef = 0; static CSimBuildingDef* g_subDef = 0; static int g_getBuilding = 0, g_getSub = 0; static long g_buildingIndex = -1, g_subIndex = -1;
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CThingBuildingDef>* out)
    {
        ++g_getBuilding; g_buildingIndex = index;
        if (!g_buildingDef) return false;
        ++g_buildingDef->RefCount; out->Object = g_buildingDef; return true;
    }
    bool GetDef(long index, CDefPointer<CSimBuildingDef>* out)
    {
        ++g_getSub; g_subIndex = index;
        if (!g_subDef) return false;
        ++g_subDef->RefCount; out->Object = g_subDef; return true;
    }
};
static CDefinitionManager g_manager;
class CThingBuilding { public:
    unsigned char  _pad00[0x8c];
    unsigned short DefGlobalIndex;
    CDefinitionManager* GetDefinitionManager() const { return &g_manager; }
    const CThingBuildingDef* PeekPDef() const
    {
        CDefPointer<CThingBuildingDef> def;
        GetDefinitionManager()->GetDef(DefGlobalIndex, &def);
        return def.Object;
    }
    CDefPointer<CSimBuildingDef> PeekPSimDefinition() const;
};
CDefPointer<CSimBuildingDef> CThingBuilding::PeekPSimDefinition() const
{
    if (PeekPDef()->SimBuildingDef) {
        CDefPointer<CSimBuildingDef> sim;
        GetDefinitionManager()->GetDef(PeekPDef()->SimBuildingDef, &sim);
        return sim;
    }
    return CDefPointer<CSimBuildingDef>();
}
int main()
{
    static CThingBuildingDef bdef; static CSimBuildingDef sub; bdef.RefCount = 1; sub.RefCount = 1; g_buildingDef = &bdef; g_subDef = &sub;
    CThingBuilding b; memset(&b, 0, sizeof(b)); b.DefGlobalIndex = 0x77;
    // the def names a sub-def: resolved, the result holds one reference
    bdef.SimBuildingDef = 0x55;
    {
        CDefPointer<CSimBuildingDef> r = b.PeekPSimDefinition();
        CHECK(r.Object == &sub && sub.RefCount == 2); CHECK(g_getBuilding == 2 && g_buildingIndex == 0x77 && g_getSub == 1 && g_subIndex == 0x55); CHECK(bdef.RefCount == 1 && g_deleted == 0);
    }
    CHECK(sub.RefCount == 1);
    // the def names nothing: a null pointer, no second lookup
    bdef.SimBuildingDef = 0;
    { CDefPointer<CSimBuildingDef> r = b.PeekPSimDefinition(); CHECK(r.Object == 0 && g_getSub == 1 && g_getBuilding == 3); }
    // sub-def lookup fails: a null pointer
    bdef.SimBuildingDef = 0x55; g_subDef = 0;
    { CDefPointer<CSimBuildingDef> r = b.PeekPSimDefinition(); CHECK(r.Object == 0 && g_getSub == 2); }
    // a building def held only by the lookup is deleted after each peek
    bdef.RefCount = 0; bdef.SimBuildingDef = 0; { CDefPointer<CSimBuildingDef> r = b.PeekPSimDefinition(); } CHECK(g_deleted == 1);
    if (fails == 0) printf("0082d2d0_TEST PASS\n"); else printf("0082d2d0_TEST FAIL\n");
    return fails;
}