// Behaviour test for the NUISystem::CParticleEffect deleting destructor (0x005440B0): the held
// emitter is killed (only when present) before the base destructor runs, and delete frees it.
#include <cstdio>

static int g_events[8];
static int g_count = 0;

namespace NUISystem {

struct IComponentA { virtual void va00() {} unsigned char _state[0x10]; };
struct IComponentB { virtual void vb00() {} };
struct CComponentRoot { virtual ~CComponentRoot() {} };

class CChangingStateComponent : public CComponentRoot, public IComponentA, public IComponentB { public:
    unsigned char _body[0x15c - 0x1c];
    CChangingStateComponent(long) {}
    virtual ~CChangingStateComponent() { g_events[g_count++] = 2; }
};

} // namespace NUISystem

struct CParticleEmitter {
    int m_killed;
    void Kill() { ++m_killed; g_events[g_count++] = 1; }
};

namespace NUISystem {

class CParticleEffect : public CChangingStateComponent { public:
    CParticleEmitter* m_pEffect;
    CParticleEffect(long owner) : CChangingStateComponent(owner), m_pEffect(0) {}
    virtual ~CParticleEffect()
    {
        if (m_pEffect) {
            m_pEffect->Kill();
            m_pEffect = 0;
        }
    }
    virtual void va00() {}
    virtual void vb00() {}
};

} // namespace NUISystem

int main()
{
    CParticleEmitter emitter;
    emitter.m_killed = 0;
    NUISystem::CParticleEffect* a = new NUISystem::CParticleEffect(1);
    a->m_pEffect = &emitter;
    NUISystem::CComponentRoot* root = a;
    delete root;
    bool ok = emitter.m_killed == 1 && g_count == 2 && g_events[0] == 1 && g_events[1] == 2;
    NUISystem::CParticleEffect* b = new NUISystem::CParticleEffect(2);
    delete b;
    ok = ok && emitter.m_killed == 1 && g_count == 3 && g_events[2] == 2;
    if (!ok) {
        std::printf("005440b0_TEST FAIL\n");
        return 1;
    }
    std::printf("005440b0_TEST PASS\n");
    return 0;
}