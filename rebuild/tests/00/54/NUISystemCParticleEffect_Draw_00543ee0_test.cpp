// Behaviour test for 00543ee0: src/00543ee0.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. NUISystem::CParticleEffect::Draw(long, long, long, long, long)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NUISystem::CParticleEffect::Draw(long, long, long, long, long) -- keep the running emitter on the component: pick the front-end layer for its depth (offset by the draw depth unless the component is independent or suppressed) and move the emitter to its position, rescaled from the display to the virtual screen size when that override is on
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00543EE0, length 192 bytes (`ret 14h`). Virtual: slot 2 of
// NUISystem::CParticleEffect's vtable 0x0124735C.
// Callees: virtual slots 101 / 104 (bool queries), CManager::GetInstance @ 0x0041E5F2 (x2) and its slot 36
//          GetLayer, NParticleEngine::CParticleEmitter::Set2DPosition @ 0x00AED090.
// Globals: the screen-size override flag 0x013B8768 with the override width/height 0x013B876C/0x013B8770 and
//          the display width/height 0x01375CD4/0x01375CD8 (the getters @ 0x0041CC14/0x0041CC2B read the same).
// Layout (no PDB record for NUISystem::CParticleEffect): +0x54 the component's 2D position, +0x12f its depth
// (signed char), +0x144 the current state, +0x15c the emitter, +0x160 the particle effect index; the state
// info returned by virtual slot 135 carries the "has a particle effect" flag at +0x1b.
// Shared helper: CreateEmitter() -- create a 2D emitter at the component's position on the front-end layer
// for its depth, with the default axes (Up (0,0,1), Forward (0,1,0)); callers start it only when the state's
// info exists, wants an effect and none is running. The manager getter is called after the arguments are pushed (VC
// evaluates the object expression of a member call last), the axes are built between the pushes.
// Shape: the position is a copy of the component's and each axis is rescaled through an inline helper written as
// `float r; if (override) r = (v / display) * virtual; else r = v; return r;` reading the copy (X's load forwards to the
// member, Y's comes from the copied word). The if/else keeps a real store in the untaken arm, so VC7.1 re-tests the flag
// for Y and shares one fstp per axis; a `?:` helper lets it drop Y's self-assignment and jump-thread the second test.
class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CRightHandedSet { public:                           // PDB: Up then Forward; the default set is Z up, Y forward
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet() : Up(0.0f, 0.0f, 1.0f), Forward(0.0f, 1.0f, 0.0f) { }
};
enum EEngine2DLayer { ENGINE_2D_LAYER_DEFAULT = 0 };

namespace NParticleEngine {
class CParticleEmitter {
public:
    void Set2DPosition(const C2DVector* position, EEngine2DLayer layer);                         // 0x00AED090
};
}
using NParticleEngine::CParticleEmitter;

class CParticleEmitterManager {
public:
    static CParticleEmitterManager* Get();                                                       // 0x00AEAAF0 (returns the instance @ 0x013D2E1C)
    CParticleEmitter* Create2DEmitter(long effect, const C2DVector* position, EEngine2DLayer layer,
                                      bool start, const CRightHandedSet* axes);                 // 0x00AEB790
};

