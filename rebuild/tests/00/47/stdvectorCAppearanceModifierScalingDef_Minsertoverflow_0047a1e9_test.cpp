// Behaviour test for 0x0047a1e9 std::vector<CAppearanceModifierScalingDef>::_M_insert_overflow (model copy over a 0x48-byte element).
#include <cstdio>
#include <cstdlib>
#include <new>

struct SElement {
    long m_id;
    unsigned char _rest[0x44];
    SElement(long id) : m_id(id) { }
    SElement(const SElement& o) : m_id(o.m_id) { }
};

struct VectorModel {
    SElement* _M_start;
    SElement* _M_finish;
    SElement* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear() { std::free(_M_start); }
    void _M_insert_overflow(SElement* position, const SElement& x, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        SElement* new_start = len != 0 ? (SElement*)std::malloc(len * sizeof(SElement)) : 0;
        SElement* new_finish = new_start;
        for (SElement* p = _M_start; p != position; ++p, ++new_finish) new (new_finish) SElement(*p);
        if (fill_len == 1) {
            new (new_finish) SElement(x);
            ++new_finish;
        }
        else
            for (unsigned int n = fill_len; n > 0; --n, ++new_finish) new (new_finish) SElement(x);
        if (!atend)
            for (SElement* q = position; q != _M_finish; ++q, ++new_finish) new (new_finish) SElement(*q);
        _M_clear();
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }
};

int main()
{
    VectorModel v;
    v._M_start = (SElement*)std::malloc(2 * sizeof(SElement));
    new (v._M_start) SElement(10);
    new (v._M_start + 1) SElement(20);
    v._M_finish = v._M_start + 2;
    v._M_end_of_storage = v._M_finish;
    SElement x(99);
    v._M_insert_overflow(v._M_start + 1, x, 1, false);          // {10, 99, 20}, capacity 4
    int ok = v.size() == 3 && v._M_start[0].m_id == 10 && v._M_start[1].m_id == 99 && v._M_start[2].m_id == 20
          && v._M_end_of_storage == v._M_start + 4;
    v._M_insert_overflow(v._M_finish, x, 5, true);              // append 5 copies at the end, capacity 3 + 5
    if (v.size() != 8 || v._M_start[7].m_id != 99 || v._M_start[2].m_id != 20 || v._M_end_of_storage != v._M_start + 8) ok = 0;
    std::free(v._M_start);
    if (!ok) { std::printf("0047a1e9_TEST FAIL\n"); return 1; }
    std::printf("0047a1e9_TEST PASS\n");
    return 0;
}