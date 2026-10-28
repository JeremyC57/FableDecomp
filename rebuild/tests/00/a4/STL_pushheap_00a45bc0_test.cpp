// Behaviour test for 0x00A45BC0: std::__push_heap<C3DSkeletalBoneMorph*, int> (sift a value up from the hole)
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
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
}

static bool IsHeap(const C3DSkeletalBoneMorph* a, int n)
{
    for (int i = 1; i < n; ++i)
        if (a[(i - 1) / 2].BoneNameCRC < a[i].BoneNameCRC)
            return false;
    return true;
}

int main()
{
    {
        static const unsigned long crcs[7] = { 9, 5, 8, 1, 2, 3, 0 };
        C3DSkeletalBoneMorph a[7];
        FillMorphs(a, crcs, 7);
        C3DSkeletalBoneMorph v;
        SetMorph(v, 7);
        std::__push_heap(a, 6, 0, v, (TBoneMorphCompare)LessByCRC);
        Check(a[6].BoneNameCRC == 7 && Consistent(a[6]) && IsHeap(a, 7));
        FillMorphs(a, crcs, 7);
        SetMorph(v, 10);
        std::__push_heap(a, 6, 0, v, (TBoneMorphCompare)LessByCRC);
        static const unsigned long want[7] = { 10, 5, 9, 1, 2, 3, 8 };
        for (int i = 0; i < 7; ++i)
            Check(a[i].BoneNameCRC == want[i] && Consistent(a[i]));
        Check(IsHeap(a, 7));
        // topIndex stops the climb
        FillMorphs(a, crcs, 7);
        std::__push_heap(a, 6, 2, v, (TBoneMorphCompare)LessByCRC);
        Check(a[2].BoneNameCRC == 10 && a[6].BoneNameCRC == 8 && a[0].BoneNameCRC == 9);
    }
    Check(g_liveStrings == 0);
    if (g_fail) {
        printf("00a45bc0_TEST FAIL\n");
        return 1;
    }
    printf("00a45bc0_TEST PASS\n");
    return 0;
}