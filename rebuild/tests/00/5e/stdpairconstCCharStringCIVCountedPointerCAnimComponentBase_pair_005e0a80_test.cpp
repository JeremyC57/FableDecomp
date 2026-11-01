// Behaviour test for the implicit std::pair<const CCharString, CIVCountedPointer<CAnimComponentBase> >::~pair 0x005E0A80: release
// the component (Delete at the last reference) BEFORE destroying the name
#include <cstdio>
static int g_order[8];
static int g_count = 0;
class CIVCountedPointeeBase {
public:
    CIVCountedPointeeBase() : IVRefCount(0) {}
    virtual ~CIVCountedPointeeBase() {}
    virtual void Delete() const { g_order[g_count++] = 100; }
    long Release() const { return --IVRefCount; }
    mutable unsigned long IVRefCount;
};
class CCharString {
public:
    CCharString() {}
    ~CCharString() { g_order[g_count++] = 1; }
};
class CAnimComponentBase;
template <class T> class CIVCountedPointer {
public:
    CIVCountedPointer() : Object(0) {}
    ~CIVCountedPointer() { Release(); }
    void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CIVCountedPointeeBase* Object;
};
namespace std_model {
template <class _T1, class _T2> struct pair {
    pair() : first(), second() {}
    _T1 first;
    _T2 second;
};
}
int main()
{
    CIVCountedPointeeBase component;
    component.IVRefCount = 1;
    {
        std_model::pair<const CCharString, CIVCountedPointer<CAnimComponentBase> > value;
        value.second.Object = &component;
    }
    if (g_count != 2 || g_order[0] != 100 || g_order[1] != 1 || component.IVRefCount != 0) {
        std::printf("005e0a80_TEST FAIL\n");
        return 1;
    }
    component.IVRefCount = 3;
    {
        std_model::pair<const CCharString, CIVCountedPointer<CAnimComponentBase> > shared;
        shared.second.Object = &component;
    }
    if (g_count != 3 || g_order[2] != 1 || component.IVRefCount != 2) { std::printf("005e0a80_TEST FAIL\n"); return 1; }
    std::printf("005e0a80_TEST PASS\n");
    return 0;
}