// CTCActionUseWindow::OnActionUse(CThing& user) -- base OnActionUse; give a live creature user the look-through-window action; then, when the current game camera reports a view, touch the owner's position and let the user's physics fill a vector
// Leaf: ?OnActionUse@CTCActionUseWindow@@UAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EBAB0, length 122 bytes (+6 int3 padding). Vtable slot 28 of CTCActionUseWindow
// (0x0123F5EC). `ret 4`.
// Callees: CTCActionUseBase::OnActionUse @ 0x007EAB90 (qualified), CCreatureAction_LookThroughWindow::CCreatureAction_
// LookThroughWindow(CThingAICreature&) @ 0x007EBA20 (stores vtable 0x01270FBC = CCreatureAction_LookThroughWindow; catalog
// label "CCreatureAction_AlertToPlayer" propagated), CThingCreatureBase::SetCurrentAction @ 0x006644F0 (result dropped),
// ~CCreatureActionBase @ 0x00693EF0, CTCBase::GetWorld @ 0x00686A70, the camera's virtual slot 9 (+0x24, bool, out pointer),
// CThing::GetPosition @ 0x004C73D0 (result unused), the user's physics virtual slot 70 (+0x118, out pointer).
// Shape: the action temporary (0xA8 bytes, no own members: only the out-of-line base dtor runs) is passed by reference straight
// from the ctor's eax; the null/dead test guards only the action; the second block reads user.+0x60 unconditionally. The camera
// is reached through an inline CWorld accessor (GetCurrentGameCamera, stand-in name): it lands straight in ecx for the virtual
// call (`mov ecx,[eax+34h] ; mov eax,[ecx]`) and also gives retail's this=esi / user=edi colouring.
// Layout: PDB CWorld PCurrentGameCamera +0x34; retail CThing +0x60 physics pointer, +0x91 bit 0 Dead.
struct C3DVector { float X, Y, Z; };
struct CThingPhysics {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void GetVector(C3DVector* out);    // slot 70 (+0x118)
};
class CThing {
public:
    unsigned char _pad00[0x60];
    CThingPhysics* Physics;                    // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Flags91;                     // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    const C3DVector& GetPosition() const;      // 0x004C73D0
};
class CCreatureActionBase;
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CThingAICreature : public CThingCreatureBase {};
class CCreatureActionBase {                    // retail 0xA8 bytes
public:
    virtual ~CCreatureActionBase();            // 0x00693EF0
    unsigned char _pad04[0xa8 - 4];
};
class __declspec(novtable) CCreatureAction_LookThroughWindow : public CCreatureActionBase {
public:
    CCreatureAction_LookThroughWindow(CThingAICreature& creature);   // 0x007EBA20
};
struct CAIGameCameraBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetView(long* out);           // slot 9 (+0x24)
};
struct CWorld {
    unsigned char _pad00[0x34];
    CAIGameCameraBase* PCurrentGameCamera;     // +0x34
    CAIGameCameraBase* GetCurrentGameCamera() const { return PCurrentGameCamera; }
};

inline CThingAICreature* CastToCreature(CThing* thing) { return (CThingAICreature*)thing; }

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
    CWorld* GetWorld();                        // 0x00686A70
};
class CTCActionUseBase : public CTCBase {
public:
    virtual void OnActionUse(CThing& user);    // 0x007EAB90
};
class CTCActionUseWindow : public CTCActionUseBase {
public:
    virtual void OnActionUse(CThing& user);
    void UpdateView(CThing& user)
    {
        long view;
        if (GetWorld()->GetCurrentGameCamera()->GetView(&view)) {
            ParentThing->GetPosition();
            C3DVector v;
            user.Physics->GetVector(&v);
        }
    }
};

void CTCActionUseWindow::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    CThingAICreature* creature = CastToCreature(&user);
    if (creature && !creature->IsDead())
        creature->SetCurrentAction(CCreatureAction_LookThroughWindow(*creature));
    UpdateView(user);
}

