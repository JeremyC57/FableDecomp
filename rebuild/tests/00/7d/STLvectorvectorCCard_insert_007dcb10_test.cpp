// Behaviour test for 0x007DCB10 vector<vector<CCard*> >::insert(pos, x): end insert copy-constructs; middle insert
// moves the hands up and assigns a copy of x; a full vector goes through the overflow; returns the inserted slot.
#include <cstdio>
#include <cstdlib>
#include <new>

struct CCard { int id; };
struct Hand {                                   // vector<CCard*> stand-in: one card id is enough to track identity
    int* data;
    Hand() : data(0) { }
    explicit Hand(int v) : data(new int(v)) { }
    Hand(const Hand& o) : data(o.data ? new int(*o.data) : 0) { }
    ~Hand() { delete data; }
    Hand& operator=(const Hand& o) { if (this != &o) { delete data; data = o.data ? new int(*o.data) : 0; } return *this; }
    int value() const { return data ? *data : -1; }
};
template <class T> T* copy_backward(T* first, T* last, T* result) { while (last != first) *--result = *--last; return result; }

struct HandVector {
    Hand* _M_start; Hand* _M_finish; Hand* _M_end_of_storage;
    int overflows;
    void _M_insert_overflow(Hand* pos, const Hand& x, unsigned int n)
    {
        ++overflows;
        unsigned int size = (unsigned int)(_M_finish - _M_start);
        unsigned int cap = size + (size < n ? n : size);
        Hand* s = (Hand*)std::malloc(cap * sizeof(Hand));
        Hand* out = s;
        for (Hand* p = _M_start; p != pos; ++p, ++out) new (out) Hand(*p);
        for (unsigned int i = 0; i < n; ++i, ++out) new (out) Hand(x);
        for (Hand* p = pos; p != _M_finish; ++p, ++out) new (out) Hand(*p);
        for (Hand* p = _M_start; p != _M_finish; ++p) p->~Hand();
        std::free(_M_start);
        _M_start = s; _M_finish = out; _M_end_of_storage = s + cap;
    }
    Hand* insert(Hand* position, const Hand& x)
    {
        unsigned int n = position - _M_start;
        if (_M_finish != _M_end_of_storage) {
            if (position == _M_finish) {
                new (_M_finish) Hand(x);
                ++_M_finish;
            }
            else {
                new (_M_finish) Hand(*(_M_finish - 1));
                ++_M_finish;
                Hand x_copy = x;
                copy_backward(position, _M_finish - 2, _M_finish - 1);
                *position = x_copy;
            }
        }
        else
            _M_insert_overflow(position, x, 1);
        return _M_start + n;
    }
};

int main()
{
    HandVector v; v.overflows = 0;
    v._M_start = (Hand*)std::malloc(4 * sizeof(Hand)); v._M_finish = v._M_start; v._M_end_of_storage = v._M_start + 4;
    Hand a(1), b(2), c(3), d(4), e(5);
    Hand* r = v.insert(v._M_finish, a);                // [1]
    bool ok = r == v._M_start && r->value() == 1;
    v.insert(v._M_finish, c);                           // [1 3]
    r = v.insert(v._M_start + 1, b);                   // [1 2 3]
    ok = ok && r == v._M_start + 1 && r->value() == 2 && r->data != b.data;
    v.insert(v._M_start, d);                            // [4 1 2 3]
    r = v.insert(v._M_start + 2, e);                   // full: [4 1 5 2 3]
    ok = ok && v.overflows == 1 && r == v._M_start + 2 && v._M_finish - v._M_start == 5;
    int want[5] = { 4, 1, 5, 2, 3 };
    for (int i = 0; i < 5 && ok; ++i) ok = v._M_start[i].value() == want[i];
    for (Hand* p = v._M_start; p != v._M_finish; ++p) p->~Hand();
    std::free(v._M_start);
    if (!ok) { std::printf("007dcb10_TEST FAIL\n"); return 1; }
    std::printf("007dcb10_TEST PASS\n");
    return 0;
}