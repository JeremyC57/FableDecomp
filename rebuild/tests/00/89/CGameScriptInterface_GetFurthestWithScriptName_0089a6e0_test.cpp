#include <cstdio>
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
struct C3DVector { float X, Y, Z; };
extern const C3DVector g_ZeroVector;                    // 0x0143E8E0
struct CScriptThingImp {
    virtual void _i00();
    virtual void _i01();
    virtual void _i02();
    virtual void _i03();
    virtual void _i04();
    virtual void _i05();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18)
    virtual void _i07();
    virtual void _i08();
    virtual void _i09();
    virtual void _i10();
    virtual void _i11();
    virtual void _i12();
    virtual void _i13();
    virtual void _i14();
    virtual void _i15();
    virtual void _i16();
    virtual void _i17();
    virtual void _i18();
    virtual void _i19();
    virtual void _i20();
    virtual void _i21();
    virtual void _i22();
    virtual void _i23();
    virtual void _i24();
    virtual void _i25();
    virtual void _i26();
    virtual void _i27();
    virtual void _i28();
    virtual void _i29();
    virtual void _i30();
    virtual void _i31();
    virtual void _i32();
    virtual void _i33();
    virtual void _i34();
    virtual void _i35();
    virtual void _i36();
    virtual void _i37();
    virtual void _i38();
    virtual void _i39();
    virtual void _i40();
    virtual void _i41();
    virtual void _i42();
    virtual void _i43();
    virtual void _i44();
    virtual void _i45();
    virtual void _i46();
    virtual void _i47();
    virtual void _i48();
    virtual void _i49();
    virtual void _i50();
    virtual void _i51();
    virtual void _i52();
    virtual void _i53();
    virtual void _i54();
    virtual void _i55();
    virtual void _i56();
    virtual void _i57();
    virtual void _i58();
    virtual void _i59();
    virtual void _i60();
    virtual void _i61();
    virtual void _i62();
    virtual void _i63();
    virtual void _i64();
    virtual void _i65();
    virtual void _i66();
    virtual void _i67();
    virtual void _i68();
    virtual void _i69();
    virtual void _i70();
    virtual void _i71();
    virtual void _i72();
    virtual void _i73();
    virtual void _i74();
    virtual void _i75();
    virtual void _i76();
    virtual void _i77();
    virtual bool IsSameThing(const CScriptThingImp* other) const;   // slot 78 (+0x138)
};
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
    CScriptThingImp* Data;
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
struct CCharString { void* PStringData; };
struct CScriptThing : CBaseClass {                      // vtable 0x01238C8C
    CCountedScriptImp PImp;                             // +0x04
    CScriptThing() {}
    virtual ~CScriptThing() {}
    virtual bool IsDead() const;
    virtual const CCharString& GetName() const;
    virtual void _slot3();
    virtual void _slot4();
    virtual void _slot5();
    virtual const C3DVector& GetPos() const { return PImp.Data == 0 ? g_ZeroVector : PImp.Data->GetPos(); }
    bool operator==(const CScriptThing& other) const { return PImp.Data->IsSameThing(other.PImp.Data); }
};
__forceinline void _Destroy(CScriptThing* first, CScriptThing* last)
{
    for (; first != last; ++first)
        first->~CScriptThing();
}
struct CScriptThingVector {                             // STLport vector<CScriptThing> (malloc allocator)
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    CScriptThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    __forceinline ~CScriptThingVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start) free(_M_start);
    }
    bool empty() const { return _M_start == _M_finish; }
};
struct CGameScriptInterface {
    virtual void _g00();
    virtual void _g01();
    virtual void _g02();
    virtual void _g03();
    virtual void _g04();
    virtual void _g05();
    virtual void _g06();
    virtual void _g07();
    virtual void _g08();
    virtual void _g09();
    virtual void _g10();
    virtual void _g11();
    virtual void _g12();
    virtual void _g13();
    virtual void _g14();
    virtual void _g15();
    virtual void _g16();
    virtual void _g17();
    virtual void _g18();
    virtual void _g19();
    virtual void _g20();
    virtual void _g21();
    virtual void _g22();
    virtual void _g23();
    virtual void _g24();
    virtual void _g25();
    virtual void _g26();
    virtual void _g27();
    virtual void _g28();
    virtual void _g29();
    virtual void _g30();
    virtual void _g31();
    virtual void _g32();
    virtual void _g33();
    virtual void _g34();
    virtual void _g35();
    virtual void _g36();
    virtual void _g37();
    virtual void _g38();
    virtual void _g39();
    virtual void _g40();
    virtual void _g41();
    virtual void _g42();
    virtual void _g43();
    virtual void _g44();
    virtual void _g45();
    virtual void _g46();
    virtual void _g47();
    virtual void _g48();
    virtual void _g49();
    virtual void _g50();
    virtual void _g51();
    virtual void _g52();
    virtual void _g53();
    virtual void _g54();
    virtual void _g55();
    virtual void _g56();
    virtual void _g57();
    virtual void _g58();
    virtual void _g59();
    virtual void _g60();
    virtual void _g61();
    virtual void _g62();
    virtual void _g63();
    virtual void _g64();
    virtual void _g65();
    virtual void _g66();
    virtual void _g67();
    virtual void _g68();
    virtual void _g69();
    virtual void _g70();
    virtual void _g71();
    virtual void _g72();
    virtual void _g73();
    virtual void _g74();
    virtual long GetAllThingsWithScriptName(const CCharString& name, CScriptThingVector& things) const;   // slot 75 (+0x12c)
    virtual CScriptThing GetFurthestWithScriptName(const CScriptThing& from, const CCharString& name) const;
};

