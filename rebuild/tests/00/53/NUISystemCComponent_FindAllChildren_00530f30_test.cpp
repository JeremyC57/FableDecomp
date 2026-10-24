// Behaviour test for 00530f30: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

class CCharString {
public:
    const char* PData;
    CCharString(const char* s) : PData(s) { }
    CCharString(const CCharString& o) : PData(o.PData) { }          // stub for 0x0099EC30
    ~CCharString() { }                                              // stub for 0x0099EAE0
    bool operator==(const CCharString& o) const { return std::strcmp(PData, o.PData) == 0; }   // inline, Compare 0x00411570
};
template <class T> struct CRetailList {                             // the found list (a fixed array stand-in)
    T items[16];
    int count;
    CRetailList() : count(0) { }
    void push_back(const T& value) { items[count++] = value; }
};
namespace NUISystem { class CComponent; }
struct CCountedComponentPointer {
    NUISystem::CComponent* m_pObject;
    void*                  m_pCount;
};
template <class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return first[n]; }
};
namespace NUISystem {
class CComponent {
public:
    CCharString Name;
    CRetailVector<CCountedComponentPointer> Children;
    CCountedComponentPointer Storage[4];
    CComponent(const char* name) : Name(name)
    {
        Children.first = Children.last = Children.end_of_storage = Storage;
    }
    void Add(CComponent* child)
    {
        Children.last->m_pObject = child;
        Children.last->m_pCount = 0;
        ++Children.last;
    }
    virtual ~CComponent() { }
    virtual void FindAllChildren(CCharString name, CRetailList<CComponent*>& found)
    {
        if (Name == name) {
            found.push_back(this);
        } else {
            for (unsigned long i = 0; i < Children.size(); ++i)
                Children[i].m_pObject->FindAllChildren(name, found);
        }
    }
};
}
using NUISystem::CComponent;
static void RunTest()
{
    // root -> { button(a), panel -> { button(b), label, button(c) -> { button(d) } } }
    CComponent root("root"), a("button"), panel("panel"), b("button"), label("label"), c("button"), d("button");
    root.Add(&a); root.Add(&panel);
    panel.Add(&b); panel.Add(&label); panel.Add(&c);
    c.Add(&d);
    CRetailList<CComponent*> found;
    root.FindAllChildren(CCharString("button"), found);
    // depth-first, and a match does not descend into its own children (d stays unfound)
    CHECK(found.count == 3 && found.items[0] == &a && found.items[1] == &b && found.items[2] == &c);
    CRetailList<CComponent*> self;
    root.FindAllChildren(CCharString("root"), self);
    CHECK(self.count == 1 && self.items[0] == &root);
    CRetailList<CComponent*> none;
    root.FindAllChildren(CCharString("missing"), none);
    CHECK(none.count == 0);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("00530f30_TEST FAIL\n");
        return 1;
    }
    std::printf("00530f30_TEST PASS\n");
    return 0;
}