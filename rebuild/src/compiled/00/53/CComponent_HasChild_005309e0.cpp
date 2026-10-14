// NUISystem::CComponent::HasChild(CComponent*) -- true for the component itself or when any child (searched newest first) has it in its subtree
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005309E0, length 81 bytes (+ int3 padding).
// Callees: vtable slot 10 (+0x28) = HasChild on each child component (the recursion through the
// child's own override). Slot 10 of every NUISystem component vtable (0x0122F5FC .. 0x0124C254 in
// rtti-vtables.tsv) is this body. Children is the std::vector<CCountedPointer<CComponent>> at
// +0xb0 in retail (Ego_r.pdb +0xb8: the States map before it is 4 bytes smaller in the shipped
// binary and the retail vector itself is 12 bytes), 8-byte elements {object, count block}, hence
// the `_Retail` model. The scan runs from the last child down to index -1; the element is reached
// through the retail-vector iterator object (`*(begin() + i)`), which is why `first` is reloaded
// from the member on every pass instead of being kept in a register across the virtual call.
template<class T> struct CRetailVector {                        // retail std::vector: {first, last, end} with an iterator OBJECT
    T* first; T* last; T* end_of_storage;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) { }
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() { return iterator(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](long n) { return *(begin() + n); }
};

struct CComponent;

struct CCountedComponentPointer {                               // CCountedPointer<NUISystem::CComponent>
    CComponent* m_pObject;                                      // +0
    void*       m_pCount;                                       // +4
};

struct CComponent {                                             // retail NUISystem component view
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual bool HasChild(CComponent* child);                   // slot 10 (+0x28)
    unsigned char _body[0xb0 - 0x04];                           // +0x04..+0xaf (CObserver base, name, states, transforms)
    CRetailVector<CCountedComponentPointer> Children;           // +0xb0
};

bool CComponent::HasChild(CComponent* child)
{
    if (this == child)
        return true;
    for (long i = Children.size() - 1; i != -1; --i) {
        bool found = Children[i].m_pObject->HasChild(child);
        if (found)
            return found;                                       // the child's own answer is returned as is (al from the call)
    }
    return false;
}