CScriptThing CGameScriptInterface::GetFurthestWithScriptName(const CScriptThing& from, const CCharString& name) const
{
    CScriptThingVector things;
    GetAllThingsWithScriptName(name, things);
    if (things.empty())
        return CScriptThing();
    long bestDistance = 0;
    CScriptThing* best = things._M_start;
    for (CScriptThing* it = things._M_start; it != things._M_finish; ++it) {
        CScriptThing thing = *it;
        const C3DVector& pos = thing.GetPos();
        const C3DVector& fromPos = from.GetPos();
        float dx = pos.X - fromPos.X;
        float dy = pos.Y - fromPos.Y;
        float distance = dx * dx + dy * dy;
        if (distance > bestDistance) {
            bestDistance = (long)distance;
            best = it;
        }
    }
    return *best;
}

void CScriptThingImp::_i00() {}
void CScriptThingImp::_i01() {}
void CScriptThingImp::_i02() {}
void CScriptThingImp::_i03() {}
void CScriptThingImp::_i04() {}
void CScriptThingImp::_i05() {}
void CScriptThingImp::_i07() {}
void CScriptThingImp::_i08() {}
void CScriptThingImp::_i09() {}
void CScriptThingImp::_i10() {}
void CScriptThingImp::_i11() {}
void CScriptThingImp::_i12() {}
void CScriptThingImp::_i13() {}
void CScriptThingImp::_i14() {}
void CScriptThingImp::_i15() {}
void CScriptThingImp::_i16() {}
void CScriptThingImp::_i17() {}
void CScriptThingImp::_i18() {}
void CScriptThingImp::_i19() {}
void CScriptThingImp::_i20() {}
void CScriptThingImp::_i21() {}
void CScriptThingImp::_i22() {}
void CScriptThingImp::_i23() {}
void CScriptThingImp::_i24() {}
void CScriptThingImp::_i25() {}
void CScriptThingImp::_i26() {}
void CScriptThingImp::_i27() {}
void CScriptThingImp::_i28() {}
void CScriptThingImp::_i29() {}
void CScriptThingImp::_i30() {}
void CScriptThingImp::_i31() {}
void CScriptThingImp::_i32() {}
void CScriptThingImp::_i33() {}
void CScriptThingImp::_i34() {}
void CScriptThingImp::_i35() {}
void CScriptThingImp::_i36() {}
void CScriptThingImp::_i37() {}
void CScriptThingImp::_i38() {}
void CScriptThingImp::_i39() {}
void CScriptThingImp::_i40() {}
void CScriptThingImp::_i41() {}
void CScriptThingImp::_i42() {}
void CScriptThingImp::_i43() {}
void CScriptThingImp::_i44() {}
void CScriptThingImp::_i45() {}
void CScriptThingImp::_i46() {}
void CScriptThingImp::_i47() {}
void CScriptThingImp::_i48() {}
void CScriptThingImp::_i49() {}
void CScriptThingImp::_i50() {}
void CScriptThingImp::_i51() {}
void CScriptThingImp::_i52() {}
void CScriptThingImp::_i53() {}
void CScriptThingImp::_i54() {}
void CScriptThingImp::_i55() {}
void CScriptThingImp::_i56() {}
void CScriptThingImp::_i57() {}
void CScriptThingImp::_i58() {}
void CScriptThingImp::_i59() {}
void CScriptThingImp::_i60() {}
void CScriptThingImp::_i61() {}
void CScriptThingImp::_i62() {}
void CScriptThingImp::_i63() {}
void CScriptThingImp::_i64() {}
void CScriptThingImp::_i65() {}
void CScriptThingImp::_i66() {}
void CScriptThingImp::_i67() {}
void CScriptThingImp::_i68() {}
void CScriptThingImp::_i69() {}
void CScriptThingImp::_i70() {}
void CScriptThingImp::_i71() {}
void CScriptThingImp::_i72() {}
void CScriptThingImp::_i73() {}
void CScriptThingImp::_i74() {}
void CScriptThingImp::_i75() {}
void CScriptThingImp::_i76() {}
void CScriptThingImp::_i77() {}
void CGameScriptInterface::_g00() {}
void CGameScriptInterface::_g01() {}
void CGameScriptInterface::_g02() {}
void CGameScriptInterface::_g03() {}
void CGameScriptInterface::_g04() {}
void CGameScriptInterface::_g05() {}
void CGameScriptInterface::_g06() {}
void CGameScriptInterface::_g07() {}
void CGameScriptInterface::_g08() {}
void CGameScriptInterface::_g09() {}
void CGameScriptInterface::_g10() {}
void CGameScriptInterface::_g11() {}
void CGameScriptInterface::_g12() {}
void CGameScriptInterface::_g13() {}
void CGameScriptInterface::_g14() {}
void CGameScriptInterface::_g15() {}
void CGameScriptInterface::_g16() {}
void CGameScriptInterface::_g17() {}
void CGameScriptInterface::_g18() {}
void CGameScriptInterface::_g19() {}
void CGameScriptInterface::_g20() {}
void CGameScriptInterface::_g21() {}
void CGameScriptInterface::_g22() {}
void CGameScriptInterface::_g23() {}
void CGameScriptInterface::_g24() {}
void CGameScriptInterface::_g25() {}
void CGameScriptInterface::_g26() {}
void CGameScriptInterface::_g27() {}
void CGameScriptInterface::_g28() {}
void CGameScriptInterface::_g29() {}
void CGameScriptInterface::_g30() {}
void CGameScriptInterface::_g31() {}
void CGameScriptInterface::_g32() {}
void CGameScriptInterface::_g33() {}
void CGameScriptInterface::_g34() {}
void CGameScriptInterface::_g35() {}
void CGameScriptInterface::_g36() {}
void CGameScriptInterface::_g37() {}
void CGameScriptInterface::_g38() {}
void CGameScriptInterface::_g39() {}
void CGameScriptInterface::_g40() {}
void CGameScriptInterface::_g41() {}
void CGameScriptInterface::_g42() {}
void CGameScriptInterface::_g43() {}
void CGameScriptInterface::_g44() {}
void CGameScriptInterface::_g45() {}
void CGameScriptInterface::_g46() {}
void CGameScriptInterface::_g47() {}
void CGameScriptInterface::_g48() {}
void CGameScriptInterface::_g49() {}
void CGameScriptInterface::_g50() {}
void CGameScriptInterface::_g51() {}
void CGameScriptInterface::_g52() {}
void CGameScriptInterface::_g53() {}
void CGameScriptInterface::_g54() {}
void CGameScriptInterface::_g55() {}
void CGameScriptInterface::_g56() {}
void CGameScriptInterface::_g57() {}
void CGameScriptInterface::_g58() {}
void CGameScriptInterface::_g59() {}
void CGameScriptInterface::_g60() {}
void CGameScriptInterface::_g61() {}
void CGameScriptInterface::_g62() {}
void CGameScriptInterface::_g63() {}
void CGameScriptInterface::_g64() {}
void CGameScriptInterface::_g65() {}
void CGameScriptInterface::_g66() {}
void CGameScriptInterface::_g67() {}
void CGameScriptInterface::_g68() {}
void CGameScriptInterface::_g69() {}
void CGameScriptInterface::_g70() {}
void CGameScriptInterface::_g71() {}
void CGameScriptInterface::_g72() {}
void CGameScriptInterface::_g73() {}
void CGameScriptInterface::_g74() {}
#include <stdlib.h>
#include <new>
const C3DVector& CScriptThingImp::GetPos() const { static C3DVector v = {0, 0, 0}; return v; }
bool CScriptThingImp::IsSameThing(const CScriptThingImp* other) const { return this == other; }
struct CTestImp : CScriptThingImp {
    C3DVector Pos;
    const C3DVector& GetPos() const { return Pos; }
    bool IsSameThing(const CScriptThingImp* other) const { return this == other; }
};
const C3DVector g_ZeroVector = {0, 0, 0};
static int g_baseCtors, g_baseDtors;
CBaseClass::CBaseClass() { ++g_baseCtors; }
CBaseClass::~CBaseClass() { ++g_baseDtors; }
bool CScriptThing::IsDead() const { return false; }
const CCharString& CScriptThing::GetName() const { static CCharString n; return n; }
void CScriptThing::_slot3() {} void CScriptThing::_slot4() {} void CScriptThing::_slot5() {}
static void __fastcall NoDelete(void*) {}
static CTestImp g_imps[4];
static int g_count;
static const CCharString* g_seenName;
long CGameScriptInterface::GetAllThingsWithScriptName(const CCharString& name, CScriptThingVector& things) const
{
    g_seenName = &name;
    if (g_count == 0) return 0;
    things._M_start = (CScriptThing*)malloc(sizeof(CScriptThing) * g_count);
    for (int i = 0; i < g_count; ++i) {
        CScriptThing* t = new (things._M_start + i) CScriptThing();
        t->PImp.Data = &g_imps[i];
        t->PImp.Info = new CCPPointerInfo;
        t->PImp.Info->RefCount = 1; t->PImp.Info->DeleteFunc = NoDelete; t->PImp.Info->Data = &g_imps[i];
    }
    things._M_finish = things._M_end_of_storage = things._M_start + g_count;
    return g_count;
}
int main()
{
    CGameScriptInterface gsi; CCharString name; name.PStringData = 0;
    CTestImp fromImp; fromImp.Pos.X = 1; fromImp.Pos.Y = 1; fromImp.Pos.Z = 50;
    CScriptThing from; from.PImp.Data = &fromImp;
    bool ok = true;
    g_count = 0;
    { CScriptThing r = gsi.GetFurthestWithScriptName(from, name); ok = ok && r.PImp.Data == 0 && r.PImp.Info == 0 && g_seenName == &name; }
    g_imps[0].Pos.X = 2; g_imps[0].Pos.Y = 1; g_imps[0].Pos.Z = 0;     // d2 = 1
    g_imps[1].Pos.X = 1; g_imps[1].Pos.Y = 5; g_imps[1].Pos.Z = -900;  // d2 = 16
    g_imps[2].Pos.X = 4; g_imps[2].Pos.Y = 1; g_imps[2].Pos.Z = 0;     // d2 = 9
    g_count = 3;
    { CScriptThing r = gsi.GetFurthestWithScriptName(from, name); ok = ok && r.PImp.Data == &g_imps[1] && r.PImp.Info != 0 && r.PImp.Info->RefCount == 1; }
    g_imps[0].Pos.X = 1; g_imps[0].Pos.Y = 1;                          // all at distance 0 -> first element
    g_imps[1].Pos.X = 1; g_imps[1].Pos.Y = 1;
    g_imps[2].Pos.X = 1; g_imps[2].Pos.Y = 1;
    { CScriptThing r = gsi.GetFurthestWithScriptName(from, name); ok = ok && r.PImp.Data == &g_imps[0]; }
    from.PImp.Data = 0;
    std::printf(ok ? "0089a6e0_TEST PASS\n" : "0089a6e0_TEST FAIL\n");
    return ok ? 0 : 1;
}