// Behaviour test for 0x008A89D0 CGameScriptInterface::GetAllCreaturesExcludingHero: every AI creature the search finds
// is appended (in order) to the caller's script-thing vector after a reserve(count); returns the vector's new size;
// the counted script-thing references are balanced (each temporary released, each stored copy holds one).
#include <stdio.h>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
struct CBaseClass { CBaseClass() {} virtual ~CBaseClass() {} };
struct CCPPointerInfo { unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
static int g_deleted = 0;
static void __fastcall DeleteImp(void*) { ++g_deleted; }
struct CCountedScriptImp {
    void* Data; CCPPointerInfo* Info;
    CCountedScriptImp() : Data(0), Info(0) {}
    CCountedScriptImp(const CCountedScriptImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedScriptImp()
    {
        if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
        Data = 0; Info = 0;
    }
};
struct CScriptThing : CBaseClass { CCountedScriptImp PImp; CScriptThing() {} virtual ~CScriptThing() {} };
struct __false_type { __false_type() {} };
struct CScriptThingVector {
    CScriptThing* _M_start; CScriptThing* _M_finish; CScriptThing* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    int reserved;
    void reserve(unsigned long n) { reserved = (int)n; grow(n); }
    void grow(unsigned long n)
    {
        if ((unsigned long)(_M_end_of_storage - _M_start) >= n) return;
        CScriptThing* p = (CScriptThing*)malloc(n * sizeof(CScriptThing));
        long sz = size();
        for (long i = 0; i < sz; ++i) { new (p + i) CScriptThing(_M_start[i]); _M_start[i].~CScriptThing(); }
        free(_M_start);
        _M_start = p; _M_finish = p + sz; _M_end_of_storage = p + n;
    }
    void _M_insert_overflow(CScriptThing* position, const CScriptThing& x, const __false_type&, unsigned long fill_len, bool atend)
    {
        grow(size() * 2 + 1);
        new (_M_finish) CScriptThing(x); ++_M_finish;
    }
    void push_back(const CScriptThing& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) CScriptThing(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CThing_Retail;
struct CThingVector {
    CThing_Retail** _M_start; CThing_Retail** _M_finish; CThing_Retail** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    CThing_Retail* operator[](unsigned long n) const { return _M_start[n]; }
};
struct CThingFilter_None { CThingFilter_None() {} };
struct CThingFilter_HasTC { CThingFilter_HasTC() {} };
template <class F1, class F2> struct CFilter_Bind2 : public F1, public F2 { CFilter_Bind2() {} };
typedef CFilter_Bind2<CThingFilter_None, CThingFilter_HasTC> CCreatureFilter;
struct CThing_Retail {
    int Id;
    CScriptThing GetScriptThing()
    {
        CScriptThing s;
        s.PImp.Data = this;
        s.PImp.Info = new CCPPointerInfo;
        s.PImp.Info->RefCount = 1; s.PImp.Info->DeleteFunc = DeleteImp; s.PImp.Info->Data = this;
        return s;
    }
};
static CThing_Retail g_creatures[3];
static int g_creatureCount = 3;
struct CThingSearchTools {
    void GetAllThings(const CCreatureFilter& filter, CThingVector& out)
    {
        out._M_start = (CThing_Retail**)malloc(sizeof(CThing_Retail*) * 4);
        out._M_finish = out._M_start;
        for (int i = 0; i < g_creatureCount; ++i) *out._M_finish++ = &g_creatures[i];
        out._M_end_of_storage = out._M_start + 4;
    }
};
struct CWorld_Retail { unsigned char _pad00[0x20]; CThingSearchTools* ThingSearchTools; };
struct CGameScriptInterface {
    virtual long GetAllCreaturesExcludingHero(CScriptThingVector& things) const;
    virtual ~CGameScriptInterface() {}
    CWorld_Retail* World;
};
long CGameScriptInterface::GetAllCreaturesExcludingHero(CScriptThingVector& things) const
{
    CThingVector found;
    World->ThingSearchTools->GetAllThings(CCreatureFilter(), found);
    const unsigned long count = found.size();
    things.reserve(count);
    for (unsigned long i = 0; i < count; ++i)
        things.push_back(found[i]->GetScriptThing());
    return things.size();
}
int main()
{
    CThingSearchTools tools; CWorld_Retail world; world.ThingSearchTools = &tools;
    CGameScriptInterface gsi; gsi.World = &world;
    CScriptThingVector v; v._M_start = v._M_finish = v._M_end_of_storage = 0; v.reserved = -1;
    long n = gsi.GetAllCreaturesExcludingHero(v);
    bool ok = n == 3 && v.size() == 3 && v.reserved == 3 && g_deleted == 0;
    for (int i = 0; ok && i < 3; ++i) ok = v._M_start[i].PImp.Data == &g_creatures[i] && v._M_start[i].PImp.Info->RefCount == 1;
    g_creatureCount = 2;
    n = gsi.GetAllCreaturesExcludingHero(v);
    ok = ok && n == 5 && v.reserved == 2 && v._M_start[4].PImp.Data == &g_creatures[1];
    for (long i = 0; i < v.size(); ++i) v._M_start[i].~CScriptThing();
    ok = ok && g_deleted == 5;
    free(v._M_start);
    if (!ok) { printf("008a89d0_TEST FAIL\n"); return 1; }
    printf("008a89d0_TEST PASS\n");
    return 0;
}
