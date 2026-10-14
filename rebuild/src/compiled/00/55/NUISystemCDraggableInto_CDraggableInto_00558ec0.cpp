// NUISystem::CDraggableInto::CDraggableInto(long owner) -- CHoverable(owner), the three vtables of this class, the target pointer at +0x16c nulled, then Initialize()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00558EC0, length 54 bytes.
// Callees: NUISystem::CHoverable::CHoverable(long) @ 0x0055BA20, Initialize @ 0x00558E10 (on `this`, in the constructor body; manifest "CDraggableInto::Initialize").
// Vtables 0x0124B2D4 / 0x0124B2B0 / 0x0124B2A8 (rtti-vtables.tsv: NUISystem::CDraggableInto): the class overrides virtuals of its primary base and of the two interface
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

struct CHoverable : CComponentRoot, IComponentA, IComponentB {
    unsigned char _body[0x16c - 0x1c];      // +0x1c .. +0x16c: the base's state
    CHoverable(long owner);                            // 0x0055BA20
};

struct CDraggableInto : CHoverable {                        // vtables 0x0124B2D4 / 0x0124B2B0 / 0x0124B2A8
    virtual void v00();
    virtual void va00();
    virtual void vb00();
    void* m_pTarget;                           // +0x16c
    void Initialize();                         // 0x00558E10
    CDraggableInto(long owner);
};

CDraggableInto::CDraggableInto(long owner)
    : CHoverable(owner), m_pTarget(0)
{
    Initialize();
}

}