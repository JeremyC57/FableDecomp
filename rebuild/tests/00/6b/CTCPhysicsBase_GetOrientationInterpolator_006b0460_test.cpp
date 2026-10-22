// Behaviour test for 006b0460: src/006b0460.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CTCPhysicsBase::GetOrientationInterpolator(C3DOrientationInterpolator& out) const
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCPhysicsBase::GetOrientationInterpolator(C3DOrientationInterpolator& out) const -- the orientation interpolator from the last frame's set to the current one; under bullet time for a thing it affects, the pair is instead two sets interpolated between them at the current and next sub-step of the bullet-time cycle
// Leaf: ?GetOrientationInterpolator@CTCPhysicsBase@@QBEXAAVC3DOrientationInterpolator@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B0460, 310 bytes, `ret 4`.
// Callees: CTCBase::GetWorld() const 0x00686A80 (twice), CThing::IsAffectedByBulletTime 0x004C9920, CBulletTime::GetTimeScale
//   0x004C5FA0, CWorld::GetWorldFrame 0x0049D870 (static), _ftol 0x00BFEA70, CRightHandedSet::Interpolate(const
//   CRightHandedSet&, float) const 0x00A23190 (hidden result), C3DOrientationInterpolator(const CRightHandedSet&, const
//   CRightHandedSet&) 0x005508D0, C3DOrientationInterpolator::operator= 0x006B0E00 (landed), ~CBase 0x0099A2E0, the
//   physics virtual slots 72 (+0x120, the current right-handed set, as the landed 0x006B0850) and 73 (+0x124, the
//   previous frame's set; name descriptive). Global 1.0f 0x0122DED8.
// Layout: CTCBase ParentThing +0x04; CWorld bullet-time controller +0x68 (active flag +0x0c, controller +0x04, as the
//   landed CTCAnimatingObject::FrameUpdate 0x007F8E20); C3DOrientationInterpolator 0x34 bytes (CBase vptr, start +0x04,
//   end +0x1c).
// Shape: plain source order: the step is (frame - 1) % (long)scale, the step size 1 / scale, the two sets interpolated
//   from the old set towards the current one (the current set's getter runs first as the right-most argument), and the
//   result assigned from an interpolator temporary in each branch.
class CThing;

struct C3DVector { float X, Y, Z; };
struct CRightHandedSet {
    C3DVector Up, Forward;
    CRightHandedSet Interpolate(const CRightHandedSet& other, float t) const;   // 0x00A23190
};
struct CBase {
    virtual void v00();
    ~CBase();                                       // 0x0099A2E0
};
struct C3DOrientationInterpolator : CBase {         // 0x34 bytes
    CRightHandedSet StartOrientation;               // +0x04
    CRightHandedSet EndOrientation;                 // +0x1c
    C3DOrientationInterpolator(const CRightHandedSet& start, const CRightHandedSet& end);   // 0x005508D0
    C3DOrientationInterpolator& operator=(const C3DOrientationInterpolator& other);         // 0x006B0E00
};

struct CBulletTime {
    unsigned char _pad0[4];
    void*         m_pController;                    // +0x04
    unsigned char _pad1[4];
    bool          m_active;                         // +0x0c
    float GetTimeScale();                           // 0x004C5FA0
    bool IsActive() const { return m_active || m_pController != 0; }
};
struct CWorld {
    unsigned char _pad[0x68];
    CBulletTime*  m_pBulletTime;                    // +0x68
    static long GetWorldFrame();                    // 0x0049D870
};
class CThing {
public:
    bool IsAffectedByBulletTime() const;            // 0x004C9920
};

class CTCPhysicsBase {
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
    virtual const CRightHandedSet& GetRightHandedSet() const;      // slot 72 (+0x120)
    virtual const CRightHandedSet& GetOldRightHandedSet() const;   // slot 73 (+0x124)
    CThing* ParentThing;                            // +0x04 (CTCBase)
    const CWorld* GetWorld() const;                 // 0x00686A80 (CTCBase)
    void GetOrientationInterpolator(C3DOrientationInterpolator& out) const;
};

