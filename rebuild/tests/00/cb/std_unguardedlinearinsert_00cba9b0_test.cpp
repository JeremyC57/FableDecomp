// Behaviour test for 0x00CBA9B0 (std::__unguarded_linear_insert<CBindingPair*, CBindingPair, CBindingLess>): the STLport
// insertion step over (CCharString, CCountedPointer) pairs ordered by name. Checks: the value moves left past every
// greater name and stops at the first not-greater one (unguarded: a smaller sentinel exists), and the counted pointers'
// reference counts come out balanced (each pair keeps exactly its own binding).
#include <cstdio>
#include <cstring>
class CCPPointerInfo { public: int RefCount; };
class CCharString {
public:
    const char* PStringData;
    bool FastCompare(const CCharString& other) const { return std::strcmp(PStringData, other.PStringData) < 0; }
};
template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { if (Info) --Info->RefCount; Data = 0; Info = 0; }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) --Info->RefCount;
            Data = data;
            Info = info;
            if (info) ++info->RefCount;
        }
        return *this;
    }
};
class CEntityScriptBindingBase { public: int id; };
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}
template <class V> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, V>& a, const std::pair<CCharString, V>& b) const { return a.first.FastCompare(b.first); }
};
typedef std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > CBindingPair;
typedef CCompareString<CCountedPointer<CEntityScriptBindingBase> > CBindingLess;
namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
}
static int fail(int line) { std::printf("00cba9b0_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)
int main()
{
    const char* names[5] = { "Alpha", "Delta", "Echo", "Kilo", "Charlie" };
    CEntityScriptBindingBase b[5];
    CCPPointerInfo info[5];
    CBindingPair v[5];
    for (int i = 0; i < 5; ++i) {
        b[i].id = i; info[i].RefCount = 1;
        v[i].first.PStringData = names[i]; v[i].second.Data = &b[i]; v[i].second.Info = &info[i];
    }
    std::__unguarded_linear_insert(v + 4, v[4], CBindingLess());
    const char* want[5] = { "Alpha", "Charlie", "Delta", "Echo", "Kilo" };
    const int wantId[5] = { 0, 4, 1, 2, 3 };
    for (int i = 0; i < 5; ++i) {
        CHECK(std::strcmp(v[i].first.PStringData, want[i]) == 0);
        CHECK(v[i].second.Data->id == wantId[i] && v[i].second.Info == &info[wantId[i]]);
        CHECK(info[i].RefCount == 1);
    }
    std::__unguarded_linear_insert(v + 4, v[4], CBindingLess());   // already in place: nothing moves
    CHECK(std::strcmp(v[4].first.PStringData, "Kilo") == 0 && v[4].second.Data->id == 3 && info[3].RefCount == 1);
    std::printf("00cba9b0_TEST PASS\n");
    return 0;
}