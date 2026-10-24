// Behaviour test for 0x007CA350 vector<CDefPointer<const CDefClassBase> >::push_back: in-place copy adds a reference
// to a non-null def; a full vector grows through _M_insert_overflow.
#include <cstdio>
#include <cstdlib>
#include <new>

class CDefClassBase {
public:
    long RefCount;
};

template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer(T* p) : Object(p) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++((CDefClassBase*)Object)->RefCount;
    }
};

struct __false_type { };

template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    int overflows;
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        ++overflows;
        unsigned int size = (unsigned int)(_M_finish - _M_start);
        unsigned int cap = size ? size * 2 : 1;
        T* storage = (T*)std::malloc(cap * sizeof(T));
        T* out = storage;
        for (T* p = _M_start; p != position; ++p, ++out) new (out) T(*p);
        for (unsigned int i = 0; i < fill_len; ++i, ++out) new (out) T(x);
        (void)atend;
        std::free(_M_start);
        _M_start = storage; _M_finish = out; _M_end_of_storage = storage + cap;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

int main()
{
    CDefClassBase a; a.RefCount = 1;
    CDefClassBase b; b.RefCount = 1;
    vector<CDefPointer<const CDefClassBase> > v;
    v._M_start = (CDefPointer<const CDefClassBase>*)std::malloc(2 * sizeof(CDefPointer<const CDefClassBase>));
    v._M_finish = v._M_start; v._M_end_of_storage = v._M_start + 2; v.overflows = 0;
    v.push_back(CDefPointer<const CDefClassBase>(&a));
    v.push_back(CDefPointer<const CDefClassBase>(0));
    bool ok = v._M_finish - v._M_start == 2 && a.RefCount == 2 && v.overflows == 0 && v._M_start[1].Object == 0;
    v.push_back(CDefPointer<const CDefClassBase>(&b));
    ok = ok && v.overflows == 1 && v._M_finish - v._M_start == 3 && v._M_start[2].Object == &b && b.RefCount == 2;
    std::free(v._M_start);
    if (!ok) { std::printf("007ca350_TEST FAIL\n"); return 1; }
    std::printf("007ca350_TEST PASS\n");
    return 0;
}