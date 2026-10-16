// Grade: DIFFER(184v189) -- register/loop shape: retail keeps name.PStringData in edi for the whole body (loaded before the first compare), caches first in ebx and walks an element pointer down by 8; ours reloads the argument and indexes [edi+esi*8] | tried: operator[] index loop, cached-first local, childName reference | next: lane gap: register allocation / strength reduction
// NUISystem::CComponent::FindChildNonRecursive(CCharString name) -- this component when its name matches, else the last direct child whose name matches (no recursion)
// Leaf: FindChildNonRecursive
// Retail: Fable.exe (TLC, VC7.1) @ 0x005308F0, 185 bytes (192 with padding), `ret 4`.
// Callees: Compare 0x00411570 (inlined CCharString ==, twice), ~CCharString 0x0099EAE0.
// Layout: as FindChild 0x00530780 (Name +0x20, Children +0xb0).
// Shape: the child's name is read through the inline GetName() reference (`add eax,20h ; mov eax,[eax]`);
//   `first` is held in a local (read once, before `last`), the index counts down from size()-1 to -1.
struct CCharStringData {                                        // the shared string block
    const char*   m_pData;                                      // +0x00
    unsigned long m_length;                                     // +0x04
};
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570

struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                      // 0x0099EC30
    ~CCharString();                                             // 0x0099EAE0
    bool operator==(const CCharString& other) const             // inlined: same block, or equal lengths and Compare == 0
    {
        CCharStringData* a = PStringData;
        CCharStringData* b = other.PStringData;
        if (a == b)
            return true;
        if (a && b && a->m_length == b->m_length)
            return Compare_00411570(a->m_pData, b->m_pData) == 0;
        return false;
    }
};

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

struct CComponent {                                             // retail NUISystem::CComponent view (as the landed 0x005304C0)
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual CComponent* FindChild(CCharString name);            // slot 3 (+0x0c)
    virtual CComponent* FindChildForward(CCharString name);     // slot 4 (+0x10)
    unsigned char _body04[0x20 - 0x04];                         // +0x04..+0x1f
    CCharString   Name;                                         // +0x20
    unsigned char _body24[0xb0 - 0x24];                         // +0x24..+0xaf
    CRetailVector<CCountedComponentPointer> Children;           // +0xb0
    const CCharString& GetName() const { return Name; }
    CComponent* FindChildNonRecursive(CCharString name);
};

CComponent* CComponent::FindChildNonRecursive(CCharString name)
{
    if (Name == name)
        return this;
    CCountedComponentPointer* children = Children.first;
    for (long i = (long)(Children.last - children) - 1; i != -1; --i) {
        if (children[i].m_pObject->GetName() == name)
            return children[i].m_pObject;
    }
    return 0;
}
