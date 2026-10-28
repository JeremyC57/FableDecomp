// Behaviour test for 008a7d60: body copied from src/008a7d60.cpp (CGameScriptInterface::GetThingWithScriptName), callees stubbed.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_strCopies = 0, g_strDtors = 0;
class CCharString { public:
    void* m_pData;
    CCharString(const char* s) : m_pData((void*)s) {}
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_strCopies; }
    ~CCharString() { ++g_strDtors; }
    const CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; return *this; }
};
class C3DVector { public: float x, y, z; };
struct SCountBlock;
static int g_baseCtors = 0;
namespace N_0099a2d0 { struct CBase {
    CBase() { ++g_baseCtors; }
    virtual void v00() {}
}; } using N_0099a2d0::CBase;
struct CScriptThingImp;
struct CScriptThingPImp {
    CScriptThingImp* Data;
    SCountBlock*     Info;
    CScriptThingPImp() : Data(0), Info(0) {}
};
class CScriptThing : public CBase { public:
    CScriptThingPImp PImp;
    CScriptThing() {}
};
static int g_getScriptThing = 0;
struct CThing_Retail {
    unsigned char  _pad00[0x74];
    CCharString    ScriptName;
    unsigned char  _pad78[0x91 - 0x78];
    unsigned char  m_dead : 1;
    unsigned char  m_91_1 : 7;
    CThing_Retail() : ScriptName("") {}
    CScriptThing GetScriptThing() { ++g_getScriptThing; CScriptThing t; t.PImp.Data = (CScriptThingImp*)this; return t; }
};
static int g_frees = 0; static void* g_lastFreed = 0;
static void myfree(void* p) { ++g_frees; g_lastFreed = p; free(p); }
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) myfree(_M_start); }
    bool empty() const { return _M_start == _M_finish; }
    T* begin() { return _M_start; }
};
static CThing_Retail* g_worldThings[4]; static int g_worldCount = 0;
static CThing_Retail* FirstWorldThing() { return g_worldCount ? g_worldThings[0] : 0; }
static CThing_Retail* NextWorldThing(CThing_Retail* previous)
{
    for (int i = 0; i + 1 < g_worldCount; ++i) if (g_worldThings[i] == previous) return g_worldThings[i + 1];
    return 0;
}
static bool HasScriptName(CThing_Retail* thing, const CCharString& name) { return strcmp((const char*)thing->ScriptName.m_pData, (const char*)name.m_pData) == 0; }
struct CThingScriptNameSearch_Retail {
    void FindThingsWithScriptName(const CCharString& name, vector<CThing_Retail*>& things);
};
void CThingScriptNameSearch_Retail::FindThingsWithScriptName(const CCharString& name, vector<CThing_Retail*>& things)
{
    for (CThing_Retail* thing = FirstWorldThing(); thing; thing = NextWorldThing(thing)) {
        if (!HasScriptName(thing, name))
            continue;
        if (things._M_finish == things._M_end_of_storage) {
            unsigned long size = things._M_finish - things._M_start;
            unsigned long len = size ? 2 * size : 8;
            CThing_Retail** start = (CThing_Retail**)malloc(len * sizeof(CThing_Retail*));
            for (unsigned long i = 0; i < size; ++i)
                start[i] = things._M_start[i];
            if (things._M_start)
                myfree(things._M_start);
            things._M_start = start;
            things._M_finish = start + size;
            things._M_end_of_storage = start + len;
        }
        *things._M_finish++ = thing;
    }
}
class CWorld { public: unsigned char _pad00[0x20]; CThingScriptNameSearch_Retail* Things; };
class CMainGameComponent; class CDisplayEngine; struct CDefinitionManager_Retail; struct CPlayerManager_Retail; class CThingSearchTools;
class CGameScriptInterface { public:
    virtual void v000() {}
    CWorld*                     World;
    CMainGameComponent*         Component;
    CDisplayEngine*             DisplayEngine;
    CDefinitionManager_Retail*  DefinitionManager;
    CPlayerManager_Retail*      PlayerManager;
    CThingSearchTools*          ThingSearchTools;
    virtual CScriptThing GetThingWithScriptName(const CCharString& scriptName) const;
    __forceinline void FindThingsWithScriptName(CCharString name, vector<CThing_Retail*>& things) const
    {
        World->Things->FindThingsWithScriptName(name, things);
    }
};
CScriptThing CGameScriptInterface::GetThingWithScriptName(const CCharString& scriptName) const
{
    vector<CThing_Retail*> things;
    FindThingsWithScriptName(scriptName, things);
    if (things.empty())
        return CScriptThing();
    return (*things.begin())->GetScriptThing();
}
int main()
{
    static CThing_Retail a, b, c; a.ScriptName = CCharString("door"); b.ScriptName = CCharString("gate"); c.ScriptName = CCharString("door");
    g_worldThings[0] = &a; g_worldThings[1] = &b; g_worldThings[2] = &c; g_worldCount = 3;
    static CThingScriptNameSearch_Retail search; static CWorld world; world.Things = &search;
    CGameScriptInterface gsi; gsi.World = &world;
    // 1: two matches: the first one's script thing
    g_strCopies = 0; g_strDtors = 0; g_getScriptThing = 0; g_frees = 0; g_baseCtors = 0;
    { CScriptThing t = gsi.GetThingWithScriptName(CCharString("door")); CHECK(t.PImp.Data == (CScriptThingImp*)&a); CHECK(t.PImp.Info == 0); }
    CHECK(g_getScriptThing == 1); CHECK(g_frees == 1); CHECK(g_strCopies >= 1 && g_strDtors >= g_strCopies); CHECK(g_baseCtors == 1);
    // 2: one match
    g_getScriptThing = 0; g_frees = 0;
    { CScriptThing t = gsi.GetThingWithScriptName(CCharString("gate")); CHECK(t.PImp.Data == (CScriptThingImp*)&b); }
    CHECK(g_getScriptThing == 1); CHECK(g_frees == 1);
    // 3: no match: a null script thing, nothing freed, nothing fetched
    g_getScriptThing = 0; g_frees = 0; g_baseCtors = 0;
    { CScriptThing t = gsi.GetThingWithScriptName(CCharString("window")); CHECK(t.PImp.Data == 0 && t.PImp.Info == 0); }
    CHECK(g_getScriptThing == 0); CHECK(g_frees == 0); CHECK(g_baseCtors == 1);
    if (fails == 0) printf("008a7d60_TEST PASS\n"); else printf("008a7d60_TEST FAIL\n");
    return fails;
}