// Behaviour test for 0x00A6F910 std::__unguarded_insertion_sort_aux over (name, dummy object) pairs: each element
// of [first, last) is copied and linearly inserted to its place by name (a smaller element sits before first)
#include <stdio.h>
#include <string.h>
#if !defined(_MSC_VER)
#define __fastcall
#endif
static int gCopies = 0;
static int gInserts = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString() : PStringData("") { }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++gCopies; }   // stand-in
    ~CCharString() { }
    const CCharString& operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }
    bool FastCompare(const CCharString& other) const { return strcmp(PStringData, other.PStringData) < 0; }   // stand-in
};
class CMatrix3x4 { public: float M[12]; };
class C3DDummyObject {
public:
    CMatrix3x4 Transform;
    long       BoneIndex;
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class V> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, V>& a, const std::pair<CCharString, V>& b) const
    {
        return a.first.FastCompare(b.first);
    }
};
typedef std::pair<CCharString, C3DDummyObject> CDummyPair;
typedef CCompareString<C3DDummyObject> CDummyLess;
namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)   // stand-in (STLport's)
{
    ++gInserts;
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
}
static CDummyPair gArr[5];
static void Set(int i, const char* name, long bone) { gArr[i].first.PStringData = name; gArr[i].second.BoneIndex = bone; }
int main()
{
    Set(0, "a_root", 0);          // sentinel: smaller than everything after it
    Set(1, "e_hand", 4);
    Set(2, "b_head", 1);
    Set(3, "d_foot", 3);
    Set(4, "c_neck", 2);
    std::__unguarded_insertion_sort_aux(gArr + 1, gArr + 5, (CDummyPair*)0, CDummyLess());
    bool ok = true;
    ok = ok && strcmp(gArr[0].first.PStringData, "a_root") == 0 && gArr[0].second.BoneIndex == 0;
    ok = ok && strcmp(gArr[1].first.PStringData, "b_head") == 0 && gArr[1].second.BoneIndex == 1;
    ok = ok && strcmp(gArr[2].first.PStringData, "c_neck") == 0 && gArr[2].second.BoneIndex == 2;
    ok = ok && strcmp(gArr[3].first.PStringData, "d_foot") == 0 && gArr[3].second.BoneIndex == 3;
    ok = ok && strcmp(gArr[4].first.PStringData, "e_hand") == 0 && gArr[4].second.BoneIndex == 4;
    ok = ok && gInserts == 4 && gCopies >= 4;        // one insert (of a copied element) per position
    std::__unguarded_insertion_sort_aux(gArr + 2, gArr + 2, (CDummyPair*)0, CDummyLess());
    ok = ok && gInserts == 4;
    if (ok) { printf("00a6f910_TEST PASS\n"); return 0; }
    printf("00a6f910_TEST FAIL\n");
    return 1;
}