namespace NUISystem {
class CManager {
public:
    static CManager* __fastcall GetInstance();                                           // 0x0041E5F2
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
    virtual EEngine2DLayer GetLayer(long depth);                                                 // slot 36 (+0x90)
};

struct CUIStateInfo {
    unsigned char _pad00[0x1b];
    bool          HasParticleEffect;                                                             // +0x1b
};

class CChangingStateComponent {                    // polymorphic root (retail vtable at +0)
public:
    virtual void v000();
    virtual void v001();
    virtual void Draw(long target, long a, long layerOffset, long c, long component);            // slot 2
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
    virtual void Initialise();                                                          // slot 43: CChangingStateComponent::Initialise @ 0x0052C730
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void ChangeState(long state);                                               // slot 48: CChangingStateComponent::ChangeState @ 0x0052CF40
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
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual bool IsIndependent();                                                       // slot 101 (+0x194)
    virtual void v102();
    virtual void v103();
    virtual bool IsSuppressed_slot104();                                                // slot 104 (+0x1a0)
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void v127();
    virtual void v128();
    virtual void v129();
    virtual void v130();
    virtual void v131();
    virtual void v132();
    virtual void v133();
    virtual void v134();
    virtual const CUIStateInfo* GetStateInfo(long state);                               // slot 135 (+0x21c)
    unsigned char _body[0x50];                     // +0x04
    C2DVector     Position;                        // +0x54
    unsigned char _pad5c[0xd3];                    // +0x5c
    char          Depth;                           // +0x12f
    unsigned char _pad130[0x14];                   // +0x130
    long          CurrentState;                    // +0x144
    unsigned char _pad148[0x14];                   // +0x148
};

class CParticleEffect : public CChangingStateComponent {   // vtable 0x0124735C
public:
    CParticleEmitter* Emitter;                     // +0x15c
    long              EffectIndex;                 // +0x160
    __forceinline void CreateEmitter()
    {
        CRightHandedSet axes;
        Emitter = CParticleEmitterManager::Get()->Create2DEmitter(EffectIndex, &Position,
                      CManager::GetInstance()->GetLayer(Depth), true, &axes);
    }
    virtual void Initialise();
    virtual void ChangeState(long state);
    virtual void Draw(long target, long a, long layerOffset, long c, long component);
};

extern bool  g_bScreenSizeOverride;                // 0x013B8768
extern float g_OverrideScreenWidth;                // 0x013B876C
extern float g_OverrideScreenHeight;               // 0x013B8770
extern float g_DisplayWidth;                       // 0x01375CD4
extern float g_DisplayHeight;                      // 0x01375CD8
static inline float ScaleScreenX(float x) { float r; if (g_bScreenSizeOverride) r = (x / g_DisplayWidth) * g_OverrideScreenWidth; else r = x; return r; }
static inline float ScaleScreenY(float y) { float r; if (g_bScreenSizeOverride) r = (y / g_DisplayHeight) * g_OverrideScreenHeight; else r = y; return r; }


void CParticleEffect::Draw(long target, long a, long layerOffset, long c, long component)
{
    if (Emitter) {
        bool independent = IsIndependent();
        bool suppressed = IsSuppressed_slot104();
        EEngine2DLayer layer;
        if (!independent && !suppressed)
            layer = CManager::GetInstance()->GetLayer(Depth + layerOffset);
        else
            layer = CManager::GetInstance()->GetLayer(Depth);
        C2DVector position = Position;
        position.X = ScaleScreenX(position.X);
        position.Y = ScaleScreenY(position.Y);
        Emitter->Set2DPosition(&position, layer);
    }
}
}

