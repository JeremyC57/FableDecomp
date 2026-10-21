#include <stdio.h>
#include <string.h>
// CTCLook::SetBodyMovingViaAnimation() -- while the creature plays a turning animation set (type 7) whose first anim is loaded, turn the body by the current turning anim's rotation over this frame's slice of the animation
// Leaf: SetBodyMovingViaAnimation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00753930, 401 bytes (+int3), `ret`, esp frame 0x3c (speed TU).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inline GetInterface 0x5A, unchecked: the miss path reads the dead `turn` slot),
//   CTCAnimationComplex::GetAnimation @ 0x0070B460 (CAnimateThing&, called twice), CAnimateThing::GetMeshBank @ 0x0070C0D0,
//   CMeshDataBank::PeekAnimation @ 0x00A242C0, C3DAnimationInfo::IsLoaded @ 0x00A23F50, GetAnimDuration @ 0x0070B4A0,
//   the turning anim entry @ 0x005DC000 (manifest "_Cons_val"; takes (from, to, float* turn)), physics slots 72 / 75 (+0x120 /
//   +0x12c, get / set the right-handed set), C3DVector::Rotate @ 0x00A13C60, C3DVector::Normalise @ 0x00A14440.
// Shape: forward is copied before up (two named vectors from the const reference), forward.Z is cleared before up is set to
//   (0,0,1) through a temporary, and SetRHSet gets a CRightHandedSet(up, forward) temporary (the second 24-byte copy).
//   GetTC stores the component as ONE ternary (`(it == last || type < it->type) ? last : it`), and the duration's anim
//   index is read into a named long first (this in ebx, the component in edi, the movsx through edx/eax as retail).
// Layout (retail, local model): CAnimSet in CAnimateThing at +0 (Anims vector<short> +0x04, CurrentPercentageComplete +0x1c,
//   LastPercentageComplete +0x20, Type +0x50; PDB +0x58 with 16-byte vectors); CTCLook FractionOfTurningToAcceptEachFrame +0x44,
//   PCurrentTurningAnim +0x9c (PDB +0xa0).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    void Rotate(const C3DVector& axis, float angle);   // 0x00A13C60
    void Normalise();                                  // 0x00A14440
};
struct CRightHandedSet {
    C3DVector Up;                                      // +0x00
    C3DVector Forward;                                 // +0x0c
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
};
struct C3DAnimationInfo {
    bool IsLoaded() const;                             // 0x00A23F50
};
struct CMeshDataBank {
    C3DAnimationInfo* PeekAnimation(long index);       // 0x00A242C0
};
struct CShortVector {                                  // STLport vector<short>
    short* _M_start;
    short* _M_finish;
    short* _M_end_of_storage;
    short* begin() { return _M_start; }
    short& operator[](unsigned int n) { return *(begin() + n); }
};
struct CAnimateThing {                                 // CurrentAnim.MainAnimSet at +0 (retail layout)
    long           UpdateMode;                         // +0x00
    CShortVector   Anims;                              // +0x04 vector<short>
    unsigned char  _pad10[0x1c - 0x10];
    float          CurrentPercentageComplete;          // +0x1c
    float          LastPercentageComplete;             // +0x20
    unsigned char  _pad24[0x50 - 0x24];
    unsigned char  Type;                               // +0x50
    CMeshDataBank* GetMeshBank();                      // 0x0070C0D0
};
struct CTCAnimationComplex {                           // interface 0x5A
    CAnimateThing& GetAnimation();                     // 0x0070B460
    float GetAnimDuration(long index);                 // 0x0070B4A0
};
struct CAnimationEntry {
    void GetTurnBetween(float from, float to, float* turn) const;   // 0x005DC000 (name descriptive)
};
struct CTCPhysicsBase {
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
    virtual const CRightHandedSet& GetRHSet();         // slot 72 (+0x120) (name descriptive)
    virtual void v73(); virtual void v74();
    virtual void SetRHSet(const CRightHandedSet& rh);  // slot 75 (+0x12c) (name descriptive)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                   // +0x20
    CInterfaceMap  m_interfaces;                       // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                         // +0x60
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        T* component;
        GetInterface(&component, interfaceType);
        return component;
    }
};
struct CTCLook {
    void*            __vftable;                        // +0x00
    CThing*          ParentThing;                      // +0x04
    unsigned char    _pad08[0x44 - 0x08];
    float            FractionOfTurningToAcceptEachFrame;   // +0x44
    unsigned char    _pad48[0x9c - 0x48];
    CAnimationEntry* PCurrentTurningAnim;              // +0x9c (PDB +0xa0)
    void SetBodyMovingViaAnimation();
};

void CTCLook::SetBodyMovingViaAnimation()
{
    CTCAnimationComplex* component = ParentThing->GetTC<CTCAnimationComplex>(0x5a);
    CAnimateThing& animation = component->GetAnimation();
    if (animation.Type == 7 &&
        component->GetAnimation().GetMeshBank()->PeekAnimation(animation.Anims[0])->IsLoaded()) {
        long anim = animation.Anims[0];
        float duration = component->GetAnimDuration(anim);
        float currentTime = duration * animation.CurrentPercentageComplete;
        float lastTime = duration * animation.LastPercentageComplete;
        CTCPhysicsBase* physics = ParentThing->PhysicsTC;
        const CRightHandedSet& current = physics->GetRHSet();
        C3DVector forward = current.Forward;
        C3DVector up = current.Up;
        float turn;
        PCurrentTurningAnim->GetTurnBetween(lastTime, currentTime, &turn);
        float angle = turn * FractionOfTurningToAcceptEachFrame;
        forward.Z = 0.0f;
        up = C3DVector(0.0f, 0.0f, 1.0f);
        forward.Rotate(up, angle);
        forward.Normalise();
        physics->SetRHSet(CRightHandedSet(up, forward));
    }
}

