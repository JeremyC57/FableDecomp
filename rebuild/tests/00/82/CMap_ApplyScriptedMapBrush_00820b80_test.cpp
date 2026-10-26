// Behaviour test for 00820b80: body copied from src/00820b80.cpp (CMap::ApplyScriptedMapBrush), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_strCopies = 0, g_strDtors = 0;
class CCharString { public:
    void* PStringData;
    CCharString() : PStringData(0) { }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_strCopies; }
    ~CCharString() { ++g_strDtors; }
};
class C2DBoxI { public: long X0, Y0, X1, Y1; };
static int g_getArea = 0, g_getName = 0;
class CScriptedMapBrush { public:
    C2DBoxI       GameArea;
    CCharString   BrushName;
    float         CreationTimeSeconds;
    const C2DBoxI& GetGameArea() const { ++g_getArea; return GameArea; }
    const CCharString& GetBrushName() const { ++g_getName; return BrushName; }
};
struct CScriptedMapBrushUpdateArea {
    float       Timer;
    CCharString Name;
    CScriptedMapBrushUpdateArea(float timer, const CCharString& name) : Timer(timer), Name(name) { }
    CScriptedMapBrushUpdateArea(const CScriptedMapBrushUpdateArea& o) : Timer(o.Timer), Name(o.Name) { }
};
struct __false_type { __false_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }
static int g_overflow = 0;
struct CUpdateAreaVector {
    CScriptedMapBrushUpdateArea* _M_start;
    CScriptedMapBrushUpdateArea* _M_finish;
    CScriptedMapBrushUpdateArea* _M_end_of_storage;
    void _M_insert_overflow(CScriptedMapBrushUpdateArea* position, const CScriptedMapBrushUpdateArea& x, const __false_type&, unsigned long fill_len, bool atend)
    { ++g_overflow; CHECK(position == _M_finish && fill_len == 1 && atend); new (_M_finish) CScriptedMapBrushUpdateArea(x); ++_M_finish; ++_M_end_of_storage; }
    void push_back(const CScriptedMapBrushUpdateArea& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CScriptedMapBrushUpdateArea(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    CScriptedMapBrushUpdateArea& back() { return *(_M_finish - 1); }
};
long g_FramesPerSecond = 30;
static CScriptedMapBrush g_brush; static int g_getBrush = 0, g_apply = 0; static const CCharString* g_name = 0; static float g_applyAmount = 0; static CScriptedMapBrush* g_applyBrush = 0;
class CMap { public:
    void*             __vftable;
    unsigned char     _pad04[0xc];
    CUpdateAreaVector ScriptedMapBrushUpdateAreas;
    CScriptedMapBrush* GetScriptedMapBrush(const CCharString& name) { ++g_getBrush; g_name = &name; return &g_brush; }
    void ApplyScriptedMapBrushUpdate(float amount, CScriptedMapBrush* brush) { ++g_apply; g_applyAmount = amount; g_applyBrush = brush; }
    void ApplyScriptedMapBrush(const CCharString& name, bool overTime);
};
void CMap::ApplyScriptedMapBrush(const CCharString& name, bool overTime)
{
    CScriptedMapBrush* brush = GetScriptedMapBrush(name);
    brush->GetGameArea();
    float time = brush->CreationTimeSeconds;
    ScriptedMapBrushUpdateAreas.push_back(CScriptedMapBrushUpdateArea(time, brush->GetBrushName()));
    if (!overTime || 1.0f / g_FramesPerSecond >= brush->CreationTimeSeconds) {
        ApplyScriptedMapBrushUpdate(1.0f, brush);
        ScriptedMapBrushUpdateAreas.back().Timer = 0;
    }
}
int main()
{
    unsigned char storage[sizeof(CScriptedMapBrushUpdateArea) * 4]; memset(storage, 0, sizeof storage);
    CMap map; memset(&map, 0, sizeof map);
    map.ScriptedMapBrushUpdateAreas._M_start = (CScriptedMapBrushUpdateArea*)storage;
    map.ScriptedMapBrushUpdateAreas._M_finish = map.ScriptedMapBrushUpdateAreas._M_start;
    map.ScriptedMapBrushUpdateAreas._M_end_of_storage = map.ScriptedMapBrushUpdateAreas._M_start + 2;
    g_brush.BrushName.PStringData = (void*)0x1234; g_brush.CreationTimeSeconds = 5.0f;
    CCharString name; name.PStringData = (void*)0x77;
    // immediate apply: pushed with the creation time, then applied and timer zeroed
    map.ApplyScriptedMapBrush(name, false);
    CHECK(g_getBrush == 1 && g_name == &name && g_getArea == 1 && g_getName == 1 && g_overflow == 0);
    CHECK(map.ScriptedMapBrushUpdateAreas._M_finish == map.ScriptedMapBrushUpdateAreas._M_start + 1);
    CHECK(g_apply == 1 && g_applyAmount == 1.0f && g_applyBrush == &g_brush);
    CHECK(map.ScriptedMapBrushUpdateAreas.back().Timer == 0.0f && map.ScriptedMapBrushUpdateAreas.back().Name.PStringData == (void*)0x1234);
    // over time, creation time longer than a frame: queued only, timer kept
    map.ApplyScriptedMapBrush(name, true);
    CHECK(g_apply == 1 && map.ScriptedMapBrushUpdateAreas._M_finish == map.ScriptedMapBrushUpdateAreas._M_start + 2);
    CHECK(map.ScriptedMapBrushUpdateAreas.back().Timer == 5.0f);
    // over time but creation time fits in a frame: applied; vector full -> overflow path
    g_brush.CreationTimeSeconds = 0.01f;
    map.ApplyScriptedMapBrush(name, true);
    CHECK(g_overflow == 1 && g_apply == 2 && map.ScriptedMapBrushUpdateAreas.back().Timer == 0.0f);
    CHECK(g_strCopies >= 3 && g_strDtors >= 3);   // temporaries copied in and destroyed
    if (fails == 0) printf("00820b80_TEST PASS\n"); else printf("00820b80_TEST FAIL (%d)\n", fails);
    return fails;
}