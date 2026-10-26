// Behaviour test for 00726be0: src/00726be0.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CTCPhysicsNavigator::UpdateAdditionalImpulse()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCPhysicsNavigator::UpdateAdditionalImpulse() -- clear the additional impulse; when the feature is on and the owner creature is busy with an action, pushable and pushable by self-avoidance, gather the other things in the area it could reach this frame and let each one that is in combat and too close add to its impulse
// Leaf: ?UpdateAdditionalImpulse@CTCPhysicsNavigator@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00726BE0, 254 bytes, plain `ret`.
// Callees: CThingCreatureBase::IsPerformingAnyAction() const 0x006622A0, the physics TC's virtual slot 83 (+0x14c,
//   IsPushableByHero as the landed 0x00715710), NCreatureMode::IsPhysicsPushableBySelfAvoidance(const CThing&) 0x0085FE60
//   (__fastcall), CTCPhysicsNavigator::GetPossibleCollidedWithAreaWithImpulse 0x007267C0 (landed), CTCBase::
//   PeekThingSearchTools() const 0x006AFD40, CThingSearchTools::GetAllThingsInRoughArea<CThingFilter_IsNot> 0x00725F10,
//   std::_For_each<CThingCreatureBase**, CApplyAdditionalImpulseIfInCombatAndTooClose> 0x00727500 (__fastcall, hidden
//   result in ecx, first in edx, the 0x18-byte functor by value), free 0x00BFEA14.
// Globals: the additional-impulse enable byte 0x0137574A (name descriptive).
// Layout: Ego_r.pdb CTCPhysicsNavigator (pack 1: AdditionalImpulse +0x95); CTCPhysicsBase Position +0x0c, Radius +0x34;
//   CThing physics TC +0x60; CApplyAdditionalImpulseIfInCombatAndTooClose {Position, Radius, Owner, OwnerCreature}.
// Shape: the impulse is cleared by a chained `X = Y = Z = 0` (Z stored first, one zero register); the functor is built in
//   place by an inline (position, float radius, owner) ctor that takes the owner creature from owner->ParentThing; the
//   C3DVector copy is memberwise; GetAllThingsInRoughArea has a same-TU stand-in (as the landed 0x0092E950) so the result
//   vector does not escape and its start stays in esi for the freeing destructor.
class CThing;
class CThingCreatureBase;
class CTCPhysicsBase;

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class C2DCoordI { public: long X, Y; };
class C2DBoxI { public:
    C2DCoordI TopLeft;
    C2DCoordI BottomRight;
};
template <class T> struct vector {                  // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
};
extern "C" void __cdecl free(void* block);          // 0x00BFEA14

class CThingFilter_IsNot { public:
    CThing* Thing;
    CThingFilter_IsNot(CThing* thing) : Thing(thing) { }
};
class CThingSearchTools { public:
    bool GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_IsNot& filter, vector<CThingCreatureBase*>& out);   // 0x00725F10
};

class CTCPhysicsBaseVirtuals {
public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual bool IsPushableByHero();                // slot 83 (+0x14c)
};
class CThingCreatureBase {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBaseVirtuals* PhysicsTC;              // +0x60
    bool IsPerformingAnyAction() const;             // 0x006622A0
};
namespace NCreatureMode {
bool __fastcall IsPhysicsPushableBySelfAvoidance(const CThing& thing);   // 0x0085FE60
}

#pragma pack(push, 1)
class CTCPhysicsBase {
public:
    virtual void v00();
    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    C3DVector     Position;                         // +0x0c
    unsigned char _pad18[0x34 - 0x18];
    float         Radius;                           // +0x34
};
#pragma pack(pop)
class CApplyAdditionalImpulseIfInCombatAndTooClose { public:   // 0x18 bytes
    C3DVector           Position;                   // +0x00
    float               Radius;                     // +0x0c
    CTCPhysicsBase*     Owner;                      // +0x10
    CThingCreatureBase* OwnerCreature;              // +0x14
    __forceinline CApplyAdditionalImpulseIfInCombatAndTooClose(const C3DVector& position, float radius, CTCPhysicsBase* owner)
        : Position(position), Radius(radius), Owner(owner),
          OwnerCreature((CThingCreatureBase*)owner->ParentThing) { }
};
namespace std {
template <class TIterator, class TFunction>
TFunction __fastcall _For_each(TIterator first, TIterator last, TFunction function);   // 0x00727500 (this instantiation)
}

extern bool g_AdditionalImpulseEnabled;             // 0x0137574A

#pragma pack(push, 1)
class CTCPhysicsNavigator : public CTCPhysicsBase {
public:
    unsigned char _pad38[0x95 - 0x38];
    C3DVector     AdditionalImpulse;                // +0x95
    const CThingSearchTools* PeekThingSearchTools() const;   // 0x006AFD40 (CTCBase)
    C2DBoxI GetPossibleCollidedWithAreaWithImpulse(const C3DVector& impulse) const;   // 0x007267C0
    void UpdateAdditionalImpulse();
};
#pragma pack(pop)

