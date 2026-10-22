// Behaviour test for 00970790: final.cpp copied verbatim, the declared callees stubbed below so calls and arguments
// are observable. CAIStateGroup_CreatureUseCoverBase::CanSeeTarget()
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifdef _MSC_VER
#define snprintf _snprintf
#else
#define __forceinline inline
#define __declspec(x)
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
static char g_log[4096];
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n  log=%s\n", __LINE__, #c, g_log); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_CreatureUseCoverBase::CanSeeTarget() -- with a live target and cover thing: true when the cover has no physics mesh, else when the eye-height line from the creature to the target does not hit the cover's mesh
// Retail: Fable.exe (TLC, VC7.1) @ 0x00970790, 264 bytes (`ret`). Ego_r.pdb: ?CanSeeTarget@CAIStateGroup_CreatureUseCoverBase@@IAE_NXZ.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (IS_THING_ALIVE twice, then the cover/target things),
// CTCPhysicsBase virtual slot 93 (+0x174, GetPhysicsMeshInfo), CThing::GetPosition 0x004C73D0,
// CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850 (CPhysicsMeshObject by hidden pointer) / ~CPhysicsMeshObject 0x00A127D0,
// CPhysicsMeshObject::GetPointIntersectionFromTo(const C3DVector*, const C3DVector*, CMeshIntersectionData*) 0x00A12450;
// 1.2f pool 0x0129749C (eye height).
// Shape: IS_THING_ALIVE on PTarget (+0x14) and PCoverThing (+0x1f, a pack(1) intelligent pointer); the cover's physics
// component reached through an inline accessor (`GetPhysics()`) so the slot-93 call's object lands straight in ecx (a
// direct member read goes through eax, and recolours this/&PTarget); the two line ends
// built as `C3DVector(p.X, p.Y, p.Z + 1.2f)` (Z first: value-ctor arguments right to left); the intersection call's
// arguments are pushed before the by-value mesh-object temporary is made; the bool is kept across its dtor.
// Layout: engine/CAIStateGroup_CreatureUseCoverBase.h (PCoverThing +0x1f); PTarget +0x14 (8-byte pointer before it).
// Leaf: CanSeeTarget@CAIStateGroup_CreatureUseCoverBase
struct CThing;

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                               // +0x04
    CThing* GetPItem() const;                      // 0x00A01B50
};
#pragma pack(pop)

struct CMeshIntersectionData { unsigned char _body[0x1c]; };
struct CPhysicsMeshObject {
    unsigned char _body[0x40];
    ~CPhysicsMeshObject();                         // 0x00A127D0
    bool GetPointIntersectionFromTo(const C3DVector* from, const C3DVector* to, CMeshIntersectionData* data);   // 0x00A12450
};
struct CTCPhysicsBaseV {                           // vtable view: slot 93 (+0x174)
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
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void v90(); virtual void v91(); virtual void v92();
    virtual void* GetPhysicsMeshInfo();            // slot 93 (+0x174)
    CPhysicsMeshObject GetPhysicsMeshObject();     // 0x006B0850
};

struct CThing {
    unsigned char _pad[0x60];
    CTCPhysicsBaseV* Physics;                      // +0x60
    CTCPhysicsBaseV* GetPhysics() const { return Physics; }
    unsigned char _pad64[0x2d];
    unsigned char m_flags91;                       // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;          // 0x004C73D0
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                             // +0x18
};

#pragma pack(push, 1)
struct CAIStateGroup_CreatureUseCoverBase {
    virtual ~CAIStateGroup_CreatureUseCoverBase();
    CAIBrain* PBrain;                              // +0x04
    unsigned char _pad08[0x0c];                    // +0x08
    CBaseIntelligentPointer PTarget;               // +0x14
    bool HasDiveAnim;                              // +0x1c
    bool HasJumpOutAnims;                          // +0x1d
    bool TimingCritical;                           // +0x1e
    CBaseIntelligentPointer PCoverThing;           // +0x1f
    bool CanSeeTarget();
};
#pragma pack(pop)

