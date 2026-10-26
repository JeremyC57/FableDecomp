// Behaviour test for 0x009F6ED0 vector<CInputEventPackage>::_M_insert_overflow: allocate old_size + max(old_size, n), copy the
// head, construct n copies of x, copy the tail unless appending, destroy and free the old block, publish the new one.
#include <cstdio>
#include <cstdlib>
#include <new>

static int gCopies = 0, gDestroys = 0, gFrees = 0;

struct __false_type { __false_type() {} };

class CInputEventPackage { public:
    long m_words[12];
    CInputEventPackage(long v) { for (int i = 0; i < 12; ++i) m_words[i] = v; }
    CInputEventPackage(const CInputEventPackage& other) { ++gCopies; for (int i = 0; i < 12; ++i) m_words[i] = other.m_words[i]; }
    ~CInputEventPackage() { ++gDestroys; }
};

static CInputEventPackage* __uninitialized_copy(CInputEventPackage* first, CInputEventPackage* last, CInputEventPackage* result,
                                                const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) CInputEventPackage(*first);
    return result;
}
static CInputEventPackage* __uninitialized_fill_n(CInputEventPackage* first, unsigned int n, const CInputEventPackage& x,
                                                  const __false_type&)
{
    CInputEventPackage* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CInputEventPackage(x);
    return cur;
}
static void __destroy_aux(CInputEventPackage* first, CInputEventPackage* last, const __false_type&)
{
    for (; first != last; ++first)
        first->~CInputEventPackage();
}

template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

struct CInputEventPackageVector {
    CInputEventPackage* _M_start;
    CInputEventPackage* _M_finish;
    CInputEventPackage* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(CInputEventPackage* position, const CInputEventPackage& x, const __false_type&,
                            unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (max)(old_size, fill_len);
        CInputEventPackage* new_start = len != 0 ? (CInputEventPackage*)std::malloc(len * sizeof(CInputEventPackage)) : 0;
        CInputEventPackage* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
        if (fill_len == 1) {
            new (new_finish) CInputEventPackage(x);
            ++new_finish;
        }
        else
            new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
        if (!atend)
            new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
        __destroy_aux(_M_start, _M_finish, __false_type());
        if (_M_start) { ++gFrees; std::free(_M_start); }
        _M_start = new_start;
        _M_finish = new_finish;
        _M_end_of_storage = new_start + len;
    }
};

int main()
{
    CInputEventPackageVector v;
    v._M_start = (CInputEventPackage*)std::malloc(2 * sizeof(CInputEventPackage));
    new (v._M_start) CInputEventPackage(1);
    new (v._M_start + 1) CInputEventPackage(2);
    v._M_finish = v._M_end_of_storage = v._M_start + 2;
    CInputEventPackage x(9);
    gCopies = gDestroys = 0;
    v._M_insert_overflow(v._M_finish, x, __false_type(), 1, true);           // append: capacity 2 -> 4
    bool ok = v.size() == 3 && v._M_end_of_storage == v._M_start + 4 && v._M_start[0].m_words[0] == 1
           && v._M_start[2].m_words[11] == 9 && gCopies == 3 && gDestroys == 2 && gFrees == 1;
    v._M_insert_overflow(v._M_start + 1, x, __false_type(), 4, false);       // insert 4 in the middle: 3 + max(3, 4) = 7
    ok = ok && v.size() == 7 && v._M_end_of_storage == v._M_start + 7 && v._M_start[0].m_words[0] == 1
            && v._M_start[1].m_words[0] == 9 && v._M_start[4].m_words[0] == 9 && v._M_start[5].m_words[0] == 2
            && v._M_start[6].m_words[0] == 9 && gFrees == 2;
    if (!ok) { std::printf("009f6ed0_TEST FAIL\n"); return 1; }
    std::printf("009f6ed0_TEST PASS\n");
    return 0;
}