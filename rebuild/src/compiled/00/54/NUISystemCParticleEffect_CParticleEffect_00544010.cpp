// NUISystem::CParticleEffect::CParticleEffect(long owner) -- CChangingStateComponent(owner), the three vtables of this class, the effect pointer at +0x15c nulled, then GetDataFromDef()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00544010, length 54 bytes.
// Callees: NUISystem::CChangingStateComponent::CChangingStateComponent(long) @ 0x0052CC50, GetDataFromDef @ 0x00543FA0 (on `this`, in the constructor body).
// Vtables 0x0124735C / 0x01247338 / 0x01247330 (rtti-vtables.tsv: NUISystem::CParticleEffect): the class overrides virtuals of its primary base and of the two interface
// bases at +4 and +0x18, so its constructor stores three vtables right after the base constructor returns. The bases' sizes are not in
// the PDB; the base is modelled as an opaque block up to the first own member. The chunk label "CDraggableInto::CDraggableInto" names the
// third body of the three; the other two are its ancestors (rtti-vtables.tsv).
namespace NUISystem {

struct IComponentA {                             // the interface base at +0x04 (vtable, 0x10 bytes of state)
    virtual void va00();
    unsigned char _state[0x10];
};

struct IComponentB {                             // the interface base at +0x18 (vtable only)
    virtual void vb00();
};

struct CComponentRoot {                          // the primary base: vptr at +0
    virtual void v00();
};

struct CChangingStateComponent : CComponentRoot, IComponentA, IComponentB {
    unsigned char _body[0x15c - 0x1c];      // +0x1c .. +0x15c: the base's state
    CChangingStateComponent(long owner);                            // 0x0052CC50
};

struct CParticleEffect : CChangingStateComponent {                        // vtables 0x0124735C / 0x01247338 / 0x01247330
    virtual void v00();
    virtual void va00();
    virtual void vb00();
    void* m_pEffect;                           // +0x15c
    void GetDataFromDef();                     // 0x00543FA0
    CParticleEffect(long owner);
};

CParticleEffect::CParticleEffect(long owner)
    : CChangingStateComponent(owner), m_pEffect(0)
{
    GetDataFromDef();
}

}