#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
void CThingPhysics::v00() { }
void CThingPhysics::v01() { }
void CThingPhysics::v02() { }
void CThingPhysics::v03() { }
void CThingPhysics::v04() { }
void CThingPhysics::v05() { }
void CThingPhysics::v06() { }
void CThingPhysics::v07() { }
void CThingPhysics::v08() { }
void CThingPhysics::v09() { }
void CThingPhysics::v10() { }
void CThingPhysics::v11() { }
void CThingPhysics::v12() { }
void CThingPhysics::v13() { }
void CThingPhysics::v14() { }
void CThingPhysics::v15() { }
void CThingPhysics::v16() { }
void CThingPhysics::v17() { }
void CThingPhysics::v18() { }
void CThingPhysics::v19() { }
void CThingPhysics::v20() { }
void CThingPhysics::v21() { }
void CThingPhysics::v22() { }
void CThingPhysics::v23() { }
void CThingPhysics::v24() { }
void CThingPhysics::v25() { }
void CThingPhysics::v26() { }
void CThingPhysics::v27() { }
void CThingPhysics::v28() { }
void CThingPhysics::v29() { }
void CThingPhysics::v30() { }
void CThingPhysics::v31() { }
void CThingPhysics::v32() { }
void CThingPhysics::v33() { }
void CThingPhysics::v34() { }
void CThingPhysics::v35() { }
void CThingPhysics::v36() { }
void CThingPhysics::v37() { }
void CThingPhysics::v38() { }
void CThingPhysics::v39() { }
void CThingPhysics::v40() { }
void CThingPhysics::v41() { }
void CThingPhysics::v42() { }
void CThingPhysics::v43() { }
void CThingPhysics::v44() { }
void CThingPhysics::v45() { }
void CThingPhysics::v46() { }
void CThingPhysics::v47() { }
void CThingPhysics::v48() { }
void CThingPhysics::v49() { }
void CThingPhysics::v50() { }
void CThingPhysics::v51() { }
void CThingPhysics::v52() { }
void CThingPhysics::v53() { }
void CThingPhysics::v54() { }
void CThingPhysics::v55() { }
void CThingPhysics::v56() { }
void CThingPhysics::v57() { }
void CThingPhysics::v58() { }
void CThingPhysics::v59() { }
void CThingPhysics::v60() { }
void CThingPhysics::v61() { }
void CThingPhysics::v62() { }
void CThingPhysics::v63() { }
void CThingPhysics::v64() { }
void CThingPhysics::v65() { }
void CThingPhysics::v66() { }
void CThingPhysics::v67() { }
void CThingPhysics::v68() { }
void CThingPhysics::v69() { }
void CAIGameCameraBase::v00() { }
void CAIGameCameraBase::v01() { }
void CAIGameCameraBase::v02() { }
void CAIGameCameraBase::v03() { }
void CAIGameCameraBase::v04() { }
void CAIGameCameraBase::v05() { }
void CAIGameCameraBase::v06() { }
void CAIGameCameraBase::v07() { }
void CAIGameCameraBase::v08() { }

static int g_vec = 0; static int g_view = 0; static bool g_viewResult = false; static int g_pos = 0;
static int g_base = 0; static int g_ctor = 0; static int g_set = 0; static int g_dtor = 0;
static CThing* g_setOn = 0; static CThingAICreature* g_ctorArg = 0; static CThing* g_baseUser = 0;
void CThingPhysics::GetVector(C3DVector* out) { ++g_vec; out->X = 1.0f; }
bool CAIGameCameraBase::GetView(long* out) { ++g_view; *out = 5; return g_viewResult; }
const C3DVector& CThing::GetPosition() const { static C3DVector p; ++g_pos; return p; }
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase& action) { ++g_set; g_setOn = this; return true; }
CCreatureActionBase::~CCreatureActionBase() { ++g_dtor; }
CCreatureAction_LookThroughWindow::CCreatureAction_LookThroughWindow(CThingAICreature& creature) { ++g_ctor; g_ctorArg = &creature; }
static CWorld g_world;
CWorld* CTCBase::GetWorld() { return &g_world; }
CTCBase::~CTCBase() { }
void CTCActionUseBase::OnActionUse(CThing& user) { ++g_base; g_baseUser = &user; }

int main()
{
    static CThingPhysics physics;
    static CAIGameCameraBase camera;
    g_world.PCurrentGameCamera = &camera;
    static CThingAICreature user;
    user.Physics = &physics; user.Flags91 = 0;
    static CThing owner;
    CTCActionUseWindow tc;
    tc.ParentThing = &owner;
    g_viewResult = false;
    tc.OnActionUse(user);
    CHECK(g_base == 1 && g_baseUser == &user);
    CHECK(g_ctor == 1 && g_ctorArg == &user && g_set == 1 && g_setOn == &user && g_dtor == 1);
    CHECK(g_view == 1 && g_pos == 0 && g_vec == 0);
    user.Flags91 = 1;            // dead: no action
    g_viewResult = true;
    tc.OnActionUse(user);
    CHECK(g_base == 2 && g_ctor == 1 && g_set == 1 && g_dtor == 1);
    CHECK(g_view == 2 && g_pos == 1 && g_vec == 1);
    if (fails == 0) printf("007ebab0_TEST PASS\n");
    return fails;
}