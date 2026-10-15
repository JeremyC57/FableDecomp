// NUISystem::CComponent::FindFirstChildOfTypeNonRecursive(long type) -- this component if its type matches, else the first direct child of that type, else null
// Retail: Fable.exe (TLC, VC7.1) @ 0x00530610, 113 bytes (128 with padding).
// Callees: GetType through vtable slot 65 (`call [eax+104h]`) on this and on each child.
// The found child is fetched again from the vector after the loop exit (`mov edx,[esi+0B0h] ;
// mov eax,[edx+edi*8]`): the return re-reads the element instead of keeping the tested pointer.
// Children is the retail 12-byte std::vector<CCountedPointer<CComponent>> at +0xb0 (Ego_r.pdb +0xb8:
// the States map before it is smaller in the shipped binary), 8-byte elements {object, count block};
// as in the landed CComponent rows (0x00530460, 0x005309E0) the element is reached through the
// retail iterator object (`*(begin() + i)`), so `first` is re-read after every virtual call and the
// size is recomputed (`(last - first) >> 3`) for each loop test; the index is unsigned (`jb`).
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
    T& operator[](unsigned long n) { return *(begin() + n); }    // STLport operator[] = *(begin() + n)
};

struct CComponent;

struct CCountedComponentPointer {                               // CCountedPointer<NUISystem::CComponent>
    CComponent* m_pObject;                                      // +0
    void*       m_pCount;                                       // +4
};

struct CComponent {                                             // retail NUISystem::CComponent view
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void v048();
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual long GetType();                                  // slot 65 (+0x104)
    unsigned char _body[0xb0 - 0x04];                           // +0x04..+0xaf
    CRetailVector<CCountedComponentPointer> Children;           // +0xb0
    CComponent* FindFirstChildOfTypeNonRecursive(long type);
};

CComponent* CComponent::FindFirstChildOfTypeNonRecursive(long type)
{
    if (GetType() == type)
        return this;
    for (unsigned long i = 0; i < Children.size(); ++i) {
        if (Children[i].m_pObject->GetType() == type)
            return Children[i].m_pObject;
    }
    return 0;
}