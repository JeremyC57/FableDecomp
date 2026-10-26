// Behaviour test for 00533cc0: a copy of the source's model and logic with its out-of-line callees stubbed.
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

#include <new>
class CCPPointerInfo { public: long Count; };
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* d, CCPPointerInfo* i) : Data(d), Info(i) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info)
    {
        if (Info)
            ++Info->Count;
    }
    T* operator->() const { return Data; }
};
struct __false_type { __false_type() {} };
static int g_overflowCalls = 0;
template <class T> class CCountedVector {
public:
    CCountedPointer<T>* _M_start;
    CCountedPointer<T>* _M_finish;
    CCountedPointer<T>* _M_end_of_storage;
    void _M_insert_overflow(CCountedPointer<T>* position, const CCountedPointer<T>& x, const __false_type&,
                            unsigned long fillLength, bool atEnd)          // stub for 0x005359D0: grow by 4
    {
        ++g_overflowCalls;
        CHECK(position == _M_finish && fillLength == 1 && atEnd);
        unsigned long n = (unsigned long)(_M_finish - _M_start);
        CCountedPointer<T>* p = (CCountedPointer<T>*)std::malloc((n + 4) * sizeof(CCountedPointer<T>));
        if (n) std::memcpy(p, _M_start, n * sizeof(CCountedPointer<T>));
        new (p + n) CCountedPointer<T>(x);
        std::free(_M_start);
        _M_start = p;
        _M_finish = p + n + 1;
        _M_end_of_storage = p + n + 4;
    }
    void push_back(const CCountedPointer<T>& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCountedPointer<T>(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
namespace NUISystem {
class CComponent {
public:
    CComponent* Parent;
    int InitialiseCalls;
    CCountedVector<CComponent> Children;
    CComponent() : Parent(0), InitialiseCalls(0) { Children._M_start = Children._M_finish = Children._M_end_of_storage = 0; }
    virtual ~CComponent() { std::free(Children._M_start); }
    virtual void Initialise() { ++InitialiseCalls; CHECK(Parent != 0); }   // the parent is set first
    virtual void SetParent(CComponent* parent) { Parent = parent; }
    virtual void AddChild(CCountedPointer<CComponent>& child)
    {
        Children.push_back(child);
        child->SetParent(this);
        child->Initialise();
    }
};
}
using NUISystem::CComponent;
static void RunTest()
{
    CComponent root, kids[6];
    CCPPointerInfo info[6];
    for (int i = 0; i < 6; ++i) {
        info[i].Count = 1;
        CCountedPointer<CComponent> p(&kids[i], &info[i]);
        root.AddChild(p);
        CHECK(kids[i].Parent == &root && kids[i].InitialiseCalls == 1 && info[i].Count == 2);
    }
    CHECK(root.Children._M_finish - root.Children._M_start == 6 && root.Children._M_start[5].Data == &kids[5]);
    CHECK(g_overflowCalls == 2);                                     // the empty vector, then at 4 elements
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("00533cc0_TEST FAIL\n");
        return 1;
    }
    std::printf("00533cc0_TEST PASS\n");
    return 0;
}