// ---- stubs and driver ----
#include <math.h>
namespace NParticleEngine {
static int g_setCalls; static float g_setX, g_setY; static long g_setLayer; static const CParticleEmitter* g_setThis;
void CParticleEmitter::Set2DPosition(const C2DVector* p, EEngine2DLayer layer)
{ ++g_setCalls; g_setThis = this; g_setX = p->X; g_setY = p->Y; g_setLayer = (long)layer; }
}
CParticleEmitterManager* CParticleEmitterManager::Get() { return 0; }
CParticleEmitter* CParticleEmitterManager::Create2DEmitter(long, const C2DVector*, EEngine2DLayer, bool, const CRightHandedSet*) { return 0; }
namespace NUISystem {
bool  g_bScreenSizeOverride;
float g_OverrideScreenWidth, g_OverrideScreenHeight, g_DisplayWidth, g_DisplayHeight;
static int g_instCalls, g_layerCalls; static long g_layerDepth;
static CManager s_fem;
CManager* __fastcall CManager::GetInstance() { ++g_instCalls; return &s_fem; }
EEngine2DLayer CManager::GetLayer(long depth) { ++g_layerCalls; g_layerDepth = depth; return (EEngine2DLayer)(1000 + depth); }
void CManager::v000() {}
void CManager::v001() {}
void CManager::v002() {}
void CManager::v003() {}
void CManager::v004() {}
void CManager::v005() {}
void CManager::v006() {}
void CManager::v007() {}
void CManager::v008() {}
void CManager::v009() {}
void CManager::v010() {}
void CManager::v011() {}
void CManager::v012() {}
void CManager::v013() {}
void CManager::v014() {}
void CManager::v015() {}
void CManager::v016() {}
void CManager::v017() {}
void CManager::v018() {}
void CManager::v019() {}
void CManager::v020() {}
void CManager::v021() {}
void CManager::v022() {}
void CManager::v023() {}
void CManager::v024() {}
void CManager::v025() {}
void CManager::v026() {}
void CManager::v027() {}
void CManager::v028() {}
void CManager::v029() {}
void CManager::v030() {}
void CManager::v031() {}
void CManager::v032() {}
void CManager::v033() {}
void CManager::v034() {}
void CManager::v035() {}
void CChangingStateComponent::v000() {}
void CChangingStateComponent::v001() {}
void CChangingStateComponent::v003() {}
void CChangingStateComponent::v004() {}
void CChangingStateComponent::v005() {}
void CChangingStateComponent::v006() {}
void CChangingStateComponent::v007() {}
void CChangingStateComponent::v008() {}
void CChangingStateComponent::v009() {}
void CChangingStateComponent::v010() {}
void CChangingStateComponent::v011() {}
void CChangingStateComponent::v012() {}
void CChangingStateComponent::v013() {}
void CChangingStateComponent::v014() {}
void CChangingStateComponent::v015() {}
void CChangingStateComponent::v016() {}
void CChangingStateComponent::v017() {}
void CChangingStateComponent::v018() {}
void CChangingStateComponent::v019() {}
void CChangingStateComponent::v020() {}
void CChangingStateComponent::v021() {}
void CChangingStateComponent::v022() {}
void CChangingStateComponent::v023() {}
void CChangingStateComponent::v024() {}
void CChangingStateComponent::v025() {}
void CChangingStateComponent::v026() {}
void CChangingStateComponent::v027() {}
void CChangingStateComponent::v028() {}
void CChangingStateComponent::v029() {}
void CChangingStateComponent::v030() {}
void CChangingStateComponent::v031() {}
void CChangingStateComponent::v032() {}
void CChangingStateComponent::v033() {}
void CChangingStateComponent::v034() {}
void CChangingStateComponent::v035() {}
void CChangingStateComponent::v036() {}
void CChangingStateComponent::v037() {}
void CChangingStateComponent::v038() {}
void CChangingStateComponent::v039() {}
void CChangingStateComponent::v040() {}
void CChangingStateComponent::v041() {}
void CChangingStateComponent::v042() {}
void CChangingStateComponent::v044() {}
void CChangingStateComponent::v045() {}
void CChangingStateComponent::v046() {}
void CChangingStateComponent::v047() {}
void CChangingStateComponent::v049() {}
void CChangingStateComponent::v050() {}
void CChangingStateComponent::v051() {}
void CChangingStateComponent::v052() {}
void CChangingStateComponent::v053() {}
void CChangingStateComponent::v054() {}
void CChangingStateComponent::v055() {}
void CChangingStateComponent::v056() {}
void CChangingStateComponent::v057() {}
void CChangingStateComponent::v058() {}
void CChangingStateComponent::v059() {}
void CChangingStateComponent::v060() {}
void CChangingStateComponent::v061() {}
void CChangingStateComponent::v062() {}
void CChangingStateComponent::v063() {}
void CChangingStateComponent::v064() {}
void CChangingStateComponent::v065() {}
void CChangingStateComponent::v066() {}
void CChangingStateComponent::v067() {}
void CChangingStateComponent::v068() {}
void CChangingStateComponent::v069() {}
void CChangingStateComponent::v070() {}
void CChangingStateComponent::v071() {}
void CChangingStateComponent::v072() {}
void CChangingStateComponent::v073() {}
void CChangingStateComponent::v074() {}
void CChangingStateComponent::v075() {}
void CChangingStateComponent::v076() {}
void CChangingStateComponent::v077() {}
void CChangingStateComponent::v078() {}
void CChangingStateComponent::v079() {}
void CChangingStateComponent::v080() {}
void CChangingStateComponent::v081() {}
void CChangingStateComponent::v082() {}
void CChangingStateComponent::v083() {}
void CChangingStateComponent::v084() {}
void CChangingStateComponent::v085() {}
void CChangingStateComponent::v086() {}
void CChangingStateComponent::v087() {}
void CChangingStateComponent::v088() {}
void CChangingStateComponent::v089() {}
void CChangingStateComponent::v090() {}
void CChangingStateComponent::v091() {}
void CChangingStateComponent::v092() {}
void CChangingStateComponent::v093() {}
void CChangingStateComponent::v094() {}
void CChangingStateComponent::v095() {}
void CChangingStateComponent::v096() {}
void CChangingStateComponent::v097() {}
void CChangingStateComponent::v098() {}
void CChangingStateComponent::v099() {}
void CChangingStateComponent::v100() {}
void CChangingStateComponent::v102() {}
void CChangingStateComponent::v103() {}
void CChangingStateComponent::v105() {}
void CChangingStateComponent::v106() {}
void CChangingStateComponent::v107() {}
void CChangingStateComponent::v108() {}
void CChangingStateComponent::v109() {}
void CChangingStateComponent::v110() {}
void CChangingStateComponent::v111() {}
void CChangingStateComponent::v112() {}
void CChangingStateComponent::v113() {}
void CChangingStateComponent::v114() {}
void CChangingStateComponent::v115() {}
void CChangingStateComponent::v116() {}
void CChangingStateComponent::v117() {}
void CChangingStateComponent::v118() {}
void CChangingStateComponent::v119() {}
void CChangingStateComponent::v120() {}
void CChangingStateComponent::v121() {}
void CChangingStateComponent::v122() {}
void CChangingStateComponent::v123() {}
void CChangingStateComponent::v124() {}
void CChangingStateComponent::v125() {}
void CChangingStateComponent::v126() {}
void CChangingStateComponent::v127() {}
void CChangingStateComponent::v128() {}
void CChangingStateComponent::v129() {}
void CChangingStateComponent::v130() {}
void CChangingStateComponent::v131() {}
void CChangingStateComponent::v132() {}
void CChangingStateComponent::v133() {}
void CChangingStateComponent::v134() {}
static bool g_independent, g_suppressed; static int g_queryCalls;
bool CChangingStateComponent::IsIndependent() { ++g_queryCalls; return g_independent; }
bool CChangingStateComponent::IsSuppressed_slot104() { ++g_queryCalls; return g_suppressed; }
const CUIStateInfo* CChangingStateComponent::GetStateInfo(long) { return 0; }
void CChangingStateComponent::Initialise() {}
void CChangingStateComponent::ChangeState(long) {}
void CChangingStateComponent::Draw(long, long, long, long, long) {}
void CParticleEffect::Initialise() {}
void CParticleEffect::ChangeState(long) {}
}
using namespace NUISystem;
using NParticleEngine::g_setCalls; using NParticleEngine::g_setX; using NParticleEngine::g_setY;
using NParticleEngine::g_setLayer; using NParticleEngine::g_setThis;
static bool Near(float a, float b) { return fabs(a - b) <= 1e-5f * (fabs(b) + 1.0f); }
static void Reset() { g_setCalls = g_instCalls = g_layerCalls = g_queryCalls = 0; g_setX = g_setY = -12345.0f; g_setLayer = -1; g_layerDepth = -999; }
int main()
{
    // C2DVector has no default constructor, so the component is copy-constructed from zeroed storage (the implicit
    // copy constructor installs CParticleEffect's vtable and copies the zero members)
    static double raw[sizeof(CParticleEffect) / sizeof(double) + 1];
    static CParticleEffect e(*(const CParticleEffect*)raw);
    CParticleEmitter* em = (CParticleEmitter*)0x1234;
    e.Position.X = 320.0f; e.Position.Y = 240.0f; e.Depth = 7;
    g_DisplayWidth = 640.0f; g_DisplayHeight = 480.0f; g_OverrideScreenWidth = 1024.0f; g_OverrideScreenHeight = 768.0f;
    // no emitter: nothing is queried or moved
    Reset(); e.Emitter = 0; e.Draw(0, 0, 5, 0, 0);
    CHECK(g_queryCalls == 0 && g_instCalls == 0 && g_setCalls == 0);
    // running emitter, normal component, no override: layer for depth + offset, position unchanged
    Reset(); e.Emitter = em; g_independent = g_suppressed = false; g_bScreenSizeOverride = false;
    e.Draw(0, 0, 5, 0, 0);
    CHECK(g_queryCalls == 2 && g_instCalls == 1 && g_layerCalls == 1 && g_layerDepth == 12);
    CHECK(g_setCalls == 1 && g_setThis == em && g_setLayer == 1012 && g_setX == 320.0f && g_setY == 240.0f);
    // independent: the offset is ignored
    Reset(); g_independent = true; e.Draw(0, 0, 5, 0, 0);
    CHECK(g_layerCalls == 1 && g_layerDepth == 7 && g_setLayer == 1007 && g_setCalls == 1);
    // suppressed: the offset is ignored too
    Reset(); g_independent = false; g_suppressed = true; e.Draw(0, 0, 5, 0, 0);
    CHECK(g_layerCalls == 1 && g_layerDepth == 7 && g_setLayer == 1007 && g_setCalls == 1);
    // negative depth is a signed char
    Reset(); g_suppressed = false; e.Depth = -3; e.Draw(0, 0, 5, 0, 0);
    CHECK(g_layerDepth == 2 && g_setLayer == 1002);
    e.Depth = 7;
    // override on: both axes rescaled from the display to the virtual screen
    Reset(); g_bScreenSizeOverride = true; e.Draw(0, 0, -2, 0, 0);
    CHECK(g_layerDepth == 5 && g_setCalls == 1);
    CHECK(Near(g_setX, 512.0f) && Near(g_setY, 384.0f));
    // distinct axis scales catch a swapped width/height
    Reset(); e.Position.X = 100.0f; e.Position.Y = 50.0f; g_DisplayWidth = 800.0f; g_DisplayHeight = 200.0f;
    g_OverrideScreenWidth = 400.0f; g_OverrideScreenHeight = 1000.0f; e.Draw(0, 0, 0, 0, 0);
    CHECK(Near(g_setX, 50.0f) && Near(g_setY, 250.0f));
    // the component's own position is left alone
    CHECK(e.Position.X == 100.0f && e.Position.Y == 50.0f);
    // override off again: the raw position
    Reset(); g_bScreenSizeOverride = false; e.Draw(0, 0, 0, 0, 0);
    CHECK(g_setX == 100.0f && g_setY == 50.0f);
    if (fails == 0) printf("00543ee0_TEST PASS\n");
    return fails;
}
