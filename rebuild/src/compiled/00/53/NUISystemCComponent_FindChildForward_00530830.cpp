// NUISystem::CComponent::FindChildForward(CCharString name) -- this component when its name matches, else the first non-null FindChildForward answer of the children, searched from the first child on
// Leaf: FindChildForward
// Retail: Fable.exe (TLC, VC7.1) @ 0x00530830, 184 bytes (192 with padding), `ret 4`.
// Callees: Compare 0x00411570 (inlined CCharString ==), CCharString copy ctor 0x0099EC30, ~CCharString 0x0099EAE0,
//   the child's FindChildForward through vtable slot 4 (+0x10).
// Layout: as FindChild 0x00530780 (Name +0x20, Children +0xb0).
// Shape: unsigned index, size() recomputed for every test (`jbe` entry, `jb` loop) after the virtual call.
class CCharStringData { public:                                        // the shared string block
    const char*   m_pData;                                      // +0x00
    unsigned long m_length;                                     // +0x04
};
template <class T> class CBasicString { public:                       // Ego_r.pdb: the string base of CCharString (data and inline comparisons)
    CCharStringData* PStringData;
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 (Ego_r.pdb ?Compare@?$CBasicString@D@@KIJPBD0@Z, protected static)
public:
    bool operator==(const CBasicString& other) const             // inlined: same block, or equal lengths and Compare == 0
    {
        CCharStringData* a = PStringData;
        CCharStringData* b = other.PStringData;
        if (a == b)
            return true;
        if (a && b && a->m_length == b->m_length)
            return Compare(a->m_pData, b->m_pData) == 0;
        return false;
    }
};

class CCharString : public CBasicString<char> { public:
    CCharString(const CCharString& other);                      // 0x0099EC30
    ~CCharString();                                             // 0x0099EAE0
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

CComponent* CComponent::FindChildForward(CCharString name)
{
    if (Name == name)
        return this;
    for (unsigned long i = 0; i < Children.size(); ++i) {
        CComponent* found = Children[i].m_pObject->FindChildForward(name);
        if (found)
            return found;
    }
    return 0;
}