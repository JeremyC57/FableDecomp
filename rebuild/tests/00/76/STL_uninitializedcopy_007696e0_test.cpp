// Behaviour test for 0x007696E0 STLport __uninitialized_copy over AttackParticleAndEndPoints: deep string copies,
// copied longs, shared particle-effect pointer (reference added), end of range returned.
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

struct CThing { int m_refs; };

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    virtual ~CBaseIntelligentPointer() { SetPItem(0); }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { if (m_pItem) --m_pItem->m_refs; m_pItem = item; if (item) ++item->m_refs; }
};

template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
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

template <class _InputIter, class _ForwardIter>
_ForwardIter uninitialized_copy_model(_InputIter __first, _InputIter __last, _ForwardIter __result)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

int main()
{
    CThing fx = { 0 };
    AttackParticleAndEndPoints src[2] = { AttackParticleAndEndPoints("a", "b", 1, 2, &fx), AttackParticleAndEndPoints("c", "d", 3, 4, 0) };
    double raw[16];
    AttackParticleAndEndPoints* dst = (AttackParticleAndEndPoints*)raw;
    const AttackParticleAndEndPoints* first = src;
    AttackParticleAndEndPoints* end = uninitialized_copy_model(first, first + 2, dst);
    int ok = (end == dst + 2) && gCopies == 4 && fx.m_refs == 2;
    if (std::strcmp(dst[0].EndDummy.PStringData, "b") || std::strcmp(dst[1].StartDummy.PStringData, "c")) ok = 0;
    if (dst[0].ParticleEffect.GetPItem() != &fx || dst[1].ParticleEffect.GetPItem() != 0) ok = 0;
    if (dst[0].ParticleType != 1 || dst[1].PeriodicFrames != 4) ok = 0;
    if (uninitialized_copy_model(first, first, dst) != dst) ok = 0;
    dst[0].~AttackParticleAndEndPoints(); dst[1].~AttackParticleAndEndPoints();
    if (fx.m_refs != 1) ok = 0;
    if (!ok) { std::printf("007696e0_TEST FAIL\n"); return 1; }
    std::printf("007696e0_TEST PASS\n");
    return 0;
}