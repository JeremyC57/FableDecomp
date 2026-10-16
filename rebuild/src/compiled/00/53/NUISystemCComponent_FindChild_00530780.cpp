// NUISystem::CComponent::FindChild(CCharString name) -- this component when its name matches, else the first non-null FindChild answer of the children, searched from the last child back
// Leaf: FindChild
// Retail: Fable.exe (TLC, VC7.1) @ 0x00530780, 168 bytes (176 with padding), `ret 4` (the string is by value, destroyed here).
// Callees: CCharString equality inlined around Compare 0x00411570 (__fastcall), CCharString copy ctor 0x0099EC30 (the
//   by-value argument built in its stack slot), ~CCharString 0x0099EAE0, the child's FindChild through vtable slot 3 (+0xc).
// Layout: retail NUISystem::CComponent as in the landed 0x005304C0: Name +0x20, Children (12-byte vector of 8-byte
//   counted pointers) +0xb0; no engine header is generated for this namespaced class.
// Shape: the index counts down from size()-1 to -1 (`dec ; cmp -1`); `first` is re-read through the STLport
//   iterator after each virtual call.
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

CComponent* CComponent::FindChild(CCharString name)
{
    if (Name == name)
        return this;
    for (long i = (long)Children.size() - 1; i != -1; --i) {
        CComponent* found = Children[i].m_pObject->FindChild(name);
        if (found)
            return found;
    }
    return 0;
}