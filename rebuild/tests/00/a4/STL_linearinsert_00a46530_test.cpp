// Behaviour test for 0x00A46530: std::__linear_insert<C3DSkeletalBoneMorph*> (insertion step: shift the run or do the unguarded insert)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

static int g_liveStrings = 0;
class CCharString {                                   // test model: a deep-copying string with a live count
public:
    char* m_pData;
    CCharString() : m_pData(0) { ++g_liveStrings; }
    CCharString(const char* s) : m_pData(Dup(s)) { ++g_liveStrings; }
    CCharString(const CCharString& other) : m_pData(Dup(other.m_pData)) { ++g_liveStrings; }
    ~CCharString() { free(m_pData); --g_liveStrings; }
    const CCharString& operator=(const CCharString& other)
    {
        if (this != &other) {
            char* p = Dup(other.m_pData);
            free(m_pData);
            m_pData = p;
        }
        return *this;
    }
    const char* c_str() const { return m_pData ? m_pData : ""; }
    bool operator<(const CCharString& o) const { return strcmp(c_str(), o.c_str()) < 0; }
    static char* Dup(const char* s)
    {
        if (s == 0) return 0;
        char* p = (char*)malloc(strlen(s) + 1);
        strcpy(p, s);
        return p;
    }
};
static bool Eq(const CCharString& s, const char* t) { return strcmp(s.c_str(), t) == 0; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class C3DSkeletalBoneMorph {
public:
    CCharString   BoneName;
    unsigned long BoneNameCRC;
    C3DVector     Scaling;
    C3DSkeletalBoneMorph() : BoneNameCRC(0) { }
};
typedef bool (__fastcall* TBoneMorphCompare)(const C3DSkeletalBoneMorph& a, const C3DSkeletalBoneMorph& b);
static bool __fastcall LessByCRC(const C3DSkeletalBoneMorph& a, const C3DSkeletalBoneMorph& b) { return a.BoneNameCRC < b.BoneNameCRC; }
static void SetMorph(C3DSkeletalBoneMorph& m, unsigned long crc)
{
    char name[3];
    name[0] = (char)('a' + crc / 26);
    name[1] = (char)('a' + crc % 26);
    name[2] = 0;
    m.BoneName = CCharString(name);
    m.BoneNameCRC = crc;
    m.Scaling = C3DVector((float)crc, 0.5f, 2.0f);
}
static bool Consistent(const C3DSkeletalBoneMorph& m)
{
    const char* n = m.BoneName.c_str();
    return n[0] == (char)('a' + m.BoneNameCRC / 26) && n[1] == (char)('a' + m.BoneNameCRC % 26) &&
        Near(m.Scaling.X, (float)m.BoneNameCRC) && Near(m.Scaling.Y, 0.5f) && Near(m.Scaling.Z, 2.0f);
}
static void FillMorphs(C3DSkeletalBoneMorph* a, const unsigned long* crcs, int n)
{
    for (int i = 0; i < n; ++i)
        SetMorph(a[i], crcs[i]);
}

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result,
                                               const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BI1, class _BI2>
inline _BI2 __fastcall copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
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
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}
}

int main()
{
    {
        static const unsigned long crcs[5] = { 3, 4, 6, 8, 1 };
        static const unsigned long want[5] = { 1, 3, 4, 6, 8 };
        C3DSkeletalBoneMorph a[5];
        FillMorphs(a, crcs, 5);
        int live = g_liveStrings;
        std::__linear_insert(a, a + 4, a[4], (TBoneMorphCompare)LessByCRC);
        Check(g_liveStrings == live);
        for (int i = 0; i < 5; ++i)
            Check(a[i].BoneNameCRC == want[i] && Consistent(a[i]));
        static const unsigned long crcs2[5] = { 3, 4, 6, 8, 5 };
        static const unsigned long want2[5] = { 3, 4, 5, 6, 8 };
        FillMorphs(a, crcs2, 5);
        std::__linear_insert(a, a + 4, a[4], (TBoneMorphCompare)LessByCRC);
        for (int i = 0; i < 5; ++i)
            Check(a[i].BoneNameCRC == want2[i] && Consistent(a[i]));
    }
    Check(g_liveStrings == 0);
    if (g_fail) {
        printf("00a46530_TEST FAIL\n");
        return 1;
    }
    printf("00a46530_TEST PASS\n");
    return 0;
}