bool CAIStateGroup_CreatureUseCoverBase::CanSeeTarget()
{
    if (IS_THING_ALIVE(PTarget) && IS_THING_ALIVE(PCoverThing)) {
        if (PCoverThing.GetPItem()->GetPhysics()->GetPhysicsMeshInfo()) {
            const C3DVector& eye = PBrain->PCreature->GetPosition();
            C3DVector from(eye.X, eye.Y, eye.Z + 1.2f);
            const C3DVector& target = PTarget.GetPItem()->GetPosition();
            C3DVector to(target.X, target.Y, target.Z + 1.2f);
            CTCPhysicsBaseV* physics = PCoverThing.GetPItem()->Physics;
            CMeshIntersectionData data;
            bool blocked = physics->GetPhysicsMeshObject().GetPointIntersectionFromTo(&from, &to, &data);
            if (blocked)
                return false;
        }
        return true;
    }
    return false;
}

// ---- stubs ----
/* g_log declared above */
static void logf(const char* fmt, long a = 0, long b = 0, long c = 0) { char t[256]; snprintf(t, sizeof t, fmt, a, b, c); strcat(g_log, t); strcat(g_log, ";"); }
void CBaseIntelligentPointer::v00() { }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static int g_meshDtors;
CPhysicsMeshObject::~CPhysicsMeshObject() { ++g_meshDtors; }   // counted, not logged: the by-value return may run an extra temporary destructor under VC7.1
static bool g_blocked; static C3DVector g_creaturePos(1.0f, 2.0f, 3.0f); static C3DVector g_targetPos(10.0f, 20.0f, 30.0f);
bool CPhysicsMeshObject::GetPointIntersectionFromTo(const C3DVector* from, const C3DVector* to, CMeshIntersectionData* data)
{
    logf("Intersect(%ld,%ld,%ld)", (long)(from->X == 1.0f && from->Y == 2.0f && from->Z == 4.2f), (long)(to->X == 10.0f && to->Y == 20.0f && to->Z == 31.2f), (long)(data != 0));
    return g_blocked;
}
#define V(n) void CTCPhysicsBaseV::n() { }
V(v00) V(v01) V(v02) V(v03) V(v04) V(v05) V(v06) V(v07) V(v08) V(v09) V(v10) V(v11) V(v12) V(v13) V(v14) V(v15) V(v16) V(v17) V(v18) V(v19)
V(v20) V(v21) V(v22) V(v23) V(v24) V(v25) V(v26) V(v27) V(v28) V(v29) V(v30) V(v31) V(v32) V(v33) V(v34) V(v35) V(v36) V(v37) V(v38) V(v39)
V(v40) V(v41) V(v42) V(v43) V(v44) V(v45) V(v46) V(v47) V(v48) V(v49) V(v50) V(v51) V(v52) V(v53) V(v54) V(v55) V(v56) V(v57) V(v58) V(v59)
V(v60) V(v61) V(v62) V(v63) V(v64) V(v65) V(v66) V(v67) V(v68) V(v69) V(v70) V(v71) V(v72) V(v73) V(v74) V(v75) V(v76) V(v77) V(v78) V(v79)
V(v80) V(v81) V(v82) V(v83) V(v84) V(v85) V(v86) V(v87) V(v88) V(v89) V(v90) V(v91) V(v92)
#undef V
static void* g_meshInfo; static CTCPhysicsBaseV g_physics;
void* CTCPhysicsBaseV::GetPhysicsMeshInfo() { logf("GetPhysicsMeshInfo(%ld)", (long)(this == &g_physics)); return g_meshInfo; }
// returned as an unnamed temporary: VC7.1 has no NRVO, a named local would run a second destructor
CPhysicsMeshObject CTCPhysicsBaseV::GetPhysicsMeshObject() { logf("GetPhysicsMeshObject(%ld)", (long)(this == &g_physics)); return CPhysicsMeshObject(); }
static CThing* g_creature; static CThing* g_target;
const C3DVector& CThing::GetPosition() const { logf("GetPosition(%ld)", (long)(this == g_creature ? 1 : this == g_target ? 2 : 0)); return this == g_creature ? g_creaturePos : g_targetPos; }
CAIStateGroup_CreatureUseCoverBase::~CAIStateGroup_CreatureUseCoverBase() { }

