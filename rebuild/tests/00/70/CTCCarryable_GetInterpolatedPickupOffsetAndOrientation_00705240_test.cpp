// Behaviour test for 00705240: src/00705240.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCCarryable::GetInterpolatedPickupOffsetAndOrientation(C3DVector&, CRightHandedSet&, bool, float) const
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCCarryable::GetInterpolatedPickupOffsetAndOrientation(C3DVector&, CRightHandedSet&, bool, float) const -- offset of the carrier's hand dummy (right hand, else left hand) from the owner's interpolated position, with the dummy's orientation; with no hand dummy, a zero offset and the physics orientation
// Leaf: GetInterpolatedPickupOffsetAndOrientation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00705240, 424 bytes (`ret 10h`, 0x24-byte frame).
//         Ego_r.pdb: `?GetInterpolatedPickupOffsetAndOrientation@CTCCarryable@@ABEXAAVC3DVector@@AAVCRightHandedSet@@_NM@Z`.
// Callees: NGraphicAppearance::CTCGraphicAppearance::GetDummyObject(const CCharString&, long, unsigned long, C3DVector&,
//          CRightHandedSet&, float) const @ 0x004C08C0 (twice; hand names g_RightHandAttachmentName @ 0x013BAC30 and
//          g_LeftHandAttachmentName @ 0x013BAC34, as the sibling GetInterpolatedPickupOffsetAndOrientationAt 0x00705190),
//          CTCPhysicsBase::GetInterpolatedPosition(float) @ 0x0049D150 (out-of-line copy on the left-hand path; the
//          right-hand path inlines it), the physics component's virtual slot 72 GetRHSet (+0x120).
// Layout: CTCBase ParentThing +0x04; retail CThing PhysicsTC +0x60, PTCGraphicAppearance +0x64 (PDB +0x70/+0x74);
//         CTCPhysicsBase Position +0x0c, OldPosition +0x18 (engine/CTCPhysicsBase.h is a flattened POD without virtuals,
//         so the class is modelled locally with the same offsets, as the landed GetInterpolatedPosition 0x0049D150).
// Shape: flags (0 when exact, else 0x21) chosen into ebp before the right-hand lookup; the right-hand path inlines
//        GetInterpolatedPosition (VC7.1 inline budget) and the left-hand path calls the out-of-line copy; each path
//        subtracts into a scalarised C3DVector temporary copied out through integer registers and keeps its own epilogue.
//        The graphic and physics components are read through inline const accessors (GetGraphicAppearance/GetPhysics):
//        with direct ParentThing->member chains VC7.1 colours ParentThing differently on the two paths and cross-jumps
//        their identical `offset = temp` copies into one shared tail.
class CCharString { public: void* m_pData; };
extern CCharString g_RightHandAttachmentName;           // 0x013BAC30
extern CCharString g_LeftHandAttachmentName;            // 0x013BAC34
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
class CRightHandedSet { public:
    C3DVector Up;                                       // +0x00
    C3DVector Forward;                                  // +0x0c
};
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    bool GetDummyObject(const CCharString& name, long lod, unsigned long flags, C3DVector& outPosition,
                        CRightHandedSet& outOrientation, float t) const;   // 0x004C08C0
};
}
class CTCPhysicsBase {
public:
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
    virtual const CRightHandedSet& GetRHSet() const;    // slot 72 (+0x120)
    void*     m_tcBase[2];                              // +0x04 CTCBase body
    C3DVector Position;                                 // +0x0c
    C3DVector OldPosition;                              // +0x18
    long      OldPositionLastFrameSet;                  // +0x24
    C3DVector GetInterpolatedPosition(float t) { return (Position - OldPosition) * t + OldPosition; }
};
class CThing { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
};
class CTCCarryable {
public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    NGraphicAppearance::CTCGraphicAppearance* GetGraphicAppearance() const { return ParentThing->PTCGraphicAppearance; }
    CTCPhysicsBase* GetPhysics() const { return ParentThing->PhysicsTC; }
private:
    void GetInterpolatedPickupOffsetAndOrientation(C3DVector& offset, CRightHandedSet& orientation, bool exact, float t) const;
};

void CTCCarryable::GetInterpolatedPickupOffsetAndOrientation(C3DVector& offset, CRightHandedSet& orientation, bool exact, float t) const
{
    C3DVector dummy;
    unsigned long flags = 0;
    if (!exact)
        flags = 0x21;
    if (GetGraphicAppearance()->GetDummyObject(g_RightHandAttachmentName, 0, flags, dummy, orientation, t))
        offset = dummy - GetPhysics()->GetInterpolatedPosition(t);
    else if (GetGraphicAppearance()->GetDummyObject(g_LeftHandAttachmentName, 0, flags, dummy, orientation, t))
        offset = dummy - GetPhysics()->GetInterpolatedPosition(t);
    else {
        offset = C3DVector(0.0f, 0.0f, 0.0f);
        orientation = GetPhysics()->GetRHSet();
    }
}

