// Behaviour test for 00d9ec50: src/00d9ec50.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CQ_MinionCampScript::IsAssignedAsHeroAttacker(const CScriptThing& thing)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CQ_MinionCampScript::IsAssignedAsHeroAttacker(const CScriptThing& thing) -- true when `thing` is one of the two live hero attackers (HeroAttackerOne / HeroAttackerTwo) and the script thread is not terminating
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D9EC50, length 147 bytes (padded to 160), `ret 4`.
// Callees: CScriptThing virtual slots 77 IsNull (+0x134; 0x004AB150), 75 IsAlive (+0x12c; 0x004AB130), and 78 IsEqualTo
// (+0x138; 0x004AA8A0) called on the argument's implementation object (PImp_Data, +4) with the attacker's PImp_Data;
// CScriptBase::IsActiveThreadTerminating @ 0x00CB7940.
// Layout (retail = Ego_r PDB offsets -0x14): HeroAttackerOne +0x78, HeroAttackerTwo +0x84 (CScriptThing, 12 bytes:
// vptr, PImp = CCountedPointer<CScriptThing> {Data +4, Info +8} as in the PDB). engine/CScriptThing.h flattens the
// vptr to a `void*` and PImp to two words, so the virtual calls need this local model.
// Shape: one `||` of two `&&` triples; the comparison is an inline CScriptThing::operator== forwarding to the
// implementation's IsEqualTo through CCountedPointer's inline operator-> / operator* (`PImp->IsEqualTo(&*other.PImp)`;
// reading `.Data` directly swaps eax/edx in the IsAlive and second IsNull vtable loads); the true path is `IsActiveThreadTerminating() ? false : true` (8-bit neg/sbb/inc).
template <class T> struct CCountedPointer {       // CCountedPointer<T> (8 bytes)
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
};
struct CScriptThing {                              // vtable 0x01238C8C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
    virtual void v076();
    virtual bool IsNull() const;   // slot 77 (+0x134)
    virtual bool IsEqualTo(const CScriptThing* other) const;   // slot 78 (+0x138)
    CCountedPointer<CScriptThing> PImp;            // +0x04 (Data +4, Info +8)
    bool operator==(const CScriptThing& other) const { return PImp->IsEqualTo(&*other.PImp); }
};
struct CScriptBase {
    bool IsActiveThreadTerminating();              // 0x00CB7940
};
struct CQ_MinionCampScript : CScriptBase {
    unsigned char _pad_0x00[0x78];
    CScriptThing  HeroAttackerOne;                 // +0x78
    CScriptThing  HeroAttackerTwo;                 // +0x84
    bool IsAssignedAsHeroAttacker(const CScriptThing& thing);
};
bool CQ_MinionCampScript::IsAssignedAsHeroAttacker(const CScriptThing& thing)
{
    if ((!HeroAttackerOne.IsNull() && thing == HeroAttackerOne && HeroAttackerOne.IsAlive())
        || (!HeroAttackerTwo.IsNull() && thing == HeroAttackerTwo && HeroAttackerTwo.IsAlive()))
        return IsActiveThreadTerminating() ? false : true;
    return false;
}

