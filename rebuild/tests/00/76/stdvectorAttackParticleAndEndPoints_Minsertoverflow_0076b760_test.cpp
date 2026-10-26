// Behaviour test for 0x0076B760 vector<AttackParticleAndEndPoints>::_M_insert_overflow: the block grows to
// old_size + max(old_size, fill_len); prefix, fill_len copies of x and (unless atend) the suffix are rebuilt in order,
// old elements destroyed (intelligent-pointer references balanced) and the old block freed.
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
#include <cstdlib>

struct Vec {
    AttackParticleAndEndPoints* _M_start;
    AttackParticleAndEndPoints* _M_finish;
    AttackParticleAndEndPoints* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(AttackParticleAndEndPoints* pos, const AttackParticleAndEndPoints& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        AttackParticleAndEndPoints* new_start = len ? (AttackParticleAndEndPoints*)std::malloc(len * sizeof(AttackParticleAndEndPoints)) : 0;
        AttackParticleAndEndPoints* new_finish = new_start;
        for (AttackParticleAndEndPoints* p = _M_start; p != pos; ++p, ++new_finish) _Construct(new_finish, *p);
        if (fill_len == 1) { _Construct(new_finish, x); ++new_finish; }
        else for (unsigned int n = fill_len; n > 0; --n, ++new_finish) _Construct(new_finish, x);
        if (!atend)
            for (AttackParticleAndEndPoints* p = pos; p != _M_finish; ++p, ++new_finish) _Construct(new_finish, *p);
        for (AttackParticleAndEndPoints* p = _M_start; p != _M_finish; ++p) p->~AttackParticleAndEndPoints();
        if (_M_start) std::free(_M_start);
        _M_start = new_start; _M_finish = new_finish; _M_end_of_storage = new_start + len;
    }
};

int main()
{
    CThing fx = { 0 };
    Vec v = { 0, 0, 0 };
    AttackParticleAndEndPoints a("a0", "a1", 1, 10, &fx);
    AttackParticleAndEndPoints b("b0", "b1", 2, 20, 0);
    v._M_insert_overflow(v._M_finish, a, 1, true);                  // empty -> 1
    int ok = v.size() == 1u && v._M_end_of_storage - v._M_start == 1 && fx.m_refs == 2;
    v._M_insert_overflow(v._M_finish, b, 1, true);                  // 1 -> 2
    if (v.size() != 2u || v._M_end_of_storage - v._M_start != 2 || v._M_start[1].ParticleType != 2) ok = 0;
    v._M_insert_overflow(v._M_start + 1, a, 3, false);              // insert 3 copies in the middle: 2 + max(2,3) = 5
    if (v.size() != 5u || v._M_end_of_storage - v._M_start != 5) ok = 0;
    long want[5] = { 1, 1, 1, 1, 2 };
    for (int i = 0; i < 5; ++i) if (v._M_start[i].ParticleType != want[i]) ok = 0;
    if (std::strcmp(v._M_start[4].StartDummy.PStringData, "b0") || fx.m_refs != 5) ok = 0;
    for (AttackParticleAndEndPoints* p = v._M_start; p != v._M_finish; ++p) p->~AttackParticleAndEndPoints();
    std::free(v._M_start);
    if (fx.m_refs != 1) ok = 0;
    if (!ok) { std::printf("0076b760_TEST FAIL\n"); return 1; }
    std::printf("0076b760_TEST PASS\n");
    return 0;
}