bool SearchRoughArea_00725f10(CThingSearchTools* tools, const C2DBoxI* area, const CThingFilter_IsNot* filter,
                              CThingCreatureBase** first, CThingCreatureBase** last, CThingCreatureBase** end);
CThingCreatureBase** SearchResultFirst_00725f10();
CThingCreatureBase** SearchResultLast_00725f10();
CThingCreatureBase** SearchResultEnd_00725f10();
// Same-TU stand-in for the GetAllThingsInRoughArea<IsNot> instantiation 0x00725F10 (not a reconstruction of it, as the
// landed 0x0092E950): the area and filter go by address, but the result vector's three words are read and rewritten by
// value, so VC7.1 knows the vector does not escape and keeps its start in a register across the for_each call.
#pragma auto_inline(off)
__declspec(noinline) bool CThingSearchTools::GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_IsNot& filter,
                                                                     vector<CThingCreatureBase*>& out)
{
    bool found = SearchRoughArea_00725f10(this, &area, &filter, out._M_start, out._M_finish, out._M_end_of_storage);
    out._M_start = SearchResultFirst_00725f10();
    out._M_finish = SearchResultLast_00725f10();
    out._M_end_of_storage = SearchResultEnd_00725f10();
    return found;
}
#pragma auto_inline(on)

void CTCPhysicsNavigator::UpdateAdditionalImpulse()
{
    AdditionalImpulse.X = AdditionalImpulse.Y = AdditionalImpulse.Z = 0.0f;
    if (g_AdditionalImpulseEnabled) {
        CThingCreatureBase* creature = (CThingCreatureBase*)ParentThing;
        if (creature->IsPerformingAnyAction() && creature->PhysicsTC->IsPushableByHero()
            && NCreatureMode::IsPhysicsPushableBySelfAvoidance(*(CThing*)creature)) {
            vector<CThingCreatureBase*> creatures;
            C2DBoxI area = GetPossibleCollidedWithAreaWithImpulse(C3DVector(1.0f, 1.0f, 0.0f));
            ((CThingSearchTools*)PeekThingSearchTools())->GetAllThingsInRoughArea(area, CThingFilter_IsNot(ParentThing), creatures);
            std::_For_each(creatures.begin(), creatures.end(),
                           CApplyAdditionalImpulseIfInCombatAndTooClose(Position, Radius, this));
        }
    }
}

