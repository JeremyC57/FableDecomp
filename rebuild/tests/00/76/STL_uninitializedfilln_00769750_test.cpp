// Behaviour test for 0x00769750 STLport __uninitialized_fill_n over AttackParticleAndEndPoints: n copies of x
// constructed in raw storage (n == 0 constructs nothing), end of range returned.
#include <cstdio>
#include <cstring>
#include <new>

static int gCopies = 0;

class CCharString {
public:
    char* PStringData;
    CCharString(const char* s) { PStringData = new char[std::strlen(s) + 1]; std::strcpy(PStringData, s); }
    CCharString(const CCharString& o) { ++gCopies; PStringData = new char[std::strlen(o.PStringData) + 1]; std::strcpy(PStringData, o.PStringData); }
    ~CCharString() { delete[] PStringData; }
};

class CThing { public: int m_refs; };

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    virtual ~CBaseIntelligentPointer() { SetPItem(0); }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { if (m_pItem) --m_pItem->m_refs; m_pItem = item; if (item) ++item->m_refs; }
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* p) { SetPItem(p); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct AttackParticleAndEndPoints {
    CCharString StartDummy;
    CCharString EndDummy;
    long        ParticleType;
    long        PeriodicFrames;
    CIntelligentPointer<CThing> ParticleEffect;
    AttackParticleAndEndPoints(const char* a, const char* b, long t, long p, CThing* e)
        : StartDummy(a), EndDummy(b), ParticleType(t), PeriodicFrames(p), ParticleEffect(e) { }
    AttackParticleAndEndPoints(const AttackParticleAndEndPoints& other)
        : StartDummy(other.StartDummy), EndDummy(other.EndDummy), ParticleType(other.ParticleType),
          PeriodicFrames(other.PeriodicFrames), ParticleEffect(other.ParticleEffect) { }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter uninitialized_fill_n_model(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

int main()
{
    CThing fx = { 0 };
    AttackParticleAndEndPoints x("start", "end", 5, 6, &fx);
    double raw[32];
    AttackParticleAndEndPoints* dst = (AttackParticleAndEndPoints*)raw;
    AttackParticleAndEndPoints* end = uninitialized_fill_n_model(dst, 3u, x);
    int ok = (end == dst + 3) && gCopies == 6 && fx.m_refs == 4;
    for (int i = 0; i < 3; ++i)
        if (std::strcmp(dst[i].StartDummy.PStringData, "start") || dst[i].ParticleType != 5 || dst[i].ParticleEffect.GetPItem() != &fx) ok = 0;
    if (uninitialized_fill_n_model(dst, 0u, x) != dst || gCopies != 6) ok = 0;
    for (int i = 0; i < 3; ++i) dst[i].~AttackParticleAndEndPoints();
    if (!ok) { std::printf("00769750_TEST FAIL\n"); return 1; }
    std::printf("00769750_TEST PASS\n");
    return 0;
}