int main()
{
    static unsigned char creatureBytes[0x100]; memset(creatureBytes, 0, sizeof creatureBytes); g_creature = (CThing*)creatureBytes;
    static unsigned char targetBytes[0x100]; memset(targetBytes, 0, sizeof targetBytes); g_target = (CThing*)targetBytes;
    static unsigned char coverBytes[0x100]; memset(coverBytes, 0, sizeof coverBytes); CThing* cover = (CThing*)coverBytes;
    cover->Physics = &g_physics;
    static CAIBrain brain; memset(&brain, 0, sizeof brain); brain.PCreature = g_creature;
    // the class has only a virtual destructor: zero-filled storage, the method called statically
    CAIStateGroup_CreatureUseCoverBase* group = (CAIStateGroup_CreatureUseCoverBase*)calloc(1, sizeof(*group));
    group->PBrain = &brain;

    // 1. no target: false, nothing else
    group->PTarget.m_pItem = 0; group->PCoverThing.m_pItem = cover; g_log[0] = 0;
    CHECK(group->CAIStateGroup_CreatureUseCoverBase::CanSeeTarget() == false);
    CHECK(strcmp(g_log, "") == 0);

    // 2. dead target / no cover / dead cover: false
    group->PTarget.m_pItem = g_target; g_target->m_flags91 = 1;
    CHECK(group->CAIStateGroup_CreatureUseCoverBase::CanSeeTarget() == false);
    g_target->m_flags91 = 0; group->PCoverThing.m_pItem = 0;
    CHECK(group->CAIStateGroup_CreatureUseCoverBase::CanSeeTarget() == false);
    group->PCoverThing.m_pItem = cover; cover->m_flags91 = 1;
    CHECK(group->CAIStateGroup_CreatureUseCoverBase::CanSeeTarget() == false);
    CHECK(strcmp(g_log, "") == 0);

    // 3. cover without a physics mesh: visible
    cover->m_flags91 = 0; g_meshInfo = 0; g_log[0] = 0;
    CHECK(group->CAIStateGroup_CreatureUseCoverBase::CanSeeTarget() == true);
    CHECK(strcmp(g_log, "GetPhysicsMeshInfo(1);") == 0);

    // 4. mesh present, line of sight blocked: false (eye-height line from the creature to the target, mesh object destroyed)
    g_meshInfo = (void*)1; g_blocked = true; g_log[0] = 0; g_meshDtors = 0;
    CHECK(group->CAIStateGroup_CreatureUseCoverBase::CanSeeTarget() == false);
    CHECK(strcmp(g_log, "GetPhysicsMeshInfo(1);GetPosition(1);GetPosition(2);GetPhysicsMeshObject(1);Intersect(1,1,1);") == 0);
    CHECK(g_meshDtors >= 1);

    // 5. mesh present, not blocked: true
    g_blocked = false; g_log[0] = 0; g_meshDtors = 0;
    CHECK(group->CAIStateGroup_CreatureUseCoverBase::CanSeeTarget() == true);
    CHECK(strcmp(g_log, "GetPhysicsMeshInfo(1);GetPosition(1);GetPosition(2);GetPhysicsMeshObject(1);Intersect(1,1,1);") == 0);
    CHECK(g_meshDtors >= 1);

    if (fails == 0) printf("00970790_TEST PASS\n"); else printf("00970790_TEST FAIL\n");
    return fails != 0;
}