// ---- stubs ----
static char g_log[512];
static int g_step = 0;
static void Log(const char* s) { char b[64]; _snprintf(b, sizeof b, "%d:%s;", ++g_step, s); strcat(g_log, b); }
void CTCPhysicsBaseVirtuals::v000() {}
void CTCPhysicsBaseVirtuals::v001() {}
void CTCPhysicsBaseVirtuals::v002() {}
void CTCPhysicsBaseVirtuals::v003() {}
void CTCPhysicsBaseVirtuals::v004() {}
void CTCPhysicsBaseVirtuals::v005() {}
void CTCPhysicsBaseVirtuals::v006() {}
void CTCPhysicsBaseVirtuals::v007() {}
void CTCPhysicsBaseVirtuals::v008() {}
void CTCPhysicsBaseVirtuals::v009() {}
void CTCPhysicsBaseVirtuals::v010() {}
void CTCPhysicsBaseVirtuals::v011() {}
void CTCPhysicsBaseVirtuals::v012() {}
void CTCPhysicsBaseVirtuals::v013() {}
void CTCPhysicsBaseVirtuals::v014() {}
void CTCPhysicsBaseVirtuals::v015() {}
void CTCPhysicsBaseVirtuals::v016() {}
void CTCPhysicsBaseVirtuals::v017() {}
void CTCPhysicsBaseVirtuals::v018() {}
void CTCPhysicsBaseVirtuals::v019() {}
void CTCPhysicsBaseVirtuals::v020() {}
void CTCPhysicsBaseVirtuals::v021() {}
void CTCPhysicsBaseVirtuals::v022() {}
void CTCPhysicsBaseVirtuals::v023() {}
void CTCPhysicsBaseVirtuals::v024() {}
void CTCPhysicsBaseVirtuals::v025() {}
void CTCPhysicsBaseVirtuals::v026() {}
void CTCPhysicsBaseVirtuals::v027() {}
void CTCPhysicsBaseVirtuals::v028() {}
void CTCPhysicsBaseVirtuals::v029() {}
void CTCPhysicsBaseVirtuals::v030() {}
void CTCPhysicsBaseVirtuals::v031() {}
void CTCPhysicsBaseVirtuals::v032() {}
void CTCPhysicsBaseVirtuals::v033() {}
void CTCPhysicsBaseVirtuals::v034() {}
void CTCPhysicsBaseVirtuals::v035() {}
void CTCPhysicsBaseVirtuals::v036() {}
void CTCPhysicsBaseVirtuals::v037() {}
void CTCPhysicsBaseVirtuals::v038() {}
void CTCPhysicsBaseVirtuals::v039() {}
void CTCPhysicsBaseVirtuals::v040() {}
void CTCPhysicsBaseVirtuals::v041() {}
void CTCPhysicsBaseVirtuals::v042() {}
void CTCPhysicsBaseVirtuals::v043() {}
void CTCPhysicsBaseVirtuals::v044() {}
void CTCPhysicsBaseVirtuals::v045() {}
void CTCPhysicsBaseVirtuals::v046() {}
void CTCPhysicsBaseVirtuals::v047() {}
void CTCPhysicsBaseVirtuals::v048() {}
void CTCPhysicsBaseVirtuals::v049() {}
void CTCPhysicsBaseVirtuals::v050() {}
void CTCPhysicsBaseVirtuals::v051() {}
void CTCPhysicsBaseVirtuals::v052() {}
void CTCPhysicsBaseVirtuals::v053() {}
void CTCPhysicsBaseVirtuals::v054() {}
void CTCPhysicsBaseVirtuals::v055() {}
void CTCPhysicsBaseVirtuals::v056() {}
void CTCPhysicsBaseVirtuals::v057() {}
void CTCPhysicsBaseVirtuals::v058() {}
void CTCPhysicsBaseVirtuals::v059() {}
void CTCPhysicsBaseVirtuals::v060() {}
void CTCPhysicsBaseVirtuals::v061() {}
void CTCPhysicsBaseVirtuals::v062() {}
void CTCPhysicsBaseVirtuals::v063() {}
void CTCPhysicsBaseVirtuals::v064() {}
void CTCPhysicsBaseVirtuals::v065() {}
void CTCPhysicsBaseVirtuals::v066() {}
void CTCPhysicsBaseVirtuals::v067() {}
void CTCPhysicsBaseVirtuals::v068() {}
void CTCPhysicsBaseVirtuals::v069() {}
void CTCPhysicsBaseVirtuals::v070() {}
void CTCPhysicsBaseVirtuals::v071() {}
void CTCPhysicsBaseVirtuals::v072() {}
void CTCPhysicsBaseVirtuals::v073() {}
void CTCPhysicsBaseVirtuals::v074() {}
void CTCPhysicsBaseVirtuals::v075() {}
void CTCPhysicsBaseVirtuals::v076() {}
void CTCPhysicsBaseVirtuals::v077() {}
void CTCPhysicsBaseVirtuals::v078() {}
void CTCPhysicsBaseVirtuals::v079() {}
void CTCPhysicsBaseVirtuals::v080() {}
void CTCPhysicsBaseVirtuals::v081() {}
void CTCPhysicsBaseVirtuals::v082() {}

static bool g_pushable = true;
bool CTCPhysicsBaseVirtuals::IsPushableByHero() { Log("Pushable"); return g_pushable; }
static bool g_busy = true;
bool CThingCreatureBase::IsPerformingAnyAction() const { Log("Busy"); return g_busy; }
static bool g_selfAvoid = true; static const CThing* g_selfAvoidThing = 0;
bool __fastcall NCreatureMode::IsPhysicsPushableBySelfAvoidance(const CThing& thing) { Log("SelfAvoid"); g_selfAvoidThing = &thing; return g_selfAvoid; }
bool g_AdditionalImpulseEnabled = true;
void CTCPhysicsBase::v00() {}
static C3DVector g_impulse;
C2DBoxI CTCPhysicsNavigator::GetPossibleCollidedWithAreaWithImpulse(const C3DVector& impulse) const
{ Log("Area"); g_impulse = impulse; C2DBoxI b; b.TopLeft.X = 1; b.TopLeft.Y = 2; b.BottomRight.X = 3; b.BottomRight.Y = 4; return b; }
static CThingSearchTools g_tools;
const CThingSearchTools* CTCPhysicsNavigator::PeekThingSearchTools() const { Log("Tools"); return &g_tools; }
static C2DBoxI g_area; static CThing* g_notThing = 0; static int g_nFound = 2; static CThingCreatureBase** g_block = 0;
static CThingCreatureBase g_c1, g_c2;
bool SearchRoughArea_00725f10(CThingSearchTools* tools, const C2DBoxI* area, const CThingFilter_IsNot* filter,
                              CThingCreatureBase** first, CThingCreatureBase** last, CThingCreatureBase** end)
{
    Log("Search"); g_area = *area; g_notThing = filter->Thing; CHECK(first == 0 && last == 0 && end == 0 && tools == &g_tools);
    g_block = g_nFound ? (CThingCreatureBase**)malloc(g_nFound * sizeof(CThingCreatureBase*)) : 0;
    if (g_nFound > 0) g_block[0] = &g_c1;
    if (g_nFound > 1) g_block[1] = &g_c2;
    return g_nFound != 0;
}
CThingCreatureBase** SearchResultFirst_00725f10() { return g_block; }
CThingCreatureBase** SearchResultLast_00725f10() { return g_block ? g_block + g_nFound : 0; }
CThingCreatureBase** SearchResultEnd_00725f10() { return g_block ? g_block + g_nFound : 0; }
static int g_applied = 0; static CThingCreatureBase* g_seen[4]; static CApplyAdditionalImpulseIfInCombatAndTooClose* g_fn = 0;
static C3DVector g_fnPos; static float g_fnRadius; static CTCPhysicsBase* g_fnOwner; static CThingCreatureBase* g_fnCreature;
namespace std {
template <class TIterator, class TFunction>
TFunction __fastcall _For_each(TIterator first, TIterator last, TFunction function)
{
    Log("ForEach");
    g_fnPos = function.Position; g_fnRadius = function.Radius; g_fnOwner = function.Owner; g_fnCreature = function.OwnerCreature;
    for (; first != last; ++first) g_seen[g_applied++] = *first;
    return function;
}
}