// ---- stubs and driver ----
CCharString g_RightHandAttachmentName = { (void*)"RightHand" };
CCharString g_LeftHandAttachmentName = { (void*)"LeftHand" };
static int g_calls = 0, g_rhCalls = 0; static bool g_rightOk = false, g_leftOk = false;
static const CCharString* g_names[4]; static long g_lods[4]; static unsigned long g_flags[4]; static float g_ts[4];
static C3DVector* g_outPos[4]; static CRightHandedSet* g_outRh[4];
static C3DVector g_dummyRight, g_dummyLeft; static CRightHandedSet g_physRh;
bool NGraphicAppearance::CTCGraphicAppearance::GetDummyObject(const CCharString& name, long lod, unsigned long flags,
    C3DVector& outPosition, CRightHandedSet& outOrientation, float t) const
{
    int i = g_calls++;
    if (i < 4) { g_names[i] = &name; g_lods[i] = lod; g_flags[i] = flags; g_ts[i] = t; g_outPos[i] = &outPosition; g_outRh[i] = &outOrientation; }
    bool right = &name == &g_RightHandAttachmentName;
    if (right ? g_rightOk : g_leftOk) {
        outPosition = right ? g_dummyRight : g_dummyLeft;
        outOrientation.Up = C3DVector(right ? 1.0f : 2.0f, 0.0f, 0.0f);
        return true;
    }
    return false;
}
#define V6(a,b,c,d,e,f) void CTCPhysicsBase::v##a() {} void CTCPhysicsBase::v##b() {} void CTCPhysicsBase::v##c() {} \
    void CTCPhysicsBase::v##d() {} void CTCPhysicsBase::v##e() {} void CTCPhysicsBase::v##f() {}
V6(00,01,02,03,04,05) V6(06,07,08,09,10,11) V6(12,13,14,15,16,17) V6(18,19,20,21,22,23) V6(24,25,26,27,28,29)
V6(30,31,32,33,34,35) V6(36,37,38,39,40,41) V6(42,43,44,45,46,47) V6(48,49,50,51,52,53) V6(54,55,56,57,58,59)
V6(60,61,62,63,64,65) V6(66,67,68,69,70,71)
const CRightHandedSet& CTCPhysicsBase::GetRHSet() const { ++g_rhCalls; return g_physRh; }
void CTCCarryable::v00() {}

// the method is private: take its address through an explicit instantiation (access is not checked there)
typedef void (CTCCarryable::*PickupFn)(C3DVector&, CRightHandedSet&, bool, float) const;
static PickupFn g_fn = 0;
template <PickupFn F> struct Thief { Thief() { g_fn = F; } static Thief instance; };
template <PickupFn F> Thief<F> Thief<F>::instance;
template struct Thief<&CTCCarryable::GetInterpolatedPickupOffsetAndOrientation>;

static bool Eq(const C3DVector& v, float x, float y, float z) { return v.X == x && v.Y == y && v.Z == z; }
int main()
{
    static CThing thing; static CTCPhysicsBase physics; static NGraphicAppearance::CTCGraphicAppearance graphic;
    static CTCCarryable carryable;
    CHECK(g_fn != 0);
    thing.PhysicsTC = &physics; thing.PTCGraphicAppearance = &graphic; carryable.ParentThing = &thing;
    physics.Position = C3DVector(4.0f, 6.0f, 8.0f); physics.OldPosition = C3DVector(0.0f, 2.0f, 4.0f);   // at t=0.5: (2,4,6)
    g_dummyRight = C3DVector(10.0f, 20.0f, 30.0f); g_dummyLeft = C3DVector(1.0f, 1.0f, 1.0f);
    g_physRh.Up = C3DVector(0.0f, 0.0f, 1.0f); g_physRh.Forward = C3DVector(0.0f, 1.0f, 0.0f);
    C3DVector offset; CRightHandedSet rh;

    // right hand found (exact): flags 0, offset = dummy - interpolated position
    g_calls = g_rhCalls = 0; g_rightOk = true; g_leftOk = true; offset = C3DVector(-1.0f, -1.0f, -1.0f);
    (carryable.*g_fn)(offset, rh, true, 0.5f);
    CHECK(g_calls == 1 && g_names[0] == &g_RightHandAttachmentName && g_lods[0] == 0 && g_flags[0] == 0 && g_ts[0] == 0.5f);
    CHECK(g_outRh[0] == &rh && g_outPos[0] != &offset && rh.Up.X == 1.0f && g_rhCalls == 0);
    CHECK(Eq(offset, 8.0f, 16.0f, 24.0f));

    // inexact: flags 0x21; right missing, left found -> left dummy minus interpolated position (t = 0.25: (1,3,5))
    g_calls = 0; g_rightOk = false; g_leftOk = true;
    (carryable.*g_fn)(offset, rh, false, 0.25f);
    CHECK(g_calls == 2 && g_names[0] == &g_RightHandAttachmentName && g_names[1] == &g_LeftHandAttachmentName);
    CHECK(g_flags[0] == 0x21 && g_flags[1] == 0x21 && g_lods[1] == 0 && g_ts[1] == 0.25f && g_outRh[1] == &rh && g_outPos[1] == g_outPos[0]);
    CHECK(Eq(offset, 0.0f, -2.0f, -4.0f) && rh.Up.X == 2.0f && g_rhCalls == 0);

    // neither hand: zero offset, physics orientation
    g_calls = 0; g_leftOk = false; rh.Up = C3DVector(9.0f, 9.0f, 9.0f);
    (carryable.*g_fn)(offset, rh, true, 1.0f);
    CHECK(g_calls == 2 && g_rhCalls == 1);
    CHECK(Eq(offset, 0.0f, 0.0f, 0.0f) && Eq(rh.Up, 0.0f, 0.0f, 1.0f) && Eq(rh.Forward, 0.0f, 1.0f, 0.0f));
    if (fails == 0) printf("00705240_TEST PASS\n");
    return fails;
}
