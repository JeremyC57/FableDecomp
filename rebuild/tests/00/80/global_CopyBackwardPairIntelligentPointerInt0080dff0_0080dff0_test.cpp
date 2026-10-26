#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif

struct random_access_iterator_tag { };
class CBaseObject { public: int m_id; };
static int g_gets = 0, g_sets = 0;
struct CBaseIntelligentPointer {
    CBaseObject* PData;
    CBaseIntelligentPointer() : PData(0) { }
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* GetPItem() const { ++g_gets; return PData; }
    void SetPItem(CBaseObject* item) { ++g_sets; PData = item; }
};
template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
class CThing;
static CBaseObject g_things[8];
struct SPairIntelligentPointerInt {
    CIntelligentPointer<CThing> first;
    int                         second;
};

typedef SPairIntelligentPointerInt T_;
enum { N = 3 };

static T_* CopyBackward(const T_* first, const T_* last, T_* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

int main()
{
    bool ok = true;
    static T_ src[N];
    static T_ dst[N];
    for (int i = 0; i < N; ++i) {
        T_* s = &src[i];
        T_* d = &dst[i];
        s->first.PData = &g_things[i]; s->second = 5 * i + 1; d->second = -7;
    }
    random_access_iterator_tag tag;
    T_* r = CopyBackward(src, src + N, dst + N, tag, (int*)0);
    if (r != dst) ok = false;
    for (int i = 0; i < N; ++i) {
        const T_* s = &src[i];
        const T_* d = &dst[i];
        if (d->first.PData != &g_things[i] || d->second != 5 * i + 1) ok = false;
    }
    // overlapping shift right by one inside one array (the vector-insert use): must run back to front
    static T_ v[4];
    for (int i = 0; i < 4; ++i) { v[i].first.PData = &g_things[i]; v[i].second = i; }
    if (CopyBackward(v, v + 3, v + 4, tag, (int*)0) != v + 1) ok = false;
    for (int i = 1; i < 4; ++i) if (v[i].first.PData != &g_things[i - 1] || v[i].second != i - 1) ok = false;
    if (CopyBackward(src, src, dst + 2, tag, (int*)0) != dst + 2) ok = false;
    if (!ok) { std::printf("0080dff0_TEST FAIL\n"); return 1; }
    std::printf("0080dff0_TEST PASS\n");
    return 0;
}