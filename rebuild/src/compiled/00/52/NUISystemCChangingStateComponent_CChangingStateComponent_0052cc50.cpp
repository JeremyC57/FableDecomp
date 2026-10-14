// NUISystem::CChangingStateComponent::CChangingStateComponent(long owner) -- CComponent(owner), the three vtables of this class (primary, +4, +0x18), then the state list at +0x13c: one self-linked sentinel node
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052CC50, length 68 bytes.
// Callees: NUISystem::CComponent::CComponent(long) @ 0x005334A0, malloc @ 0x00BFEA0E (0xc-byte sentinel node; `add esp,4` after it).
// Vtables 0x01245DE4 / 0x01245DBC / 0x01245DB4 (rtti-vtables.tsv: NUISystem::CChangingStateComponent): the class overrides virtuals of its primary base and of the two interface
// bases at +4 and +0x18, so its constructor stores three vtables right after the base constructor returns. The bases' sizes are not in
// the PDB; the base is modelled as an opaque block up to the first own member. The chunk label "CDraggableInto::CDraggableInto" names the
// third body of the three; the other two are its ancestors (rtti-vtables.tsv).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct CRetailList_node0xc {                     // VC6 Dinkumware list with 4-byte values: one sentinel node pointer
    struct node { node* next; node* prev; long value; };
    node* head;
    CRetailList_node0xc()
        : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));   // linked to itself before it is stored
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

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

struct CComponent : CComponentRoot, IComponentA, IComponentB {
    unsigned char _body[0x13c - 0x1c];      // +0x1c .. +0x13c: the base's state
    CComponent(long owner);                            // 0x005334A0
};

struct CChangingStateComponent : CComponent {                        // vtables 0x01245DE4 / 0x01245DBC / 0x01245DB4
    virtual void v00();
    virtual void va00();
    virtual void vb00();
    CRetailList_node0xc m_stateList;           // +0x13c (list<long>: one sentinel pointer)
    CChangingStateComponent(long owner);
};

CChangingStateComponent::CChangingStateComponent(long owner)
    : CComponent(owner), m_stateList()
{
}

}