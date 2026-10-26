#include <cstdio>
extern int g_foundCount;   // test-only: the search stand-ins report this many creatures
#include <string.h>
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
class CBaseClass { public:                                     // the polymorphic root (vptr at +0)
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
class CCPPointerInfo { public:
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
class CScriptThing : public CBaseClass { public:                      // vtable 0x01238C8C
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
struct CThingVector {                                   // STLport vector<CThingAICreature*> (malloc allocator)
    CThing_Retail** _M_start;
    CThing_Retail** _M_finish;
    CThing_Retail** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector() { if (_M_start) free(_M_start); }
    bool empty() const { return _M_start == _M_finish; }
    unsigned long size() const { return _M_finish - _M_start; }
};
#pragma pack(push, 1)
struct CBasicStringChar {                               // CBasicString<char>, 13 bytes
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;            // 0x004115A0
};
class CCharStringData { public:                                // 17 bytes
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class C3DVector { public: float X, Y, Z; };
class C2DCoordI { public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};
class C2DBoxI { public:
    C2DCoordI TopLeft, BottomRight;
    C2DBoxI(const C2DCoordI& tl, const C2DCoordI& br) : TopLeft(tl), BottomRight(br) {}
    void Expand(long d) { TopLeft.X -= d; TopLeft.Y -= d; BottomRight.X += d; BottomRight.Y += d; }
};
class CThingFilter_None { public:
    CThingFilter_None() {}
};
class CThingFilter_HasScriptName { public:
    CCharString m_name;
    CThingFilter_HasScriptName(const CCharString& name) : m_name(name) {}
};
struct CThing_Retail {
    CScriptThing GetScriptThing();                      // 0x004C7E50
};
CThing_Retail** FindThings_None(const C2DBoxI* box, const CThingFilter_None* filter, CThing_Retail** capacity);
CThing_Retail** FindThings_HasScriptName(const C2DBoxI* box, const CThingFilter_HasScriptName* filter, CThing_Retail** capacity);
class CThingSearchTools { public:
    void GetAllThingsInRoughArea(const C2DBoxI& box, const CThingFilter_None& filter, CThingVector& out);            // 0x008AD710
    void GetAllThingsInRoughArea(const C2DBoxI& box, const CThingFilter_HasScriptName& filter, CThingVector& out);   // 0x008AD950
};
#pragma auto_inline(off)
void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& box, const CThingFilter_None& filter, CThingVector& out)
{
    CThing_Retail** p = FindThings_None(&box, &filter, out._M_end_of_storage);
    out._M_start = p;
    out._M_finish = p + g_foundCount;
}
void CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& box, const CThingFilter_HasScriptName& filter, CThingVector& out)
{
    CThing_Retail** p = FindThings_HasScriptName(&box, &filter, out._M_end_of_storage);
    out._M_start = p;
    out._M_finish = p + g_foundCount;
}
#pragma auto_inline(on)
struct CWorld_Retail {
    unsigned char      _pad00[0x20];
    CThingSearchTools* ThingSearchTools;                // +0x20
};
class CGameScriptInterface { public:
    virtual long GetAllCreaturesInAreaWithScriptName(const CCharString& scriptName, const C3DVector& position, float radius, CScriptThingVector& things) const;
    CWorld_Retail*   World;                             // +0x04 (engine/CGameScriptInterface.h)
};

long CGameScriptInterface::GetAllCreaturesInAreaWithScriptName(const CCharString& scriptName, const C3DVector& position, float radius, CScriptThingVector& things) const
{
    C2DCoordI centre((long)position.X, (long)position.Y);
    long r = (long)radius;
    C2DBoxI box(centre, centre);
    box.Expand(r);
    CThingVector found;
    if (scriptName == "")
        World->ThingSearchTools->GetAllThingsInRoughArea(box, CThingFilter_None(), found);
    else
        World->ThingSearchTools->GetAllThingsInRoughArea(box, CThingFilter_HasScriptName(scriptName), found);
    if (found.empty())
        return 0;
    things.reserve(found.size());
    for (CThing_Retail** it = found._M_start; it != found._M_finish; ++it) {
        CThing_Retail* creature = *it;
        things.push_back(creature->GetScriptThing());
    }
    return things.size();
}

#include <stdlib.h>
static int g_fail = 0;
static void check(bool c, const char* w) { if (!c) { std::printf("FAIL %s\n", w); g_fail = 1; } }
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
static int g_copies, g_dtors, g_eqCalls;
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
CCharString::~CCharString() { ++g_dtors; }
bool CBasicStringChar::operator==(const char* text) const { ++g_eqCalls; return strcmp(PData, text) == 0; }
static CThing_Retail g_things[8];
static CThing_Retail* g_result[8]; static int g_resultCount; static int g_noneCalls, g_nameCalls; static C2DBoxI g_box(C2DCoordI(0, 0), C2DCoordI(0, 0));
static const CCharStringData* g_filterName;
int g_foundCount;
static CThing_Retail** Make()
{
    g_foundCount = g_resultCount;
    CThing_Retail** p = (CThing_Retail**)malloc(sizeof(CThing_Retail*) * (g_resultCount + 1));
    memcpy(p, g_result, sizeof(CThing_Retail*) * g_resultCount);
    return p;
}
CThing_Retail** FindThings_None(const C2DBoxI* box, const CThingFilter_None*, CThing_Retail**) { ++g_noneCalls; g_box = *box; return Make(); }
CThing_Retail** FindThings_HasScriptName(const C2DBoxI* box, const CThingFilter_HasScriptName* filter, CThing_Retail**)
{
    ++g_nameCalls; g_box = *box; g_filterName = filter->m_name.PStringData; return Make();
}
static CCPPointerInfo g_infos[8];
CScriptThing CThing_Retail::GetScriptThing()
{
    CScriptThing r;
    int i = this - g_things;
    r.PImp.Data = this; r.PImp.Info = &g_infos[i]; ++g_infos[i].RefCount;
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
    ++g_overflows; check(position == _M_finish && fill_len == 1 && atend, "overflow args");
    new (_M_finish) CScriptThing(x); ++_M_finish; _M_end_of_storage = _M_finish;
}
static bool BoxIs(long x0, long y0, long x1, long y1) { return g_box.TopLeft.X == x0 && g_box.TopLeft.Y == y0 && g_box.BottomRight.X == x1 && g_box.BottomRight.Y == y1; }
int main()
{
    CGameScriptInterface gsi; CWorld_Retail world; CThingSearchTools tools;
    world.ThingSearchTools = &tools; gsi.World = &world;
    CScriptThingVector things; things._M_start = things._M_finish = things._M_end_of_storage = 0;
    C3DVector pos; pos.X = 100.7f; pos.Y = -20.2f; pos.Z = 5;
    // null script name: the empty filter, nothing found
    CCharString empty = *reinterpret_cast<CCharString*>(&g_result[7]); empty.PStringData = 0; g_copies = 0;
    g_resultCount = 0;
    long n = gsi.GetAllCreaturesInAreaWithScriptName(empty, pos, 10.9f, things);
    check(n == 0 && g_noneCalls == 1 && g_nameCalls == 0 && g_eqCalls == 0 && g_reserves == 0, "null name, none found");
    check(BoxIs(90, -30, 110, -10), "box truncated and grown");
    // a name holding "" also counts as empty (out-of-line compare)
    static char emptyText[1] = ""; static CCharStringData emptyData; emptyData.Data.PData = emptyText;
    CCharString blank = empty; blank.PStringData = &emptyData;
    g_result[0] = &g_things[0]; g_result[1] = &g_things[1]; g_resultCount = 2; g_reserveCap = 1;
    for (int i = 0; i < 8; ++i) g_infos[i].RefCount = 1;
    n = gsi.GetAllCreaturesInAreaWithScriptName(blank, pos, 2.0f, things);
    check(n == 2 && g_noneCalls == 2 && g_eqCalls == 1 && g_reserveArg == 2 && g_overflows == 1, "blank name, two creatures");
    check(things._M_start[0].PImp.Data == &g_things[0] && things._M_start[1].PImp.Data == &g_things[1] && g_infos[0].RefCount == 2 && g_infos[1].RefCount == 2, "script things appended");
    check(BoxIs(98, -22, 102, -18), "box radius 2");
    // a real name: the script-name filter (a copy, destroyed after the search)
    static char guardText[] = "GUARD"; static CCharStringData guardData; guardData.Data.PData = guardText;
    CCharString guard = empty; guard.PStringData = &guardData;
    int copies = g_copies, dtors = g_dtors;
    g_result[0] = &g_things[2]; g_resultCount = 1; g_reserveCap = 8;
    n = gsi.GetAllCreaturesInAreaWithScriptName(guard, pos, 0.5f, things);
    check(n == 3 && g_nameCalls == 1 && g_filterName == &guardData && g_copies == copies + 1 && g_dtors == dtors + 1, "named filter");
    check(BoxIs(100, -20, 100, -20) && things._M_start[2].PImp.Data == &g_things[2], "zero radius box");
    if (!g_fail) std::printf("008a86c0_TEST PASS\n");
    return g_fail;
}