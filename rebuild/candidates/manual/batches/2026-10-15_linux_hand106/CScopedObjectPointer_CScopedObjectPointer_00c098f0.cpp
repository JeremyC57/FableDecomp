// CScopedObjectPointer::~CScopedObjectPointer() -- destructor of a one-pointer owner: deletes the owned polymorphic object through its virtual deleting destructor and clears the pointer
// Leaf: ??1CScopedObjectPointer
// Retail: Fable.exe @ 0x00C098F0, 24 bytes. Callee: the owned object's scalar deleting destructor (vtable slot 6, `push 1 ; call [eax+18h]`).
// Label: the manifest's "CTexture::Uninitialise" is propagated (CTexture::Uninitialise is 0x009F9F70). The only reference
// is a `jmp` from an EH unwind funclet (0x01210AB3), i.e. a stack object's destructor; its neighbour 0x00C098E0 is the
// matching `CScopedObjectPointer(T* p) : m_pObject(p) {}` constructor. The owning class could not be established, so the
// owner is named for what it does.
// Shape: `if (m_pObject) { delete m_pObject; m_pObject = 0; }` -- the clear stays inside the branch; the object's
// virtual destructor sits in slot 6 of its vtable.
struct CDeletableObject {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual ~CDeletableObject();                   // slot 6 (+0x18)
};

struct CScopedObjectPointer {
    CDeletableObject* m_pObject;                   // +0x00
    ~CScopedObjectPointer();
};

CScopedObjectPointer::~CScopedObjectPointer()
{
    if (m_pObject) {
        delete m_pObject;
        m_pObject = 0;
    }
}
