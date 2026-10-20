// Behaviour test for 00543d30: body copied from src/00543d30.cpp (CParticleEffect), callees stubbed.
#include <cstdio>
#include <cstring>
static int g_baseInit = 0, g_baseChange = 0, g_creates = 0, g_layerCalls = 0, g_setCalls = 0; static long g_baseState = -1, g_layerDepth = -1, g_createEffect = -1;
static const void* g_createPos = 0; static long g_createLayer = -1; static bool g_createStart = false; static float g_axes[6];
struct C2DVector { float X, Y; };
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; CRightHandedSet() : Up(0.0f, 0.0f, 1.0f), Forward(0.0f, 1.0f, 0.0f) { } };
enum EEngine2DLayer { ENGINE_2D_LAYER_DEFAULT = 0 };
namespace NParticleEngine { class CParticleEmitter { public: int tag; }; }
using NParticleEngine::CParticleEmitter;
static CParticleEmitter g_emitter;
class CParticleEmitterManager {
public:
    static CParticleEmitterManager* Get() { static CParticleEmitterManager m; return &m; }
    CParticleEmitter* Create2DEmitter(long effect, const C2DVector* position, EEngine2DLayer layer, bool start, const CRightHandedSet& axes)
    { ++g_creates; g_createEffect = effect; g_createPos = position; g_createLayer = layer; g_createStart = start; memcpy(g_axes, &axes, sizeof(g_axes)); return &g_emitter; }
};
namespace NUISystem {
class CFrontEndManager {
public:
    static CFrontEndManager* GetInstance() { static CFrontEndManager m; return &m; }
    virtual EEngine2DLayer GetLayer(long depth) { ++g_layerCalls; g_layerDepth = depth; return (EEngine2DLayer)(depth + 10); }
};
struct CUIStateInfo { unsigned char _pad[0x1b]; bool HasParticleEffect; };
static CUIStateInfo g_info; static CUIStateInfo* g_infoFor[4];
class CChangingStateComponent {
public:
    virtual const CUIStateInfo* GetStateInfo(long state) { return state >= 0 && state < 4 ? g_infoFor[state] : 0; }
    void Initialise() { ++g_baseInit; }
    void ChangeState(long state) { ++g_baseChange; g_baseState = state; }
    C2DVector Position; char Depth; long CurrentState;
};
class CParticleEffect : public CChangingStateComponent {
public:
    CParticleEmitter* Emitter; long EffectIndex;
    __forceinline void CreateEmitter()
    {
        Emitter = CParticleEmitterManager::Get()->Create2DEmitter(EffectIndex, &Position,
                      CFrontEndManager::GetInstance()->GetLayer(Depth), true, CRightHandedSet());
    }
    virtual void Initialise();
    virtual void ChangeState(long state);
};
void CParticleEffect::Initialise()
{
    CChangingStateComponent::Initialise();
    if (GetStateInfo(CurrentState) && GetStateInfo(CurrentState)->HasParticleEffect && Emitter == 0)
        CreateEmitter();
}
void CParticleEffect::ChangeState(long state) { }
}
using namespace NUISystem;
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static void reset() { g_baseInit = g_baseChange = g_creates = g_layerCalls = 0; g_baseState = -1; g_layerDepth = -1; }
int main()
{
    CParticleEffect fx; memset(&fx.Position, 0, sizeof(fx.Position)); fx.Depth = 3; fx.CurrentState = 1; fx.Emitter = 0; fx.EffectIndex = 42;
    CUIStateInfo on; on.HasParticleEffect = true; CUIStateInfo off; off.HasParticleEffect = false;
    g_infoFor[0] = 0; g_infoFor[1] = &on; g_infoFor[2] = &off;
    // state with an effect and no emitter -> create one
    reset(); fx.Initialise();
    CHECK(g_baseInit == 1 && g_creates == 1 && fx.Emitter == &g_emitter && g_createEffect == 42 && g_createPos == &fx.Position && g_layerDepth == 3 && g_createLayer == 13 && g_createStart == true);
    CHECK(g_axes[0] == 0.0f && g_axes[1] == 0.0f && g_axes[2] == 1.0f && g_axes[3] == 0.0f && g_axes[4] == 1.0f && g_axes[5] == 0.0f);
    // emitter already running -> nothing
    reset(); fx.Initialise(); CHECK(g_baseInit == 1 && g_creates == 0);
    // state without effect -> nothing
    reset(); fx.Emitter = 0; fx.CurrentState = 2; fx.Initialise(); CHECK(g_creates == 0);
    // no state info -> nothing
    reset(); fx.CurrentState = 0; fx.Initialise(); CHECK(g_creates == 0 && g_baseInit == 1);
    if (fails == 0) printf("00543d30_TEST PASS\n"); else printf("00543d30_TEST FAIL\n");
    return fails;
}