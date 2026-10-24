// Behaviour test for 0x00A461D0: _STL::__unguarded_partition<C3DSkeletalBoneMorph*> (Hoare partition around a pivot copy)
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
    CCharString& operator=(const CCharString& other)
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

namespace _STL {
template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template <class _ForwardIter1, class _ForwardIter2>
inline void __fastcall iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2) { swap(*__i1, *__i2); }
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for ( ; ; ) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
}

int main()
{
    {
        static const unsigned long crcs[9] = { 5, 1, 9, 3, 7, 2, 8, 4, 6 };
        C3DSkeletalBoneMorph a[9];
        FillMorphs(a, crcs, 9);
        int live = g_liveStrings;
        C3DSkeletalBoneMorph* cut = _STL::__unguarded_partition(a, a + 9, a[0], (TBoneMorphCompare)LessByCRC);
        Check(g_liveStrings == live);
        Check(cut > a && cut < a + 9);
        for (C3DSkeletalBoneMorph* p = a; p < cut; ++p)
            Check(p->BoneNameCRC <= 5 && Consistent(*p));
        for (C3DSkeletalBoneMorph* q = cut; q < a + 9; ++q)
            Check(q->BoneNameCRC >= 5 && Consistent(*q));
        unsigned long sum = 0;
        for (int i = 0; i < 9; ++i)
            sum += a[i].BoneNameCRC;
        Check(sum == 45);
    }
    Check(g_liveStrings == 0);
    if (g_fail) {
        printf("00a461d0_TEST FAIL\n");
        return 1;
    }
    printf("00a461d0_TEST PASS\n");
    return 0;
}