// Behaviour test for 006cba50: src/006cba50.cpp copied verbatim, the position getter and navigator stubbed so the call and
// its arguments are observable. CTCCreatureNavigation::IsPosNavigable(const C3DVector&) const
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCreatureNavigation::IsPosNavigable(const C3DVector& pos) -- can the current navigator route from the thing's position to pos
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CBA50, length 38 bytes (+10 int3). Twin of the landed IsPosNavLineOfSightTo
// (0x006CBA20, slot 10).
// Callees: CThing::GetPosition @ 0x004C73D0, CNavigatorBase::CanNavigateToPos (virtual slot 9, `call [edi+24h]`) with
// proximity 0.0f (`push 0`) and `false`.
// Shape: the navigator's vtable is fetched before the position call (edi) and the object reloaded into ecx after it;
// the thing is reached through an inline GetParentThing() accessor -- with the plain member VC7.1 loads pos after the
// pushes, with the accessor it loads pos first (edx) and interleaves the pushes as retail.
// Model: CTCCreatureNavigation PDB names (PNavigator +0x0c, PossibleNavigators +0x10 as the retail 12-byte vector);
// navigator slots from the CNavigatorAStar/CNavigatorFlyer vtables (9 CanNavigateToPos, 10 IsNavLineOfSightClear,
// 18 Interrupt) and the external interface vtables (slot 4 FindPosZAt, float).

struct C3DVector { float X, Y, Z; };

struct CThing {
    const C3DVector& GetPosition() const;   // 0x004C73D0 (PhysicsTC +0x60, position at +0x0c)
};

struct CNavigatorExternalInterface {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual float FindPosZAt(const C3DVector& pos, bool flag) const;    // slot 4
};

struct CNavigatorBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool CanNavigateToPos(const C3DVector& from, const C3DVector& to, float proximity = 0.0f, bool flag = false) const;   // slot 9
    virtual bool IsNavLineOfSightClear(const C3DVector& from, const C3DVector& to) const;                          // slot 10
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17();
    virtual void Interrupt();               // slot 18
    unsigned char _pad04[0x0c];
    unsigned char NavigatorType;            // +0x10
    unsigned char _pad11[3];
    CNavigatorExternalInterface* PInterface;   // +0x14
};

struct CNavigatorVector {                   // retail 12-byte vector<CNavigatorBase*>
    CNavigatorBase** first;
    CNavigatorBase** last;
    CNavigatorBase** end;
};

struct CTCCreatureNavigation {
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    CNavigatorBase* PNavigator;             // +0x0c
    CNavigatorVector PossibleNavigators;    // +0x10
    bool IsPosNavigable(const C3DVector& pos) const;
    CNavigatorBase* GetNavigator() const { return PNavigator; }
    CThing* GetParentThing() const { return ParentThing; }
};

bool CTCCreatureNavigation::IsPosNavigable(const C3DVector& pos) const
{
    return PNavigator->CanNavigateToPos(GetParentThing()->GetPosition(), pos);
}

// ---- stubs ----
static C3DVector g_thingPos = { 1.0f, 2.0f, 3.0f };
static const CThing* g_posOf = 0;
const C3DVector& CThing::GetPosition() const { g_posOf = this; return g_thingPos; }
struct CTestNavigator : CNavigatorBase {
    const C3DVector* from; const C3DVector* to; float proximity; bool flag; int calls; bool answer;
    virtual bool CanNavigateToPos(const C3DVector& f, const C3DVector& t, float p, bool fl) const
    { CTestNavigator* self = (CTestNavigator*)this; ++self->calls; self->from = &f; self->to = &t; self->proximity = p; self->flag = fl; return answer; }
};
void CNavigatorBase::v00() {} void CNavigatorBase::v01() {} void CNavigatorBase::v02() {} void CNavigatorBase::v03() {}
void CNavigatorBase::v04() {} void CNavigatorBase::v05() {} void CNavigatorBase::v06() {} void CNavigatorBase::v07() {}
void CNavigatorBase::v08() {} void CNavigatorBase::v11() {} void CNavigatorBase::v12() {} void CNavigatorBase::v13() {}
void CNavigatorBase::v14() {} void CNavigatorBase::v15() {} void CNavigatorBase::v16() {} void CNavigatorBase::v17() {}
void CNavigatorBase::Interrupt() {}
bool CNavigatorBase::CanNavigateToPos(const C3DVector&, const C3DVector&, float, bool) const { return false; }
bool CNavigatorBase::IsNavLineOfSightClear(const C3DVector&, const C3DVector&) const { return false; }
int main()
{
    static CThing thing; static CTestNavigator nav; nav.calls = 0;
    CTCCreatureNavigation tc; tc.ParentThing = &thing; tc.PNavigator = &nav;
    C3DVector target = { 9.0f, 8.0f, 7.0f };
    nav.answer = true; nav.proximity = 5.0f; nav.flag = true;
    CHECK(tc.IsPosNavigable(target) == true);
    CHECK(nav.calls == 1 && g_posOf == &thing && nav.from == &g_thingPos && nav.to == &target);
    CHECK(nav.proximity == 0.0f && nav.flag == false);
    nav.answer = false;
    CHECK(tc.IsPosNavigable(target) == false); CHECK(nav.calls == 2);
    if (fails == 0) printf("006cba50_TEST PASS\n"); else printf("006cba50_TEST FAIL\n");
    return fails;
}