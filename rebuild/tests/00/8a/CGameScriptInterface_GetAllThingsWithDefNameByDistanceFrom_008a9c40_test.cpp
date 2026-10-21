#include <cstdio>
extern int g_foundCount;   // test-only: the stand-in for the 0x008ADBA0 search reports this many things
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct CBaseClass {                                     // the polymorphic root (vptr at +0)
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
struct CCPPointerInfo {
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CCountedScriptImp {                              // CCountedPointer<CScriptThingImp>
    void* Data;
    CCPPointerInfo* Info;
    CCountedScriptImp() : Data(0), Info(0) {}
    CCountedScriptImp(const CCountedScriptImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedScriptImp()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThing : CBaseClass {                      // vtable 0x01238C8C
    CCountedScriptImp PImp;                             // +0x04
    CScriptThing() {}
    virtual ~CScriptThing() {}
};
struct __false_type { __false_type() {} };
struct CScriptThingVector {                             // STLport vector<CScriptThing> (12-byte elements)
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    void reserve(unsigned long n);                      // 0x008ACD30
    void _M_insert_overflow(CScriptThing* position, const CScriptThing& x, const __false_type&,
                            unsigned long fill_len, bool atend);   // 0x008AD5F0
    void push_back(const CScriptThing& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CScriptThing(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CThing_Retail;
struct CThingVector {                                   // STLport vector<CThing*> (malloc allocator)
    CThing_Retail** _M_start;
    CThing_Retail** _M_finish;
    CThing_Retail** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector() { if (_M_start) free(_M_start); }
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
};
struct CCharString {
    void* PStringData;
};
struct C3DVector { float X, Y, Z; C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {} };
struct CScriptThingVirtuals : CBaseClass {              // CScriptThing's virtual interface (slots 1-6)
    virtual bool IsDead() const;
    virtual const CCharString& GetName() const;
    virtual void _slot3();
    virtual void _slot4();
    virtual void _slot5();
    virtual const C3DVector& GetPos() const;            // +0x18
};
struct CScriptThingFrom : CScriptThingVirtuals {
    CCountedScriptImp PImp;
};
struct CThingFilter_HasDefGlobalIndex {
    unsigned long DefGlobalIndex;
    CThingFilter_HasDefGlobalIndex(unsigned long index) : DefGlobalIndex(index) {}
};
struct CThingCompare_Nearest {
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) {}
};
namespace std {
template <class It, class Cmp> void __fastcall sort(It first, It last, Cmp comp);   // 0x007A4260 for CThing**
}
struct CThing_Retail {
    CScriptThing GetScriptThing();                      // 0x004C7E50
};
struct CGameDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
CGameDefinitionManager* __fastcall GetGameDefinitionManager();   // 0x0044C6B0
struct CThingSearchTools {
    void GetAllThings(const CThingFilter_HasDefGlobalIndex& filter, CThingVector& out);   // 0x008ADBA0
};
CThing_Retail** FindThings_GetAllThingsWithDefGlobalIndex(const CThingFilter_HasDefGlobalIndex* filter, CThing_Retail** capacity);
#pragma auto_inline(off)
void CThingSearchTools::GetAllThings(const CThingFilter_HasDefGlobalIndex& filter, CThingVector& out)
{
    CThing_Retail** p = FindThings_GetAllThingsWithDefGlobalIndex(&filter, out._M_end_of_storage);
    out._M_start = p;
    out._M_finish = p + g_foundCount;
}
#pragma auto_inline(on)
struct CWorld_Retail {
    unsigned char      _pad00[0x20];
    CThingSearchTools* ThingSearchTools;                // +0x20
};
struct CGameScriptInterface {
    virtual long GetAllThingsWithDefNameByDistanceFrom(const CScriptThingFrom& from, const CCharString& defName, CScriptThingVector& things) const;
    CWorld_Retail*   World;                             // +0x04 (engine/CGameScriptInterface.h)
};

long CGameScriptInterface::GetAllThingsWithDefNameByDistanceFrom(const CScriptThingFrom& from, const CCharString& defName, CScriptThingVector& things) const
{
    CThingVector found;
    World->ThingSearchTools->GetAllThings(CThingFilter_HasDefGlobalIndex(GetGameDefinitionManager()->GetDefGlobalIndexFromName(defName)), found);
    if (found.empty())
        return 0;
    things.reserve(found.size());
    std::sort(found._M_start, found._M_finish, CThingCompare_Nearest(from.GetPos()));
    for (CThing_Retail** it = found._M_start; it != found._M_finish; ++it) {
        CThing_Retail* thing = *it;
        things.push_back(thing->GetScriptThing());
    }
    return things.size();
}

#include <stdlib.h>
#include <string.h>
static int g_fail = 0;
static void check(bool c, const char* w) { if (!c) { std::printf("FAIL %s\n", w); g_fail = 1; } }
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
bool CScriptThingVirtuals::IsDead() const { return false; }
const CCharString& CScriptThingVirtuals::GetName() const { static CCharString s; return s; }
void CScriptThingVirtuals::_slot3() {}
void CScriptThingVirtuals::_slot4() {}
void CScriptThingVirtuals::_slot5() {}
struct Vec3 { float X, Y, Z; };
static Vec3 g_fromPos;
const C3DVector& CScriptThingVirtuals::GetPos() const { return *reinterpret_cast<const C3DVector*>(&g_fromPos); }
static CGameDefinitionManager g_defs; static const CCharString* g_defNameSeen; static long g_defIndex;
CGameDefinitionManager* __fastcall GetGameDefinitionManager() { return &g_defs; }
long CGameDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) { g_defNameSeen = &name; return g_defIndex; }
static CThing_Retail g_things[8]; static Vec3 g_thingPos[8];
static CThing_Retail* g_result[8]; static int g_resultCount; static unsigned long g_filterSeen;
int g_foundCount;
CThing_Retail** FindThings_GetAllThingsWithDefGlobalIndex(const CThingFilter_HasDefGlobalIndex* filter, CThing_Retail** capacity)
{
    g_filterSeen = filter->DefGlobalIndex;
    g_foundCount = g_resultCount;
    if (g_resultCount < 0) { g_foundCount = 0; return 0; }
    CThing_Retail** p = (CThing_Retail**)malloc(sizeof(CThing_Retail*) * (g_resultCount + 1));
    memcpy(p, g_result, sizeof(CThing_Retail*) * g_resultCount);
    return p;
}
static float DistSq(CThing_Retail* t, const Vec3& from)
{
    const Vec3& p = g_thingPos[t - g_things];
    float dx = p.X - from.X, dy = p.Y - from.Y, dz = p.Z - from.Z;
    return dx * dx + dy * dy + dz * dz;
}
static Vec3 g_sortPos; static int g_sorts; static long g_sortLen;
namespace std {
template <class It, class Cmp> void __fastcall sort(It first, It last, Cmp comp)
{
    ++g_sorts; g_sortLen = last - first; memcpy(&g_sortPos, &comp.Pos, sizeof(Vec3));
    for (It i = first; i != last; ++i)
        for (It j = i + 1; j != last; ++j)
            if (DistSq(*j, g_sortPos) < DistSq(*i, g_sortPos)) { CThing_Retail* t = *i; *i = *j; *j = t; }
}
}
static CCPPointerInfo g_infos[8];
CScriptThing CThing_Retail::GetScriptThing()
{
    CScriptThing r;
    int i = this - g_things;
    r.PImp.Data = this;
    r.PImp.Info = &g_infos[i];
    ++g_infos[i].RefCount;
    return r;
}
static unsigned char g_storage[16 * sizeof(CScriptThing)]; static unsigned long g_reserveArg; static int g_reserves, g_overflows; static long g_reserveCap;
void CScriptThingVector::reserve(unsigned long n)
{
    ++g_reserves; g_reserveArg = n;
    long count = _M_finish - _M_start;
    if (_M_start != (CScriptThing*)g_storage) { _M_start = (CScriptThing*)g_storage; _M_finish = _M_start + count; }
    _M_end_of_storage = _M_start + g_reserveCap;
}
void CScriptThingVector::_M_insert_overflow(CScriptThing* position, const CScriptThing& x, const __false_type&, unsigned long fill_len, bool atend)
{
    ++g_overflows;
    check(position == _M_finish && fill_len == 1 && atend, "overflow args");
    new (_M_finish) CScriptThing(x);
    ++_M_finish;
    _M_end_of_storage = _M_finish;
}
struct TestFrom : CScriptThingFrom {};
int main()
{
    CGameScriptInterface gsi; CWorld_Retail world; CThingSearchTools tools;
    world.ThingSearchTools = &tools; gsi.World = &world;
    TestFrom from; CCharString defName; CScriptThingVector things; things._M_start = things._M_finish = things._M_end_of_storage = 0;
    // none found: 0, output untouched
    g_defIndex = 42; g_resultCount = 0;
    long n = gsi.GetAllThingsWithDefNameByDistanceFrom(from, defName, things);
    check(n == 0 && g_defNameSeen == &defName && g_filterSeen == 42 && g_reserves == 0 && g_sorts == 0, "empty");
    g_resultCount = -1;
    n = gsi.GetAllThingsWithDefNameByDistanceFrom(from, defName, things);
    check(n == 0 && g_reserves == 0, "empty null buffer");
    // three things, sorted nearest-first from (10,0,0); capacity 2 forces one overflow push
    g_fromPos.X = 10; g_fromPos.Y = 0; g_fromPos.Z = 0;
    g_thingPos[0].X = 0;  g_thingPos[1].X = 9; g_thingPos[2].X = 15;
    g_result[0] = &g_things[0]; g_result[1] = &g_things[1]; g_result[2] = &g_things[2]; g_resultCount = 3;
    for (int i = 0; i < 8; ++i) g_infos[i].RefCount = 1;
    g_defIndex = 7; g_reserveCap = 2;
    n = gsi.GetAllThingsWithDefNameByDistanceFrom(from, defName, things);
    check(n == 3 && g_filterSeen == 7 && g_reserves == 1 && g_reserveArg == 3, "reserve count");
    check(g_sorts == 1 && g_sortLen == 3 && g_sortPos.X == 10 && g_sortPos.Y == 0, "sort args");
    check(things._M_start[0].PImp.Data == &g_things[1] && things._M_start[1].PImp.Data == &g_things[2] && things._M_start[2].PImp.Data == &g_things[0], "nearest first");
    check(g_overflows == 1, "one overflow push");
    check(g_infos[0].RefCount == 2 && g_infos[1].RefCount == 2 && g_infos[2].RefCount == 2, "temporaries released");
    // appending to a non-empty output returns the total size
    g_result[0] = &g_things[3]; g_resultCount = 1; g_reserveCap = 8; g_infos[3].RefCount = 1;
    n = gsi.GetAllThingsWithDefNameByDistanceFrom(from, defName, things);
    check(n == 4 && things._M_start[3].PImp.Data == &g_things[3] && g_overflows == 1 && g_infos[3].RefCount == 2, "append total");
    if (!g_fail) std::printf("008a9c40_TEST PASS\n");
    return g_fail;
}