// ---- stubs and driver ----
#include <new>
void CScriptThing::v000() { }
void CScriptThing::v001() { }
void CScriptThing::v002() { }
void CScriptThing::v003() { }
void CScriptThing::v004() { }
void CScriptThing::v005() { }
void CScriptThing::v006() { }
void CScriptThing::v007() { }
void CScriptThing::v008() { }
void CScriptThing::v009() { }
void CScriptThing::v010() { }
void CScriptThing::v011() { }
void CScriptThing::v012() { }
void CScriptThing::v013() { }
void CScriptThing::v014() { }
void CScriptThing::v015() { }
void CScriptThing::v016() { }
void CScriptThing::v017() { }
void CScriptThing::v018() { }
void CScriptThing::v019() { }
void CScriptThing::v020() { }
void CScriptThing::v021() { }
void CScriptThing::v022() { }
void CScriptThing::v023() { }
void CScriptThing::v024() { }
void CScriptThing::v025() { }
void CScriptThing::v026() { }
void CScriptThing::v027() { }
void CScriptThing::v028() { }
void CScriptThing::v029() { }
void CScriptThing::v030() { }
void CScriptThing::v031() { }
void CScriptThing::v032() { }
void CScriptThing::v033() { }
void CScriptThing::v034() { }
void CScriptThing::v035() { }
void CScriptThing::v036() { }
void CScriptThing::v037() { }
void CScriptThing::v038() { }
void CScriptThing::v039() { }
void CScriptThing::v040() { }
void CScriptThing::v041() { }
void CScriptThing::v042() { }
void CScriptThing::v043() { }
void CScriptThing::v044() { }
void CScriptThing::v045() { }
void CScriptThing::v046() { }
void CScriptThing::v047() { }
void CScriptThing::v048() { }
void CScriptThing::v049() { }
void CScriptThing::v050() { }
void CScriptThing::v051() { }
void CScriptThing::v052() { }
void CScriptThing::v053() { }
void CScriptThing::v054() { }
void CScriptThing::v055() { }
void CScriptThing::v056() { }
void CScriptThing::v057() { }
void CScriptThing::v058() { }
void CScriptThing::v059() { }
void CScriptThing::v060() { }
void CScriptThing::v061() { }
void CScriptThing::v062() { }
void CScriptThing::v063() { }
void CScriptThing::v064() { }
void CScriptThing::v065() { }
void CScriptThing::v066() { }
void CScriptThing::v067() { }
void CScriptThing::v068() { }
void CScriptThing::v069() { }
void CScriptThing::v070() { }
void CScriptThing::v071() { }
void CScriptThing::v072() { }
void CScriptThing::v073() { }
void CScriptThing::v074() { }
void CScriptThing::v076() { }
bool CScriptThing::IsAlive() const { return false; }
bool CScriptThing::IsNull() const { return true; }
bool CScriptThing::IsEqualTo(const CScriptThing*) const { return false; }
// implementation objects: identity compare, configurable null/alive flags
struct CTestImp : CScriptThing {
    bool nullFlag, aliveFlag;
    virtual bool IsAlive() const { return aliveFlag; }
    virtual bool IsNull() const { return nullFlag; }
    virtual bool IsEqualTo(const CScriptThing* other) const { return other == this; }
};
// wrapper things: forward IsNull/IsAlive to their implementation
struct CTestThing : CScriptThing {
    virtual bool IsAlive() const { return PImp->IsAlive(); }
    virtual bool IsNull() const { return PImp->IsNull(); }
};
static bool g_terminating = false; static int g_termCalls = 0;
bool CScriptBase::IsActiveThreadTerminating() { ++g_termCalls; return g_terminating; }
template <class T> static void Construct(T* p) { new (p) T(); }
int main()
{
    static CTestImp impA, impB, impC; CTestImp* imps[3] = { &impA, &impB, &impC };
    for (int i = 0; i < 3; ++i) { new (imps[i]) CTestImp(); imps[i]->nullFlag = false; imps[i]->aliveFlag = true; }
    static long storage[0x40];
    CQ_MinionCampScript* s = (CQ_MinionCampScript*)storage;
    new (&s->HeroAttackerOne) CTestThing(); new (&s->HeroAttackerTwo) CTestThing();
    s->HeroAttackerOne.PImp.Data = &impA; s->HeroAttackerTwo.PImp.Data = &impB;
    static CTestThing thingA, thingB, thingC;
    new (&thingA) CTestThing(); new (&thingB) CTestThing(); new (&thingC) CTestThing();
    thingA.PImp.Data = &impA; thingB.PImp.Data = &impB; thingC.PImp.Data = &impC;
    CHECK(s->IsAssignedAsHeroAttacker(thingA) == true);
    CHECK(s->IsAssignedAsHeroAttacker(thingB) == true);
    CHECK(s->IsAssignedAsHeroAttacker(thingC) == false);
    impA.aliveFlag = false;
    CHECK(s->IsAssignedAsHeroAttacker(thingA) == false);
    impA.aliveFlag = true; impA.nullFlag = true;
    CHECK(s->IsAssignedAsHeroAttacker(thingA) == false);
    impA.nullFlag = false;
    g_terminating = true;
    int t0 = g_termCalls;
    CHECK(s->IsAssignedAsHeroAttacker(thingB) == false && g_termCalls == t0 + 1);
    CHECK(s->IsAssignedAsHeroAttacker(thingC) == false && g_termCalls == t0 + 1);
    if (fails == 0) printf("00d9ec50_TEST PASS\n");
    return fails;
}
