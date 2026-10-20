// Behaviour test for CTCDExplosion::SetSpecificThingsToHit (0x0080CC40): every thing of the argument is appended (in order) as
// an intelligent pointer, growing the list through _M_insert_overflow when full; each temporary is destroyed.
#include <cstdio>
#include <cstdlib>
#include <new>
struct CThing { int id; };
static int g_ctors = 0, g_dtors = 0, g_overflows = 0;
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ctors; }
    ~CBaseIntelligentPointer() { ++g_dtors; }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() { }
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct __false_type { __false_type() { } };
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fill_len, bool atend)
    {
        unsigned long n = _M_finish - _M_start, len = n ? 2 * n : 1;
        T* s = (T*)std::malloc(len * sizeof(T)); T* f = s;
        for (T* p = _M_start; p != position; ++p, ++f) new (f) T(*p);
        new (f) T(x); ++f;
        _M_start = s; _M_finish = f; _M_end_of_storage = s + len; ++g_overflows;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CTCDExplosion {
    virtual void v00() { }
    CRetailVector<CIntelligentPointer> SpecifiedThingsToHit;
    void SetSpecificThingsToHit(const CRetailVector<CThing*>& things);
};

void CTCDExplosion::SetSpecificThingsToHit(const CRetailVector<CThing*>& things)
{
    for (CThing* const* it = things.begin(); it != things.end(); ++it)
        SpecifiedThingsToHit.push_back(CIntelligentPointer(*it));
}

int main()
{
    int bad = 0;
    CThing a, b, c;
    CThing* arr[3] = { &a, &b, &c };
    CRetailVector<CThing*> things; things._M_start = arr; things._M_finish = arr + 3; things._M_end_of_storage = arr + 3;
    unsigned char raw[2 * sizeof(CIntelligentPointer)];
    CTCDExplosion e;
    e.SpecifiedThingsToHit._M_start = (CIntelligentPointer*)raw; e.SpecifiedThingsToHit._M_finish = (CIntelligentPointer*)raw;
    e.SpecifiedThingsToHit._M_end_of_storage = (CIntelligentPointer*)raw + 2;
    e.SetSpecificThingsToHit(things);
    CIntelligentPointer* v = e.SpecifiedThingsToHit._M_start;
    if (e.SpecifiedThingsToHit._M_finish - v != 3 || v[0].GetPItem() != &a || v[1].GetPItem() != &b || v[2].GetPItem() != &c) bad |= 1;
    if (g_overflows != 1 || g_dtors != 3) bad |= 2;   // three temporaries destroyed; the stored copies live on
    things._M_finish = arr;
    e.SetSpecificThingsToHit(things);                // empty input
    if (e.SpecifiedThingsToHit._M_finish - e.SpecifiedThingsToHit._M_start != 3 || g_dtors != 3) bad |= 4;
    std::free(e.SpecifiedThingsToHit._M_start);
    if (bad) { std::printf("0080cc40_TEST FAIL\n"); return 1; }
    std::printf("0080cc40_TEST PASS\n");
    return 0;
}