int main()
{
    static unsigned char ownerBuf[0x64]; memset(ownerBuf, 0, sizeof ownerBuf);
    CThingCreatureBase* owner = (CThingCreatureBase*)ownerBuf;
    static CTCPhysicsBaseVirtuals physics;
    owner->PhysicsTC = &physics;
    CTCPhysicsNavigator nav;
    nav.ParentThing = (CThing*)owner;
    nav.Position.X = 5.0f; nav.Position.Y = 6.0f; nav.Position.Z = 7.0f; nav.Radius = 0.75f;
    // 1: all tests pass: impulse cleared, area with (1,1,0), search excluding the owner, functor applied to both
    {
        nav.AdditionalImpulse.X = nav.AdditionalImpulse.Y = nav.AdditionalImpulse.Z = 9.0f;
        nav.UpdateAdditionalImpulse();
        CHECK(strcmp(g_log, "1:Busy;2:Pushable;3:SelfAvoid;4:Area;5:Tools;6:Search;7:ForEach;") == 0);
        CHECK(nav.AdditionalImpulse.X == 0.0f && nav.AdditionalImpulse.Y == 0.0f && nav.AdditionalImpulse.Z == 0.0f);
        CHECK(g_selfAvoidThing == (CThing*)owner);
        CHECK(g_impulse.X == 1.0f && g_impulse.Y == 1.0f && g_impulse.Z == 0.0f);
        CHECK(g_area.TopLeft.X == 1 && g_area.TopLeft.Y == 2 && g_area.BottomRight.X == 3 && g_area.BottomRight.Y == 4);
        CHECK(g_notThing == (CThing*)owner);
        CHECK(g_applied == 2 && g_seen[0] == &g_c1 && g_seen[1] == &g_c2);
        CHECK(g_fnPos.X == 5.0f && g_fnPos.Y == 6.0f && g_fnPos.Z == 7.0f && g_fnRadius == 0.75f);
        CHECK(g_fnOwner == (CTCPhysicsBase*)&nav && g_fnCreature == owner);
    }
    // 2: feature off: only the clear
    {
        g_log[0] = 0; g_step = 0; g_applied = 0; g_AdditionalImpulseEnabled = false; nav.AdditionalImpulse.Y = 3.0f;
        nav.UpdateAdditionalImpulse();
        CHECK(g_log[0] == 0 && nav.AdditionalImpulse.Y == 0.0f);
        g_AdditionalImpulseEnabled = true;
    }
    // 3: not pushable: stops after the physics test
    {
        g_log[0] = 0; g_step = 0; g_pushable = false;
        nav.UpdateAdditionalImpulse();
        CHECK(strcmp(g_log, "1:Busy;2:Pushable;") == 0 && g_applied == 0);
        g_pushable = true;
    }
    // 4: nothing found: for_each over an empty range, no free
    {
        g_log[0] = 0; g_step = 0; g_nFound = 0;
        nav.UpdateAdditionalImpulse();
        CHECK(strcmp(g_log, "1:Busy;2:Pushable;3:SelfAvoid;4:Area;5:Tools;6:Search;7:ForEach;") == 0 && g_applied == 0);
    }
    // 5: not busy
    {
        g_log[0] = 0; g_step = 0; g_busy = false;
        nav.UpdateAdditionalImpulse();
        CHECK(strcmp(g_log, "1:Busy;") == 0);
    }
    if (fails == 0) printf("00726be0_TEST PASS\n");
    return fails;
}