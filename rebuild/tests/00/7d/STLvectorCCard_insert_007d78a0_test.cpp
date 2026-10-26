// Behaviour test for 0x007D78A0 vector<CCard*>::insert(pos, x): end insert appends; middle insert shifts the tail up;
// a full vector goes through the overflow; the returned iterator points at the inserted card.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

class CCard { public: int id; };
struct __true_type { };

template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    int overflows;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void _M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int n, bool atend = false)
    {
        (void)atend;
        ++overflows;
        unsigned int size = (unsigned int)(_M_finish - _M_start);
        unsigned int cap = size + (size < n ? n : size);
        T* s = (T*)std::malloc(cap * sizeof(T));
        T* out = s;
        for (T* p = _M_start; p != pos; ++p) *out++ = *p;
        for (unsigned int i = 0; i < n; ++i) *out++ = x;
        for (T* p = pos; p != _M_finish; ++p) *out++ = *p;
        std::free(_M_start);
        _M_start = s; _M_finish = out; _M_end_of_storage = s + cap;
    }
    T* insert(T* position, const T& x)
    {
        unsigned int n = position - begin();
        if (_M_finish != _M_end_of_storage) {
            if (position == end()) {
                new (_M_finish) T(x);
                ++_M_finish;
            }
            else {
                new (_M_finish) T(*(_M_finish - 1));
                ++_M_finish;
                T x_copy = x;
                int num = (int)((char*)(_M_finish - 2) - (char*)position);
                if (num > 0) std::memmove((char*)(_M_finish - 1) - num, position, num);
                *position = x_copy;
            }
        }
        else
            _M_insert_overflow(position, x, __true_type(), 1);
        return begin() + n;
    }
};

int main()
{
    CCard c[6];
    for (int i = 0; i < 6; ++i) c[i].id = i;
    vector<CCard*> v; v.overflows = 0;
    v._M_start = (CCard**)std::malloc(4 * sizeof(CCard*)); v._M_finish = v._M_start; v._M_end_of_storage = v._M_start + 4;
    CCard* p0 = &c[0]; CCard* p1 = &c[1]; CCard* p2 = &c[2]; CCard* p3 = &c[3]; CCard* p4 = &c[4];
    CCard** r = v.insert(v.end(), p0);                 // [0]
    bool ok = r == v._M_start && *r == &c[0];
    v.insert(v.end(), p2);                              // [0 2]
    r = v.insert(v._M_start + 1, p1);                  // [0 1 2]
    ok = ok && r == v._M_start + 1 && *r == &c[1];
    r = v.insert(v._M_start, p3);                      // [3 0 1 2]
    ok = ok && r == v._M_start && v.overflows == 0;
    r = v.insert(v._M_start + 2, p4);                  // full: [3 0 4 1 2]
    ok = ok && v.overflows == 1 && r == v._M_start + 2 && v._M_finish - v._M_start == 5;
    int want[5] = { 3, 0, 4, 1, 2 };
    for (int i = 0; i < 5 && ok; ++i) ok = v._M_start[i]->id == want[i];
    std::free(v._M_start);
    if (!ok) { std::printf("007d78a0_TEST FAIL\n"); return 1; }
    std::printf("007d78a0_TEST PASS\n");
    return 0;
}