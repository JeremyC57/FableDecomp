// NUISystem::CParticleEffect::Draw(long, long, long, long, long) -- keep the running emitter on the component: pick the front-end layer for its depth (offset by the draw depth unless the component is independent or suppressed) and move the emitter to its position, rescaled from the display to the virtual screen size when that override is on
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00543EE0, length 192 bytes (`ret 14h`). Virtual: slot 2 of
// NUISystem::CParticleEffect's vtable 0x0124735C.
// Callees: virtual slots 101 / 104 (bool queries), CFrontEndManager::GetInstance @ 0x0041E5F2 (x2) and its slot 36
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
struct C2DVector {
    float X, Y;
    C2DVector(const C2DVector& other) : X(other.X), Y(other.Y) { }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CRightHandedSet {                           // PDB: Up then Forward; the default set is Z up, Y forward
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
class CFrontEndManager {
public:
    static CFrontEndManager* __fastcall GetInstance();                                           // 0x0041E5F2
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
    virtual void Draw(long target, long a, long layerOffset, long c, long d);            // slot 2
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
                      CFrontEndManager::GetInstance()->GetLayer(Depth), true, &axes);
    }
    virtual void Initialise();
    virtual void ChangeState(long state);
    virtual void Draw(long target, long a, long layerOffset, long c, long d);
};

extern bool  g_bScreenSizeOverride;                // 0x013B8768
extern float g_OverrideScreenWidth;                // 0x013B876C
extern float g_OverrideScreenHeight;               // 0x013B8770
extern float g_DisplayWidth;                       // 0x01375CD4
extern float g_DisplayHeight;                      // 0x01375CD8
static inline float ScaleScreenX(float x) { float r; if (g_bScreenSizeOverride) r = (x / g_DisplayWidth) * g_OverrideScreenWidth; else r = x; return r; }
static inline float ScaleScreenY(float y) { float r; if (g_bScreenSizeOverride) r = (y / g_DisplayHeight) * g_OverrideScreenHeight; else r = y; return r; }


void CParticleEffect::Draw(long target, long a, long layerOffset, long c, long d)
{
    if (Emitter) {
        bool independent = IsIndependent();
        bool suppressed = IsSuppressed_slot104();
        EEngine2DLayer layer;
        if (!independent && !suppressed)
            layer = CFrontEndManager::GetInstance()->GetLayer(Depth + layerOffset);
        else
            layer = CFrontEndManager::GetInstance()->GetLayer(Depth);
        C2DVector position = Position;
        position.X = ScaleScreenX(position.X);
        position.Y = ScaleScreenY(position.Y);
        Emitter->Set2DPosition(&position, layer);
    }
}
}