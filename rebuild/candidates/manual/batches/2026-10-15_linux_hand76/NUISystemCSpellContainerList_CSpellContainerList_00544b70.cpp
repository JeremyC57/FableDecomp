// NUISystem::CSpellContainerList::CSpellContainerList(long owner) -- CChangingStateComponent(owner), the three vtables of this class, SpellSelectionChild / SpellContainers / InputDelay cleared, then GetDataFromDef()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00544B70, length 78 bytes.
// Callees: NUISystem::CChangingStateComponent::CChangingStateComponent(long) @ 0x0052CC50 (manifest label
// CDraggableInto, a sibling's name), GetDataFromDef @ 0x00544480 (on `this`, in the constructor body).
// Vtables 0x012475C4 / 0x012475A0 / 0x01247598 (rtti-vtables.tsv: NUISystem::CSpellContainerList) at +0, +4,
// +0x18: the class overrides virtuals of its primary base and of the two interface bases, as the landed
// sibling NUISystem::CParticleEffect (0x00544010), so three vtable stores follow the base constructor.
// Layout: the PDB (Ego_r, 476 bytes) lists LastInputTime first at +0x18c; the retail base ends at +0x15c
// (CParticleEffect's first member) and the retail vector is 12 bytes, so LastInputTime +0x15c, SpellIndex
// +0x160, SelectedChild +0x164, SpellSelectionChild +0x168, SpellIndicationChild +0x16c, SpellContainers
// +0x170, SpellToAssign +0x17c, PositionOffset +0x180, AlphaOffset +0x188, NextSpellContainerAlpha +0x18c,
// NextSpellContainerPosition +0x190, PositionOffsetToUse +0x198, AlphaOffsetToUse +0x1a0, InputDelay +0x1a4
// -- which puts the five retail zero stores on SpellSelectionChild, the vector's three words and InputDelay.
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
    unsigned char _body[0x15c - 0x1c];          // +0x1c .. +0x15c: the base's state
    CChangingStateComponent(long owner);        // 0x0052CC50
};

struct CComponent;

template <class T>
struct CRetailVector {                          // VC6-era Dinkumware vector: first / last / end
    T* first;
    T* last;
    T* end;
    CRetailVector() : first(0), last(0), end(0) { }
};

struct C2DVector { float x, y; };

struct CSpellContainerList : CChangingStateComponent {    // vtables 0x012475C4 / 0x012475A0 / 0x01247598
    virtual void v00();
    virtual void va00();
    virtual void vb00();
    float                      LastInputTime;                   // +0x15c
    unsigned long              SpellIndex;                      // +0x160
    unsigned long              SelectedChild;                   // +0x164
    CComponent*                SpellSelectionChild;             // +0x168
    CComponent*                SpellIndicationChild;            // +0x16c
    CRetailVector<CComponent*> SpellContainers;                 // +0x170
    long                       SpellToAssign;                   // +0x17c
    C2DVector                  PositionOffset;                  // +0x180
    float                      AlphaOffset;                     // +0x188
    float                      NextSpellContainerAlpha;         // +0x18c
    C2DVector                  NextSpellContainerPosition;      // +0x190
    C2DVector                  PositionOffsetToUse;             // +0x198
    float                      AlphaOffsetToUse;                // +0x1a0
    float                      InputDelay;                      // +0x1a4
    void GetDataFromDef();                      // 0x00544480
    CSpellContainerList(long owner);
};

CSpellContainerList::CSpellContainerList(long owner)
    : CChangingStateComponent(owner), SpellSelectionChild(0), InputDelay(0)
{
    GetDataFromDef();
}

}
