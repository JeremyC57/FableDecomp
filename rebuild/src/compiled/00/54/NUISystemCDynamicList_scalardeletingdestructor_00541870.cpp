// NUISystem::CDynamicList::`scalar deleting destructor'(unsigned int) -- destructor inlined: the three vtables, the ScrollingChildren vector's storage freed, then the CChangingStateComponent base destructor; frees `this` when bit 0 of the flags is set
// Leaf: ??_GCDynamicList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00541870, length 71 bytes.
// Callees: free @ 0x00BFEA14 (the vector storage), NUISystem::CChangingStateComponent::~CChangingStateComponent()
//          @ 0x0052CCF0, operator delete @ 0x00BFE9BC.
// Vtables 0x01246E44 / 0x01246E20 / 0x01246E18 = NUISystem::CDynamicList (rtti-vtables.tsv; this body
// is slot 0 of the primary one). Layout: PDB CDynamicList members 0x30 lower in the shipped build
// (the landed Initialise 0x00540DF0 pins BInitialised at +0x15c): BInitialised +0x15c,
// PositionOffset +0x160, TopIndex +0x168, SelectionIndex +0x16c, NumVisible +0x170,
// ScrollingChildren (12-byte retail vector) +0x174.
// Shape: `??_G` with the destructor inlined (`__forceinline virtual ~CDynamicList`); the vtable
// stores come first, the vector member's inline destructor frees its block under a null test
// (`push eax ; call free ; add esp,4`), the base destructor runs out of line. The `new` factory
// (inline constructor) pulls the vtable and the deleting destructor into the TU.

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

struct CComponent;

struct CComponentPtrVector {                                              // retail vector<CComponent*> (12 bytes)
    CComponent** m_first;
    CComponent** m_last;
    CComponent** m_end;
    CComponentPtrVector() : m_first(0), m_last(0), m_end(0) {}
    ~CComponentPtrVector()
    {
        if (m_first)
            free(m_first);
    }
};

struct C2DVector {
    float X;
    float Y;
};

struct CDynamicList : CChangingStateComponent {                           // vtables 0x01246E44 / 0x01246E20 / 0x01246E18
    long                BInitialised;                                     // +0x15c
    C2DVector           PositionOffset;                                   // +0x160
    long                TopIndex;                                         // +0x168
    long                SelectionIndex;                                   // +0x16c
    long                NumVisible;                                       // +0x170
    CComponentPtrVector ScrollingChildren;                                // +0x174
    CDynamicList(long owner) : CChangingStateComponent(owner), BInitialised(0) {}
    __forceinline virtual ~CDynamicList() {}
    virtual void va00();
    virtual void vb00();
};

CDynamicList* NewDynamicList(long owner)
{
    return new CDynamicList(owner);
}

} // namespace NUISystem