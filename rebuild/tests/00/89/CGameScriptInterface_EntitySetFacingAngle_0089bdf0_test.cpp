// Behaviour test for 0089bdf0: src/0089bdf0.cpp copied verbatim, the script thing, physics and components stubbed so the
// early exits, the look direction and the immediate turn are observable. CGameScriptInterface::EntitySetFacingAngle
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CGameScriptInterface::EntitySetFacingAngle(const CScriptThing& entity, float angle, bool immediate) const -- turn a valid entity to the angle (a fraction of a full turn): reset TC 7, stop its looking (TC 0x43); unless immediate, make it look at a point one unit away along the new heading, else set the physics facing directly and reset its perception (TC 0x34)
// Retry 10b: cos(pitch) held as a FLOAT local (a double local keeps it on the x87 stack past the Y product: `fmul st(1)` + a late `fstp st(0)`; the float is consumed by retail's `fld st(2) ; fmulp` / `fmulp st(1)`).
// Leaf: ?EntitySetFacingAngle@CGameScriptInterface@@UBEXABVCScriptThing@@M_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089BDF0, length 385 bytes (`ret 0Ch`).
// Callees: the script thing's slots 75 (+0x12C IsValid), 11 (+0x2C GetThing) and 6 (+0x18, the position), the thing
//   physics (+0x60) slots 63 (+0xFC, current facing angle) and 66 (+0x108, set facing angle), the component-map
//   LowerBound @ 0x0040F020, the TC 7 reset @ 0x006CB6B0 (manifest "CScriptThing::operator==" is propagated),
//   CTCLook::StopLooking(long) @ 0x00751B80 and LookAtPos(const C3DVector&, long) @ 0x00753E90 (TC 0x43),
//   CTCPerceiveThing::ResetPerceptionVariables @ 0x00744DB0 (TC 0x34).
// Shape: the look direction is the intrinsic cos/sin pair of `angle * (double)2pi` and a zero pitch read from a double
//   in memory (0x0122ED70, not folded), X = sin(yaw)*cos(pitch), Y = cos(yaw)*cos(pitch), Z = sin(pitch).
#include <math.h>
#pragma intrinsic(sin, cos)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
extern const double g_zeroPitch;                        // 0x0122ED70 (0.0)
extern const double g_twoPi;                            // 0x0124F2B8 ((double)6.2831855f)
struct CTCBase_Retail;
struct CInterfaceMap_Retail {
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CThingPhysics_Retail {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngle();                     // slot 63 (+0xFC)
    virtual void v64(); virtual void v65();
    virtual void SetFacingAngle(float angle);           // slot 66 (+0x108)
};
struct CThing_Retail {
    unsigned char         _pad00[0x20];
    unsigned long         m_presentBits[9];         // +0x20
    CInterfaceMap_Retail  m_interfaces;             // +0x44
    unsigned char         _pad4c[0x14];
    CThingPhysics_Retail* m_pPhysics;               // +0x60
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase_Retail* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
struct CTCType7 { void Reset(); };                          // 0x006CB6B0
struct CTCLook {
    void StopLooking(long reason);                          // 0x00751B80
    void LookAtPos(const C3DVector& pos, long priority);    // 0x00753E90
};
struct CTCPerceiveThing { void ResetPerceptionVariables(); };   // 0x00744DB0
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual const C3DVector& GetPosition() const;   // slot 6 (+0x18)
    virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing_Retail* GetThing() const;        // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
};
struct CGameScriptInterface {
    virtual void EntitySetFacingAngle(const CScriptThing& entity, float angle, bool immediate) const;
};

void CGameScriptInterface::EntitySetFacingAngle(const CScriptThing& entity, float angle, bool immediate) const
{
    if (!entity.IsValid())
        return;
    CThing_Retail* thing = entity.GetThing();
    if (thing->m_pPhysics->GetFacingAngle() == angle)
        return;
    if (thing->HasInterface(7))
        ((CTCType7*)thing->GetInterface(7))->Reset();
    CTCLook* look;
    if (thing->HasInterface(0x43))
        look = (CTCLook*)thing->GetInterface(0x43);
    else
        look = 0;
    if (look)
        look->StopLooking(0);
    if (!immediate && look) {
        float cosPitch = (float)cos(g_zeroPitch);
        double yaw = angle * g_twoPi;
        C3DVector direction((float)(sin(yaw) * cosPitch), (float)(cos(yaw) * cosPitch), (float)sin(g_zeroPitch));
        look->LookAtPos(entity.GetPosition() + direction, 0x21);
        return;
    }
    thing->m_pPhysics->SetFacingAngle(angle);
    if (thing->HasInterface(0x34))
        ((CTCPerceiveThing*)thing->GetInterface(0x34))->ResetPerceptionVariables();
}

// ---- stubs ----
const double g_zeroPitch = 0.0;
const double g_twoPi = (double)6.2831855f;
void CThingPhysics_Retail::v00() {}
void CThingPhysics_Retail::v01() {}
void CThingPhysics_Retail::v02() {}
void CThingPhysics_Retail::v03() {}
void CThingPhysics_Retail::v04() {}
void CThingPhysics_Retail::v05() {}
void CThingPhysics_Retail::v06() {}
void CThingPhysics_Retail::v07() {}
void CThingPhysics_Retail::v08() {}
void CThingPhysics_Retail::v09() {}
void CThingPhysics_Retail::v10() {}
void CThingPhysics_Retail::v11() {}
void CThingPhysics_Retail::v12() {}
void CThingPhysics_Retail::v13() {}
void CThingPhysics_Retail::v14() {}
void CThingPhysics_Retail::v15() {}
void CThingPhysics_Retail::v16() {}
void CThingPhysics_Retail::v17() {}
void CThingPhysics_Retail::v18() {}
void CThingPhysics_Retail::v19() {}
void CThingPhysics_Retail::v20() {}
void CThingPhysics_Retail::v21() {}
void CThingPhysics_Retail::v22() {}
void CThingPhysics_Retail::v23() {}
void CThingPhysics_Retail::v24() {}
void CThingPhysics_Retail::v25() {}
void CThingPhysics_Retail::v26() {}
void CThingPhysics_Retail::v27() {}
void CThingPhysics_Retail::v28() {}
void CThingPhysics_Retail::v29() {}
void CThingPhysics_Retail::v30() {}
void CThingPhysics_Retail::v31() {}
void CThingPhysics_Retail::v32() {}
void CThingPhysics_Retail::v33() {}
void CThingPhysics_Retail::v34() {}
void CThingPhysics_Retail::v35() {}
void CThingPhysics_Retail::v36() {}
void CThingPhysics_Retail::v37() {}
void CThingPhysics_Retail::v38() {}
void CThingPhysics_Retail::v39() {}
void CThingPhysics_Retail::v40() {}
void CThingPhysics_Retail::v41() {}
void CThingPhysics_Retail::v42() {}
void CThingPhysics_Retail::v43() {}
void CThingPhysics_Retail::v44() {}
void CThingPhysics_Retail::v45() {}
void CThingPhysics_Retail::v46() {}
void CThingPhysics_Retail::v47() {}
void CThingPhysics_Retail::v48() {}
void CThingPhysics_Retail::v49() {}
void CThingPhysics_Retail::v50() {}
void CThingPhysics_Retail::v51() {}
void CThingPhysics_Retail::v52() {}
void CThingPhysics_Retail::v53() {}
void CThingPhysics_Retail::v54() {}
void CThingPhysics_Retail::v55() {}
void CThingPhysics_Retail::v56() {}
void CThingPhysics_Retail::v57() {}
void CThingPhysics_Retail::v58() {}
void CThingPhysics_Retail::v59() {}
void CThingPhysics_Retail::v60() {}
void CThingPhysics_Retail::v61() {}
void CThingPhysics_Retail::v62() {}
void CThingPhysics_Retail::v64() {}
void CThingPhysics_Retail::v65() {}
void CScriptThing::v000() {}
void CScriptThing::v001() {}
void CScriptThing::v002() {}
void CScriptThing::v003() {}
void CScriptThing::v004() {}
void CScriptThing::v005() {}
void CScriptThing::v007() {}
void CScriptThing::v008() {}
void CScriptThing::v009() {}
void CScriptThing::v010() {}
void CScriptThing::v012() {}
void CScriptThing::v013() {}
void CScriptThing::v014() {}
void CScriptThing::v015() {}
void CScriptThing::v016() {}
void CScriptThing::v017() {}
void CScriptThing::v018() {}
void CScriptThing::v019() {}
void CScriptThing::v020() {}
void CScriptThing::v021() {}
void CScriptThing::v022() {}
void CScriptThing::v023() {}
void CScriptThing::v024() {}
void CScriptThing::v025() {}
void CScriptThing::v026() {}
void CScriptThing::v027() {}
void CScriptThing::v028() {}
void CScriptThing::v029() {}
void CScriptThing::v030() {}
void CScriptThing::v031() {}
void CScriptThing::v032() {}
void CScriptThing::v033() {}
void CScriptThing::v034() {}
void CScriptThing::v035() {}
void CScriptThing::v036() {}
void CScriptThing::v037() {}
void CScriptThing::v038() {}
void CScriptThing::v039() {}
void CScriptThing::v040() {}
void CScriptThing::v041() {}
void CScriptThing::v042() {}
void CScriptThing::v043() {}
void CScriptThing::v044() {}
void CScriptThing::v045() {}
void CScriptThing::v046() {}
void CScriptThing::v047() {}
void CScriptThing::v048() {}
void CScriptThing::v049() {}
void CScriptThing::v050() {}
void CScriptThing::v051() {}
void CScriptThing::v052() {}
void CScriptThing::v053() {}
void CScriptThing::v054() {}
void CScriptThing::v055() {}
void CScriptThing::v056() {}
void CScriptThing::v057() {}
void CScriptThing::v058() {}
void CScriptThing::v059() {}
void CScriptThing::v060() {}
void CScriptThing::v061() {}
void CScriptThing::v062() {}
void CScriptThing::v063() {}
void CScriptThing::v064() {}
void CScriptThing::v065() {}
void CScriptThing::v066() {}
void CScriptThing::v067() {}
void CScriptThing::v068() {}
void CScriptThing::v069() {}
void CScriptThing::v070() {}
void CScriptThing::v071() {}
void CScriptThing::v072() {}
void CScriptThing::v073() {}
void CScriptThing::v074() {}
const C3DVector& CScriptThing::GetPosition() const { static C3DVector p(0, 0, 0); return p; }
CThing_Retail* CScriptThing::GetThing() const { return 0; }
bool CScriptThing::IsValid() const { return false; }
float CThingPhysics_Retail::GetFacingAngle() { return 0; }
void CThingPhysics_Retail::SetFacingAngle(float) {}
struct CTestPhysics : CThingPhysics_Retail {
    float facing; int sets;
    virtual float GetFacingAngle() { return facing; }
    virtual void SetFacingAngle(float a) { facing = a; ++sets; }
};
struct CTestThing : CScriptThing {
    bool valid; CThing_Retail* thing; C3DVector pos;
    virtual const C3DVector& GetPosition() const { return pos; }
    virtual CThing_Retail* GetThing() const { return thing; }
    virtual bool IsValid() const { return valid; }
};
CInterfaceMap_Retail::SEntry* CInterfaceMap_Retail::LowerBound(const long* type)
{ for (SEntry* it = first; it != last; ++it) if (!(it->type < *type)) return it; return last; }
static int g_resets = 0; static int g_stops = 0; static long g_stopReason = -1; static C3DVector g_lookAt; static long g_lookPri = 0; static int g_looks = 0; static int g_perceive = 0;
void CTCType7::Reset() { ++g_resets; }
void CTCLook::StopLooking(long reason) { ++g_stops; g_stopReason = reason; }
void CTCLook::LookAtPos(const C3DVector& pos, long priority) { ++g_looks; g_lookAt = pos; g_lookPri = priority; }
void CTCPerceiveThing::ResetPerceptionVariables() { ++g_perceive; }
static void SetBit(CThing_Retail& t, long type) { t.m_presentBits[type >> 5] |= 1UL << (type & 31); }
int main()
{
    static CTestPhysics phys; phys.facing = 0.0f; phys.sets = 0;
    static CThing_Retail thing; memset(&thing, 0, sizeof(thing)); thing.m_pPhysics = &phys;
    static CTCType7 t7; static CTCLook look; static CTCPerceiveThing perceive;
    static CInterfaceMap_Retail::SEntry entries[3] = { { 7, (CTCBase_Retail*)&t7 }, { 0x34, (CTCBase_Retail*)&perceive }, { 0x43, (CTCBase_Retail*)&look } };
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 3;
    CTestThing st; st.valid = false; st.thing = &thing; st.pos = C3DVector(10, 20, 30);
    static unsigned char gsiRaw[sizeof(CGameScriptInterface)]; CGameScriptInterface* gsi = (CGameScriptInterface*)gsiRaw;
    // invalid entity: nothing happens
    gsi->CGameScriptInterface::EntitySetFacingAngle(st, 0.25f, true); CHECK(phys.sets == 0 && g_resets == 0);
    // same angle: nothing happens
    st.valid = true; phys.facing = 0.5f;
    gsi->CGameScriptInterface::EntitySetFacingAngle(st, 0.5f, true); CHECK(phys.sets == 0);
    // immediate, no components: set directly
    gsi->CGameScriptInterface::EntitySetFacingAngle(st, 0.25f, true); CHECK(phys.sets == 1 && phys.facing == 0.25f && g_resets == 0 && g_perceive == 0);
    // all components, immediate: reset TC7, stop looking, set, reset perception
    SetBit(thing, 7); SetBit(thing, 0x43); SetBit(thing, 0x34);
    gsi->CGameScriptInterface::EntitySetFacingAngle(st, 0.75f, true);
    CHECK(g_resets == 1 && g_stops == 1 && g_stopReason == 0 && phys.sets == 2 && phys.facing == 0.75f && g_perceive == 1 && g_looks == 0);
    // not immediate with a look component: look along the angle (quarter turn: +X), no direct set
    gsi->CGameScriptInterface::EntitySetFacingAngle(st, 0.25f, false);
    CHECK(g_looks == 1 && g_lookPri == 0x21 && phys.sets == 2 && g_perceive == 1);
    CHECK(g_lookAt.X > 10.99f && g_lookAt.X < 11.01f && g_lookAt.Y > 19.99f && g_lookAt.Y < 20.01f && g_lookAt.Z == 30.0f);
    if (fails == 0) printf("0089bdf0_TEST PASS\n"); else printf("0089bdf0_TEST FAIL\n");
    return fails;
}