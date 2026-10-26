// NUISystem::CParticleEffect::`scalar deleting destructor'(unsigned int) -- destructor body inlined: the three vtables, the owned particle emitter killed and the pointer cleared, then the CChangingStateComponent base destructor; frees `this` when bit 0 of the flags is set
// Leaf: ??_GCParticleEffect
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005440B0, length 77 bytes.
// Callees: CParticleEmitter::Kill() @ 0x00AEE8C0 (on the held emitter),
//          NUISystem::CChangingStateComponent::~CChangingStateComponent() @ 0x0052CCF0,
//          operator delete @ 0x00BFE9BC.
// Vtables 0x0124735C / 0x01247338 / 0x01247330 = NUISystem::CParticleEffect (rtti-vtables.tsv; this
// body is slot 0 of the primary one). The model follows the landed constructor 0x00544010
// (NUISystemCParticleEffect_CParticleEffect_00544010.cpp): primary base at +0, interface bases at
// +4 and +0x18, the effect pointer at +0x15c.
// Shape: `??_G` with the destructor inlined (`__forceinline virtual ~CParticleEffect`): the member
// load is hoisted above the three vtable stores, the kill + null store sit under one null test,
// the base destructor is out of line. The `new CParticleEffect(owner)` factory (inline constructor)
// pulls the vtable -- and so the deleting destructor -- into the TU.

namespace NUISystem {

struct IComponentA {                                                      // the interface base at +0x04 (vtable, 0x10 bytes of state)
    virtual void va00();
    unsigned char _state[0x10];
};

struct IComponentB {                                                      // the interface base at +0x18 (vtable only)
    virtual void vb00();
};

struct CComponentRoot {                                                   // the primary base: vptr at +0
    virtual ~CComponentRoot();
};

class CChangingStateComponent : public CComponentRoot, public IComponentA, public IComponentB { public:
    unsigned char _body[0x15c - 0x1c];                                    // +0x1c .. +0x15c: the base's state
    CChangingStateComponent(long owner);                                  // 0x0052CC50
    virtual ~CChangingStateComponent();                                   // 0x0052CCF0
};

} // namespace NUISystem

struct CParticleEmitter {
    void Kill();                                                          // 0x00AEE8C0
};

namespace NUISystem {

class CParticleEffect : public CChangingStateComponent { public:                        // vtables 0x0124735C / 0x01247338 / 0x01247330
    CParticleEmitter* m_pEffect;                                          // +0x15c
    CParticleEffect(long owner) : CChangingStateComponent(owner), m_pEffect(0) {}
    __forceinline virtual ~CParticleEffect()
    {
        if (m_pEffect) {
            m_pEffect->Kill();
            m_pEffect = 0;
        }
    }
    virtual void va00();
    virtual void vb00();
};

CParticleEffect* NewParticleEffect(long owner)
{
    return new CParticleEffect(owner);
}

} // namespace NUISystem