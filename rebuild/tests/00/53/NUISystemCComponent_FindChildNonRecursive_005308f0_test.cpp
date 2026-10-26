// Behaviour test for 005308f0: src/005308f0.cpp copied verbatim; returns this component when its own name matches,
// else the LAST direct child whose name matches (searched from the end), else null; no recursion.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
class CCharStringData { public:                                        // the shared string block
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

class CCharString { public:
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
// ---- end source ----
static int g_copies, g_dtors;
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
CCharString::~CCharString() { ++g_dtors; }
void CComponent::v000() {} void CComponent::v001() {} void CComponent::v002() {}
CComponent* CComponent::FindChild(CCharString) { return 0; }
CComponent* CComponent::FindChildForward(CCharString) { return 0; }
static CCharStringData Make(const char* t) { CCharStringData d; d.m_pData = t; d.m_length = (unsigned long)strlen(t); return d; }
static CComponent* Comp(long* raw, CCharStringData* name) { memset(raw, 0, 0xc0); CComponent* c = (CComponent*)raw; c->Name.PStringData = name; return c; }
static CComponent* Find(CComponent* c, CCharStringData* name) { CCharStringData* slot = name; return c->FindChildNonRecursive(*(CCharString*)&slot); }
int main()
{
    CCharStringData root = Make("Root"), a = Make("Button"), b = Make("Label"), a2 = Make("Button"), q = Make("Butto?");
    static long rr[0x30], r0[0x30], r1[0x30], r2[0x30];
    CComponent* parent = Comp(rr, &root); CComponent* c0 = Comp(r0, &a); CComponent* c1 = Comp(r1, &b); CComponent* c2 = Comp(r2, &a2);
    CCountedComponentPointer kids[3] = { { c0, 0 }, { c1, 0 }, { c2, 0 } };
    parent->Children.first = kids; parent->Children.last = kids + 3; parent->Children.end_of_storage = kids + 3;
    CCharStringData rootCopy = Make("Root"), button = Make("Button"), label = Make("Label");
    CHECK(Find(parent, &rootCopy) == parent);
    CHECK(Find(parent, &button) == c2);                  // last match wins (searched from the end)
    CHECK(Find(parent, &label) == c1);
    CHECK(Find(parent, &q) == 0);
    CHECK(Find(parent, &a) == c2);                       // c2's text equals c0's block text, so c2 is still found first
    parent->Children.last = kids; CHECK(Find(parent, &label) == 0);
    CHECK(g_copies == g_dtors && g_copies == 6);   // one by-value name per call, destroyed by the callee
    if (fails == 0) printf("005308f0_TEST PASS\n"); else printf("005308f0_TEST FAIL\n");
    return fails;
}