void CTCPhysicsBase::GetOrientationInterpolator(C3DOrientationInterpolator& out) const
{
    if (GetWorld()->m_pBulletTime->IsActive() && ParentThing->IsAffectedByBulletTime()) {
        float timeScale = GetWorld()->m_pBulletTime->GetTimeScale();
        long step = (CWorld::GetWorldFrame() - 1) % (long)timeScale;
        float stepSize = 1.0f / timeScale;
        CRightHandedSet start = GetOldRightHandedSet().Interpolate(GetRightHandedSet(), step * stepSize);
        ++step;
        CRightHandedSet end = GetOldRightHandedSet().Interpolate(GetRightHandedSet(), step * stepSize);
        out = C3DOrientationInterpolator(start, end);
    } else {
        out = C3DOrientationInterpolator(GetOldRightHandedSet(), GetRightHandedSet());
    }
}

// ---- stubs ----
static char g_log[512];
static int g_step = 0;
static void Log(const char* s) { char b[64]; _snprintf(b, sizeof b, "%d:%s;", ++g_step, s); strcat(g_log, b); }
void CTCPhysicsBase::v000() {}
void CTCPhysicsBase::v001() {}
void CTCPhysicsBase::v002() {}
void CTCPhysicsBase::v003() {}
void CTCPhysicsBase::v004() {}
void CTCPhysicsBase::v005() {}
void CTCPhysicsBase::v006() {}
void CTCPhysicsBase::v007() {}
void CTCPhysicsBase::v008() {}
void CTCPhysicsBase::v009() {}
void CTCPhysicsBase::v010() {}
void CTCPhysicsBase::v011() {}
void CTCPhysicsBase::v012() {}
void CTCPhysicsBase::v013() {}
void CTCPhysicsBase::v014() {}
void CTCPhysicsBase::v015() {}
void CTCPhysicsBase::v016() {}
void CTCPhysicsBase::v017() {}
void CTCPhysicsBase::v018() {}
void CTCPhysicsBase::v019() {}
void CTCPhysicsBase::v020() {}
void CTCPhysicsBase::v021() {}
void CTCPhysicsBase::v022() {}
void CTCPhysicsBase::v023() {}
void CTCPhysicsBase::v024() {}
void CTCPhysicsBase::v025() {}
void CTCPhysicsBase::v026() {}
void CTCPhysicsBase::v027() {}
void CTCPhysicsBase::v028() {}
void CTCPhysicsBase::v029() {}
void CTCPhysicsBase::v030() {}
void CTCPhysicsBase::v031() {}
void CTCPhysicsBase::v032() {}
void CTCPhysicsBase::v033() {}
void CTCPhysicsBase::v034() {}
void CTCPhysicsBase::v035() {}
void CTCPhysicsBase::v036() {}
void CTCPhysicsBase::v037() {}
void CTCPhysicsBase::v038() {}
void CTCPhysicsBase::v039() {}
void CTCPhysicsBase::v040() {}
void CTCPhysicsBase::v041() {}
void CTCPhysicsBase::v042() {}
void CTCPhysicsBase::v043() {}
void CTCPhysicsBase::v044() {}
void CTCPhysicsBase::v045() {}
void CTCPhysicsBase::v046() {}
void CTCPhysicsBase::v047() {}
void CTCPhysicsBase::v048() {}
void CTCPhysicsBase::v049() {}
void CTCPhysicsBase::v050() {}
void CTCPhysicsBase::v051() {}
void CTCPhysicsBase::v052() {}
void CTCPhysicsBase::v053() {}
void CTCPhysicsBase::v054() {}
void CTCPhysicsBase::v055() {}
void CTCPhysicsBase::v056() {}
void CTCPhysicsBase::v057() {}
void CTCPhysicsBase::v058() {}
void CTCPhysicsBase::v059() {}
void CTCPhysicsBase::v060() {}
void CTCPhysicsBase::v061() {}
void CTCPhysicsBase::v062() {}
void CTCPhysicsBase::v063() {}
void CTCPhysicsBase::v064() {}
void CTCPhysicsBase::v065() {}
void CTCPhysicsBase::v066() {}
void CTCPhysicsBase::v067() {}
void CTCPhysicsBase::v068() {}
void CTCPhysicsBase::v069() {}
void CTCPhysicsBase::v070() {}
void CTCPhysicsBase::v071() {}

