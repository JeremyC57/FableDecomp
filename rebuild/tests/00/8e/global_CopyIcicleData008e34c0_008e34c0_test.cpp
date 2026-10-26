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
class C3DVector { public: float X, Y, Z; };
struct CIcicleData {
    CIntelligentPointer<CThing> PEffect;
    CIntelligentPointer<CThing> PTrap;
    long                        EffectTimer;
    long                        TrapTimer;
    C3DVector                   Position;
};

typedef CIcicleData T_;
enum { N = 3 };

static T_* Copy(const T_* first, const T_* last, T_* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
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
        s->PEffect.PData = &g_things[i]; s->PTrap.PData = &g_things[i + 3]; s->EffectTimer = 11 * i; s->TrapTimer = 13 * i;
        s->Position.X = 1.0f * i; s->Position.Y = 2.0f; s->Position.Z = -0.5f * i;
    }
    random_access_iterator_tag tag;
    T_* r = Copy(src, src + N, dst, tag, (int*)0);
    if (r != dst + N) ok = false;
    for (int i = 0; i < N; ++i) {
        const T_* s = &src[i];
        const T_* d = &dst[i];
        if (d->PEffect.PData != &g_things[i] || d->PTrap.PData != &g_things[i + 3] || d->EffectTimer != 11 * i || d->TrapTimer != 13 * i) ok = false;
        if (d->Position.X != 1.0f * i || d->Position.Y != 2.0f || d->Position.Z != -0.5f * i) ok = false;
    }
    if (g_gets != 2 * N || g_sets != 2 * N) ok = false;
    if (Copy(src, src, dst + 1, tag, (int*)0) != dst + 1) ok = false;
    if (!ok) { std::printf("008e34c0_TEST FAIL\n"); return 1; }
    std::printf("008e34c0_TEST PASS\n");
    return 0;
}