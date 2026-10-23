// Behaviour test for CGameScriptInterface::GetNearestWithDefName (0x0089A880): the model and body of final.cpp with the
// callees stubbed. Checks: (1) no things with the def name -> an empty handle; (2) of several things the nearest (2D,
// XY only) to `from` is returned, `from` itself is skipped, Z is ignored; (3) a single thing is returned even when it IS
// `from` (the size() > 1 guard never updates the start candidate); (4) a handle without an implementation measures from
// the zero vector; (5) every copied handle releases its reference block and the vector storage is freed.
#include <stdio.h>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif

static int g_frees = 0, g_blockDeletes = 0, g_impDeletes = 0;
void FreeStorage(void* p) { g_frees++; free(p); }
struct CCharString { void* PStringData; };
struct C3DVector { float X, Y, Z; };
C3DVector g_ZeroVector = { 0.0f, 0.0f, 0.0f };

struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& p) : Data(p.Data), Info(p.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                g_blockDeletes++;
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CBaseClass {
    CBaseClass() {}
    virtual ~CBaseClass() {}
};
struct CScriptThing : CBaseClass {
    CCountedPointer<CScriptThing> PImp;
    CScriptThing() { }
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    ~CScriptThing() { }
    virtual const C3DVector& GetPos() const { if (PImp.Data == 0) return g_ZeroVector; return PImp.Data->GetPos(); }
    virtual bool IsSameThing(const CScriptThing* other) const { return false; }
    bool operator==(const CScriptThing& other) const { return PImp.Data->IsSameThing(other.PImp.Data); }
};
// the implementation objects behind the handles: GetPos/IsSameThing are answered by the implementation itself
struct CThingImp : CScriptThing {
    C3DVector Pos;
    long Id;
    virtual const C3DVector& GetPos() const { return Pos; }
    virtual bool IsSameThing(const CScriptThing* other) const { return other == (const CScriptThing*)this; }
};
static void __fastcall DeleteImp(void* data) { g_impDeletes++; }
static CScriptThing MakeHandle(CThingImp* imp)
{
    CScriptThing h;
    if (imp) {
        h.PImp.Data = imp;
        h.PImp.Info = new CCPPointerInfo;
        h.PImp.Info->RefCount = 1; h.PImp.Info->DeleteFunc = DeleteImp; h.PImp.Info->Data = imp;
    }
    return h;
}

template <class T> __forceinline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            FreeStorage(_M_start);
    }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
};

static CThingImp* g_pool[4]; static int g_poolCount = 0;
struct CGameScriptInterface {
    virtual long GetAllThingsWithDefName(const CCharString& defName, vector<CScriptThing>& things) const
    {
        if (g_poolCount == 0) return 0;
        things._M_start = (CScriptThing*)malloc(sizeof(CScriptThing) * g_poolCount);
        things._M_finish = things._M_start;
        for (int i = 0; i < g_poolCount; ++i, ++things._M_finish)
            new (things._M_finish) CScriptThing(MakeHandle(g_pool[i]));
        things._M_end_of_storage = things._M_finish;
        return g_poolCount;
    }
    CScriptThing GetNearestWithDefName(const CScriptThing& from, const CCharString& defName) const;
};
CScriptThing CGameScriptInterface::GetNearestWithDefName(const CScriptThing& from, const CCharString& defName) const
{
    vector<CScriptThing> things;
    GetAllThingsWithDefName(defName, things);
    if (things.empty())
        return CScriptThing();
    long nearestDistance = 100000;
    CScriptThing* nearest = things.begin();
    for (CScriptThing* it = things.begin(); it != things.end(); ++it) {
        CScriptThing thing = *it;
        const C3DVector& pos = thing.GetPos();
        const C3DVector& fromPos = from.GetPos();
        float dx = pos.X - fromPos.X;
        float dy = pos.Y - fromPos.Y;
        float distance = dx * dx + dy * dy;
        if (distance < nearestDistance) {
            CScriptThing* fromItem = from.PImp.Data;
            if (!thing.PImp.Data->IsSameThing(fromItem) && things.size() > 1) {
                nearestDistance = (long)distance;
                nearest = it;
            }
        }
    }
    return *nearest;
}

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

int main()
{
    CGameScriptInterface gsi; CCharString name = { 0 };
    CThingImp a, b, c, d;
    a.Pos.X = 0.0f; a.Pos.Y = 0.0f; a.Pos.Z = 0.0f; a.Id = 1;       // `from`
    b.Pos.X = 3.0f; b.Pos.Y = 4.0f; b.Pos.Z = 100.0f; b.Id = 2;     // 25 away in XY (Z ignored)
    c.Pos.X = 1.0f; c.Pos.Y = 1.0f; c.Pos.Z = 0.0f; c.Id = 3;       // 2 away: nearest
    d.Pos.X = -6.0f; d.Pos.Y = 0.0f; d.Pos.Z = 0.0f; d.Id = 4;      // 36 away
    {
        CScriptThing from = MakeHandle(&a);
        // (1) nothing found
        g_poolCount = 0;
        CScriptThing none = gsi.GetNearestWithDefName(from, name);
        CHECK(none.PImp.Data == 0 && none.PImp.Info == 0);
        // (2) a (= from, skipped), b, c, d -> c
        g_pool[0] = &a; g_pool[1] = &b; g_pool[2] = &c; g_pool[3] = &d; g_poolCount = 4;
        int blocksBefore = g_blockDeletes, freesBefore = g_frees;
        CScriptThing near1 = gsi.GetNearestWithDefName(from, name);
        CHECK(near1.PImp.Data == &c && near1.PImp.Info && near1.PImp.Info->RefCount == 1);
        CHECK(g_frees == freesBefore + 1 && g_blockDeletes == blocksBefore + 3);   // a, b, d blocks released; c's kept by the result
        // from first then others: still c; from nearest but skipped
        g_pool[0] = &c; g_pool[1] = &a; g_pool[2] = &d; g_poolCount = 3;
        CScriptThing near2 = gsi.GetNearestWithDefName(from, name);
        CHECK(near2.PImp.Data == &c);
        // (3) a single thing that is `from` itself is still returned (start candidate)
        g_pool[0] = &a; g_poolCount = 1;
        CScriptThing only = gsi.GetNearestWithDefName(from, name);
        CHECK(only.PImp.Data == &a);
        // (4) a from-handle with no implementation measures from the origin: d (36) vs b (25) -> b
        CScriptThing empty;
        g_pool[0] = &d; g_pool[1] = &b; g_poolCount = 2;
        CScriptThing near3 = gsi.GetNearestWithDefName(empty, name);
        CHECK(near3.PImp.Data == &b);
    }
    // (5) every block released once its last handle is gone
    CHECK(g_blockDeletes == g_impDeletes && g_frees == 4);

    char line[64];
    sprintf(line, "0089a880_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}