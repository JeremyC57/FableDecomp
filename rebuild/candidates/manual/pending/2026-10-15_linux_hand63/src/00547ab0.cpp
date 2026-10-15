// NUISystem::CSwappingStateComponent::`scalar deleting destructor'(unsigned int) -- destructor inlined: the three vtables, the SwappingStates vector's storage freed, then the CChangingStateComponent base destructor; frees `this` when bit 0 of the flags is set
// Leaf: ??_GCSwappingStateComponent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00547AB0, length 71 bytes.
// Callees: free @ 0x00BFEA14 (the vector storage), NUISystem::CChangingStateComponent::~CChangingStateComponent()
//          @ 0x0052CCF0, operator delete @ 0x00BFE9BC.
// Vtables 0x012485AC / 0x01248584 / 0x0124857C = NUISystem::CSwappingStateComponent (rtti-vtables.tsv;
// this body is slot 0 of the primary one). Layout: PDB CSwappingStateComponent members 0x30 lower in
// the shipped build (CChangingStateComponent ends at +0x15c there, see the landed 0x00540DF0):
// SwappingStates (12-byte retail vector<pair<unsigned long,float> >) +0x15c, LastSwapTime +0x168,
// Seed +0x16c.
// Shape: `??_G` with the destructor inlined (`__forceinline virtual ~CSwappingStateComponent`); the
// vtable stores come first, the vector member's inline destructor frees its block under a null test,
// the base destructor runs out of line. The `new` factory (inline constructor) pulls the vtable and
// the deleting destructor into the TU.

extern "C" void __cdecl free(void* block);                                // 0x00BFEA14

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

struct CChangingStateComponent : CComponentRoot, IComponentA, IComponentB {
    unsigned char _body[0x15c - 0x1c];                                    // +0x1c .. +0x15c: the base's state
    CChangingStateComponent(long owner);                                  // 0x0052CC50
    virtual ~CChangingStateComponent();                                   // 0x0052CCF0
};

struct SSwappingState {                                                   // pair<unsigned long, float>
    unsigned long first;
    float         second;
};

struct CSwappingStateVector {                                             // retail vector<pair<unsigned long,float> > (12 bytes)
    SSwappingState* m_first;
    SSwappingState* m_last;
    SSwappingState* m_end;
    CSwappingStateVector() : m_first(0), m_last(0), m_end(0) {}
    ~CSwappingStateVector()
    {
        if (m_first)
            free(m_first);
    }
};

struct CSwappingStateComponent : CChangingStateComponent {                // vtables 0x012485AC / 0x01248584 / 0x0124857C
    CSwappingStateVector SwappingStates;                                  // +0x15c
    float                LastSwapTime;                                    // +0x168
    unsigned long        Seed;                                            // +0x16c
    CSwappingStateComponent(long owner) : CChangingStateComponent(owner), LastSwapTime(0.0f), Seed(0) {}
    __forceinline virtual ~CSwappingStateComponent() {}
    virtual void va00();
    virtual void vb00();
};

CSwappingStateComponent* NewSwappingStateComponent(long owner)
{
    return new CSwappingStateComponent(owner);
}

} // namespace NUISystem