static CRightHandedSet g_cur, g_old;
const CRightHandedSet& CTCPhysicsBase::GetRightHandedSet() const { Log("Cur"); return g_cur; }
const CRightHandedSet& CTCPhysicsBase::GetOldRightHandedSet() const { Log("Old"); return g_old; }
// Interpolate stub: Up.X = this.Up.X + (other.Up.X - this.Up.X) * t, everything else copied from this; records t
static float g_ts[4]; static int g_nts = 0;
CRightHandedSet CRightHandedSet::Interpolate(const CRightHandedSet& other, float t) const
{ Log("Interp"); g_ts[g_nts++] = t; CRightHandedSet r = *this; r.Up.X = Up.X + (other.Up.X - Up.X) * t; return r; }
void CBase::v00() {}
static int g_bases = 0;
CBase::~CBase() { --g_bases; }
C3DOrientationInterpolator::C3DOrientationInterpolator(const CRightHandedSet& start, const CRightHandedSet& end)
{ ++g_bases; StartOrientation = start; EndOrientation = end; }
C3DOrientationInterpolator& C3DOrientationInterpolator::operator=(const C3DOrientationInterpolator& other)
{ Log("Assign"); StartOrientation = other.StartOrientation; EndOrientation = other.EndOrientation; return *this; }
static float g_scale = 4.0f;
float CBulletTime::GetTimeScale() { Log("Scale"); return g_scale; }
static long g_frame = 0;
long CWorld::GetWorldFrame() { Log("Frame"); return g_frame; }
static bool g_affected = true;
bool CThing::IsAffectedByBulletTime() const { Log("Affected"); return g_affected; }
static CBulletTime g_bt;
static CWorld g_world;
const CWorld* CTCPhysicsBase::GetWorld() const { Log("GetWorld"); return &g_world; }

static void Reset() { g_log[0] = 0; g_step = 0; g_nts = 0; }

int main()
{
    g_world.m_pBulletTime = &g_bt; memset(&g_bt, 0, sizeof g_bt);
    memset(&g_cur, 0, sizeof g_cur); memset(&g_old, 0, sizeof g_old);
    g_old.Up.X = 10.0f; g_cur.Up.X = 18.0f; g_old.Forward.Z = 3.0f; g_cur.Forward.Z = 7.0f;
    static CThing thing;
    CTCPhysicsBase phys; phys.ParentThing = &thing;
    C3DOrientationInterpolator out(g_cur, g_cur); g_bases = 0;
    // 1: no bullet time: old -> current
    {
        Reset();
        phys.GetOrientationInterpolator(out);
        CHECK(strcmp(g_log, "1:GetWorld;2:Cur;3:Old;4:Assign;") == 0);
        CHECK(out.StartOrientation.Up.X == 10.0f && out.StartOrientation.Forward.Z == 3.0f);
        CHECK(out.EndOrientation.Up.X == 18.0f && out.EndOrientation.Forward.Z == 7.0f && g_bases == 0);
    }
    // 2: bullet time active but the thing is not affected: same as 1
    {
        Reset(); g_bt.m_active = true; g_affected = false;
        phys.GetOrientationInterpolator(out);
        CHECK(strcmp(g_log, "1:GetWorld;2:Affected;3:Cur;4:Old;5:Assign;") == 0);
        CHECK(out.StartOrientation.Up.X == 10.0f && out.EndOrientation.Up.X == 18.0f);
    }
    // 3: bullet time (controller set), affected, scale 4, frame 7: step (7-1)%4 = 2 -> t 0.5 and 0.75
    {
        Reset(); g_bt.m_active = false; g_bt.m_pController = &g_bt; g_affected = true; g_frame = 7;
        phys.GetOrientationInterpolator(out);
        CHECK(strcmp(g_log, "1:GetWorld;2:Affected;3:GetWorld;4:Scale;5:Frame;6:Cur;7:Old;8:Interp;9:Cur;10:Old;11:Interp;12:Assign;") == 0);
        CHECK(g_nts == 2 && g_ts[0] == 0.5f && g_ts[1] == 0.75f);
        CHECK(out.StartOrientation.Up.X == 14.0f && out.EndOrientation.Up.X == 16.0f);
        CHECK(out.StartOrientation.Forward.Z == 3.0f && g_bases == 0);
    }
    // 4: frame 1, scale 2: step 0 -> t 0 and 0.5
    {
        Reset(); g_frame = 1; g_scale = 2.0f;
        phys.GetOrientationInterpolator(out);
        CHECK(g_nts == 2 && g_ts[0] == 0.0f && g_ts[1] == 0.5f && out.StartOrientation.Up.X == 10.0f && out.EndOrientation.Up.X == 14.0f);
    }
    if (fails == 0) printf("006b0460_TEST PASS\n");
    return fails;
}