// ---- test stubs ----
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("CHECK failed line %d: %s\n", __LINE__, #c); fails++; } } while (0)
static int g_rotates, g_normalises, g_sets, g_peeks, g_durations, g_turns; static bool g_loaded;
static C3DVector g_rotAxis, g_rotFwd; static float g_rotAngle; static long g_peekIndex, g_durIndex; static float g_turnFrom, g_turnTo;
static CRightHandedSet g_current(C3DVector(0.1f, 0.2f, 0.9f), C3DVector(0.6f, 0.7f, 0.3f)), g_set(C3DVector(0, 0, 0), C3DVector(0, 0, 0));
void C3DVector::Rotate(const C3DVector& axis, float angle) { g_rotates++; g_rotAxis = axis; g_rotAngle = angle; g_rotFwd = *this; X += 1.0f; }
void C3DVector::Normalise() { g_normalises++; Y += 10.0f; }
static C3DAnimationInfo g_info;
bool C3DAnimationInfo::IsLoaded() const { return g_loaded; }
static CMeshDataBank g_bank;
C3DAnimationInfo* CMeshDataBank::PeekAnimation(long index) { g_peeks++; g_peekIndex = index; return &g_info; }
CMeshDataBank* CAnimateThing::GetMeshBank() { return &g_bank; }
static unsigned char g_animBuf[0x60];
CAnimateThing& CTCAnimationComplex::GetAnimation() { return *(CAnimateThing*)g_animBuf; }
float CTCAnimationComplex::GetAnimDuration(long index) { g_durations++; g_durIndex = index; return 4.0f; }
void CAnimationEntry::GetTurnBetween(float from, float to, float* turn) const { g_turns++; g_turnFrom = from; g_turnTo = to; *turn = 0.5f; }
#define V(n) void CTCPhysicsBase::v##n() {}
V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59)
V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69) V(70) V(71) V(73) V(74)
const CRightHandedSet& CTCPhysicsBase::GetRHSet() { return g_current; }
void CTCPhysicsBase::SetRHSet(const CRightHandedSet& rh) { g_sets++; g_set = rh; }
static void Reset() { g_rotates = g_normalises = g_sets = g_peeks = g_durations = g_turns = 0; g_peekIndex = g_durIndex = -1; }
static int Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }
static unsigned char g_thingBuf[0x100], g_lookBuf[sizeof(CTCLook)];
int main()
{
    CThing* thing = (CThing*)g_thingBuf; static CTCPhysicsBase physics; thing->PhysicsTC = &physics;
    static CTCAnimationComplex anim;
    CInterfaceMap::SEntry e[] = { { 0x10, 0 }, { 0x5a, &anim } };
    thing->m_presentBits[0x5a >> 5] |= 1UL << (0x5a & 31); thing->m_interfaces.first = e; thing->m_interfaces.last = e + 2;
    CAnimateThing* at = (CAnimateThing*)g_animBuf; static short idx[2] = { 9, 3 };
    at->Anims._M_start = idx; at->Anims._M_finish = idx + 2; at->Type = 7; at->CurrentPercentageComplete = 0.75f; at->LastPercentageComplete = 0.25f;
    static CAnimationEntry entry;
    CTCLook* look = (CTCLook*)g_lookBuf; look->ParentThing = thing; look->FractionOfTurningToAcceptEachFrame = 0.4f; look->PCurrentTurningAnim = &entry;
    g_loaded = true; Reset();
    look->SetBodyMovingViaAnimation();
    CHECK(g_peeks == 1 && g_peekIndex == 9 && g_durations == 1 && g_durIndex == 9);
    CHECK(g_turns == 1 && Near(g_turnFrom, 1.0f) && Near(g_turnTo, 3.0f));
    CHECK(g_rotates == 1 && Near(g_rotAngle, 0.2f) && g_rotAxis.X == 0.0f && g_rotAxis.Y == 0.0f && g_rotAxis.Z == 1.0f);
    CHECK(Near(g_rotFwd.X, 0.6f) && Near(g_rotFwd.Y, 0.7f) && g_rotFwd.Z == 0.0f && g_normalises == 1);
    CHECK(g_sets == 1 && g_set.Up.X == 0.0f && g_set.Up.Z == 1.0f && Near(g_set.Forward.X, 1.6f) && Near(g_set.Forward.Y, 10.7f) && g_set.Forward.Z == 0.0f);
    // first anim not loaded
    g_loaded = false; Reset(); look->SetBodyMovingViaAnimation();
    CHECK(g_peeks == 1 && g_durations == 0 && g_sets == 0);
    // not a turning set
    g_loaded = true; at->Type = 3; Reset(); look->SetBodyMovingViaAnimation();
    CHECK(g_peeks == 0 && g_sets == 0);
    if (fails == 0) printf("00753930_TEST PASS\n"); else printf("00753930_TEST FAIL\n");
    return fails;
}