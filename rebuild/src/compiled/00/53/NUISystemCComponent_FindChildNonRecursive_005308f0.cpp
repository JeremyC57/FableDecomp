// NUISystem::CComponent::FindChildNonRecursive(CCharString name) -- this component when its name matches, else the last direct child whose name matches (no recursion)
// Leaf: FindChildNonRecursive
// Retail: Fable.exe (TLC, VC7.1) @ 0x005308F0, 185 bytes (192 with padding), `ret 4`.
// Callees: Compare 0x00411570 (inlined CCharString ==, twice), ~CCharString 0x0099EAE0.
// Layout: as FindChild 0x00530780 (Name +0x20, Children +0xb0).
// Shape: Compare gets its landed body as a same-TU `auto_inline(off)` stand-in, so VC7.1 keeps the by-value name's
//   block in a register for the whole body. The inline CCharString == is the MsgIsKilledBy 0x008D04E0 form (one
//   identity/null/length condition, then `Compare(...) ? false : true`). The child's name is read through the inline
//   GetName() reference (`add eax,20h ; mov eax,[eax]`); the loop is `for (long i = size() - 1; i != -1; --i)` over
//   `Children[i]` (STLport `*(begin() + n)`), strength-reduced to an element pointer walking down by 8.
struct CCharStringData {                                        // the shared string block
    const char*   m_pData;                                      // +0x00
    unsigned long m_length;                                     // +0x04
};
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body, same-TU stand-in)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                      // 0x0099EC30
    ~CCharString();                                             // 0x0099EAE0
    bool operator==(const CCharString& other) const             // inlined: same block, or equal lengths and Compare == 0
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare_00411570(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
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
    for (long i = (long)Children.size() - 1; i != -1; --i) {
        if (Children[i].m_pObject->GetName() == name)
            return Children[i].m_pObject;
    }
    return 0;
}