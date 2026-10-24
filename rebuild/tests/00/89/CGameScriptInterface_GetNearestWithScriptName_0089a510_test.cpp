// Behaviour test for 0x0089A510 CGameScriptInterface::GetNearestWithScriptName: of the things with the script name, the
// one nearest (2D, x/y) to `from` other than `from` itself; the first one when only `from` matches; an empty handle when
// none match; the squared distance is truncated to a long when stored (-1 = none yet).
#include <stdio.h>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
struct C3DVector { float X, Y, Z; };
static C3DVector g_ZeroVector = { 0, 0, 0 };
struct CCPPointerInfo { long RefCount; void (__fastcall* DeleteFunc)(void* data); void* Data; };
static void __fastcall NoDelete(void*) { }
template <class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& p) : Data(p.Data), Info(p.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } Data = 0; Info = 0; }
};
struct CBaseClass { CBaseClass() { } virtual ~CBaseClass() { } };
struct CScriptThing : CBaseClass {
    CCountedPointer<CScriptThing> PImp;
    C3DVector Pos; int Id;                               // implementation-object data (test only)
    CScriptThing() : Id(0) { Pos = g_ZeroVector; }
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp), Pos(other.Pos), Id(other.Id) { }
    ~CScriptThing() { }
    virtual const C3DVector& GetPos() const { if (PImp.Data == 0) return g_ZeroVector; return PImp.Data->GetImpPos(); }
    virtual const C3DVector& GetImpPos() const { return Pos; }
    virtual bool IsSameThing(const CScriptThing* other) const { return other == this; }
};
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { for (T* p = _M_start; p != _M_finish; ++p) p->~T(); if (_M_start) free(_M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    bool empty() const { return _M_start == _M_finish; }
};
struct CCharString { void* PStringData; };
static CScriptThing* g_imps[8]; static int g_impCount = 0;
static CScriptThing MakeHandle(CScriptThing* imp)
{
    CScriptThing h; h.PImp.Data = imp; h.PImp.Info = new CCPPointerInfo; h.PImp.Info->RefCount = 1;
    h.PImp.Info->DeleteFunc = NoDelete; h.PImp.Info->Data = imp; return h;
}
struct CGameScriptInterface {
    long GetAllThingsWithScriptName(const CCharString&, vector<CScriptThing>& things) const
    {
        if (g_impCount == 0) return 0;
        things._M_start = (CScriptThing*)malloc(sizeof(CScriptThing) * g_impCount);
        things._M_finish = things._M_end_of_storage = things._M_start + g_impCount;
        for (int i = 0; i < g_impCount; ++i) new (things._M_start + i) CScriptThing(MakeHandle(g_imps[i]));
        return g_impCount;
    }
    CScriptThing GetNearestWithScriptName(const CScriptThing& from, const CCharString& scriptName) const;
};
CScriptThing CGameScriptInterface::GetNearestWithScriptName(const CScriptThing& from, const CCharString& scriptName) const
{
    vector<CScriptThing> things;
    GetAllThingsWithScriptName(scriptName, things);
    if (things.empty())
        return CScriptThing();
    long nearestDistance = -1;
    CScriptThing* nearest = things.begin();
    for (CScriptThing* it = things.begin(); it != things.end(); ++it) {
        CScriptThing thing = *it;
        CScriptThing* fromItem = from.PImp.Data;
        if (!thing.PImp.Data->IsSameThing(fromItem)) {
            const C3DVector& pos = thing.GetPos();
            const C3DVector& fromPos = from.GetPos();
            float dx = pos.X - fromPos.X;
            float dy = pos.Y - fromPos.Y;
            float distance = dx * dx + dy * dy;
            if (nearestDistance == -1 || distance < nearestDistance) {
                nearestDistance = (long)distance;
                nearest = it;
            }
        }
    }
    return *nearest;
}
static CScriptThing* Imp(float x, float y, float z, int id)
{ CScriptThing* p = new CScriptThing(); p->Pos.X = x; p->Pos.Y = y; p->Pos.Z = z; p->Id = id; return p; }
int main()
{
    CGameScriptInterface gsi; CCharString name; name.PStringData = 0;
    CScriptThing* me = Imp(0, 0, 0, 1);
    CScriptThing from = MakeHandle(me);
    g_impCount = 0;
    CScriptThing r0 = gsi.GetNearestWithScriptName(from, name);
    bool ok = r0.PImp.Data == 0;
    g_imps[0] = me; g_impCount = 1;
    CScriptThing r1 = gsi.GetNearestWithScriptName(from, name);
    ok = ok && r1.PImp.Data == me && me->Id == 1;
    g_imps[0] = Imp(10, 0, 0, 2); g_imps[1] = me; g_imps[2] = Imp(3, 4, 500, 3); g_imps[3] = Imp(-2, -2, 0, 4); g_impCount = 4;
    CScriptThing r2 = gsi.GetNearestWithScriptName(from, name);
    ok = ok && r2.PImp.Data && r2.PImp.Data->Id == 4;    // 8 < 25 < 100 (z ignored)
    g_imps[3] = Imp(3.0f, 4.4f, 0, 5); g_impCount = 4;    // 28.36 vs 25: id 3 stays nearest
    CScriptThing r3 = gsi.GetNearestWithScriptName(from, name);
    ok = ok && r3.PImp.Data && r3.PImp.Data->Id == 3;
    g_imps[0] = Imp(5.0f, 0.5f, 0, 6); g_imps[1] = Imp(5.0f, 0.2f, 0, 7); g_impCount = 2;   // 25.25 -> 25, then 25.04 < 25 is false
    CScriptThing r4 = gsi.GetNearestWithScriptName(from, name);
    ok = ok && r4.PImp.Data && r4.PImp.Data->Id == 6;
    if (!ok) { printf("0089a510_TEST FAIL\n"); return 1; }
    printf("0089a510_TEST PASS\n");
    return 0;
}
