// Behaviour test for 0x006FC6A0 std::vector<pair<unsigned long, CSmallVector<CEvent,32> > >::_M_insert_overflow
#include <cstdio>
#include <cstdlib>
#include <new>

namespace model {

struct CEvent { long m_a, m_b; };

struct CSmallVector {
    CEvent* Start;
    int Size;
    CSmallVector() : Start(0), Size(0) {}
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    void Copy(const CSmallVector& other)
    {
        delete[] Start;
        Start = 0;
        Size = 0;
        Start = new CEvent[other.Size];
        Size = other.Size;
        for (int i = 0; i < Size; ++i)
            Start[i] = other.Start[i];
    }
    void Clear() { delete[] Start; Start = 0; Size = 0; }
    ~CSmallVector() { Clear(); }
};

struct SKeyedEvents {
    unsigned long first;
    CSmallVector second;
    SKeyedEvents(const SKeyedEvents& o) : first(o.first), second(o.second) {}
    SKeyedEvents(unsigned long k, int n) : first(k)
    {
        second.Start = new CEvent[n];
        second.Size = n;
        for (int i = 0; i < n; ++i) { second.Start[i].m_a = (long)k * 10 + i; second.Start[i].m_b = i; }
    }
};

SKeyedEvents* uninit_copy(const SKeyedEvents* first, const SKeyedEvents* last, SKeyedEvents* result)
{
    for (; first != last; ++first, ++result)
        new (result) SKeyedEvents(*first);
    return result;
}
SKeyedEvents* uninit_fill_n(SKeyedEvents* first, unsigned int n, const SKeyedEvents& x)
{
    for (; n > 0; --n, ++first)
        new (first) SKeyedEvents(x);
    return first;
}

struct vector {
    SKeyedEvents* _M_start;
    SKeyedEvents* _M_finish;
    SKeyedEvents* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(SKeyedEvents* position, const SKeyedEvents& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        SKeyedEvents* new_start = len != 0 ? (SKeyedEvents*)std::malloc(len * sizeof(SKeyedEvents)) : 0;
        SKeyedEvents* new_finish = uninit_copy(_M_start, position, new_start);
        if (fill_len == 1) {
            new (new_finish) SKeyedEvents(x);
            ++new_finish;
        }
        else
            new_finish = uninit_fill_n(new_finish, fill_len, x);
        if (!atend)
            new_finish = uninit_copy(position, _M_finish, new_finish);
        for (SKeyedEvents* p = _M_start; p != _M_finish; ++p)
            p->~SKeyedEvents();
        if (_M_start)
            std::free(_M_start);
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }
};

}

int main()
{
    using namespace model;
    vector v;
    v._M_start = v._M_finish = v._M_end_of_storage = 0;
    SKeyedEvents a(1, 2), b(2, 3), c(3, 1);
    v._M_insert_overflow(v._M_finish, a, 1, true);                 // empty -> capacity 1
    int bad = 0;
    if (v.size() != 1 || v._M_end_of_storage - v._M_start != 1 || v._M_start[0].first != 1) bad = 1;
    if (v._M_start[0].second.Size != 2 || v._M_start[0].second.Start == a.second.Start) bad = 2;
    v._M_insert_overflow(v._M_finish, b, 1, true);                 // capacity 2
    v._M_insert_overflow(v._M_start + 1, c, 3, false);             // insert 3 copies in the middle
    if (v.size() != 5 || v._M_end_of_storage - v._M_start != 5) bad = 3;
    unsigned long want[5] = { 1, 3, 3, 3, 2 };
    for (int i = 0; i < 5; ++i)
        if (v._M_start[i].first != want[i]) bad = 4;
    if (v._M_start[4].second.Size != 3 || v._M_start[4].second.Start[2].m_a != 22) bad = 5;
    if (v._M_start[2].second.Size != 1 || v._M_start[2].second.Start[0].m_a != 30) bad = 6;
    for (SKeyedEvents* p = v._M_start; p != v._M_finish; ++p)
        p->~SKeyedEvents();
    std::free(v._M_start);
    if (bad) { std::printf("006fc6a0_TEST FAIL\n"); return 1; }
    std::printf("006fc6a0_TEST PASS\n");
    return 0;
}