// Behaviour test for 0x0077E580 vector<CIntelligentPointer<CThing> >::reserve
#include <cstdio>
#include <cstdlib>
#include <new>

class CThing { public: int id; };
struct CPtr {
    CThing* PItem;
    unsigned int pad;
    CPtr(CThing* t) : PItem(t), pad(0) { }
    CPtr(const CPtr& o) : PItem(o.PItem), pad(0) { }
};
struct __false_type { __false_type() { } };
static CPtr* UninitializedCopy(CPtr* first, CPtr* last, CPtr* result, const __false_type&)
{
    CPtr* cur = result;
    for (; first != last; ++first, ++cur) new (cur) CPtr(*first);
    return cur;
}
struct CPtrVector {
    CPtr* _M_start;
    CPtr* _M_finish;
    CPtr* _M_end_of_storage;
    int clears;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    CPtr* allocate(unsigned int n) { return n != 0 ? (CPtr*)std::malloc(n * sizeof(CPtr)) : 0; }
    void _M_clear() { ++clears; if (_M_start) std::free(_M_start); }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            const unsigned int oldSize = size();
            CPtr* tmp;
            if (_M_start) {
                tmp = allocate(n);
                UninitializedCopy(_M_start, _M_finish, tmp, __false_type());
                _M_clear();
            } else
                tmp = allocate(n);
            _M_start = tmp; _M_finish = tmp + oldSize; _M_end_of_storage = tmp + n;
        }
    }
};

int main()
{
    CThing a = { 1 }, b = { 2 };
    CPtrVector v = { 0, 0, 0, 0 };
    v.reserve(2);
    if (v.capacity() != 2 || v.size() != 0 || v.clears != 0) { std::printf("0077e580_TEST FAIL\n"); return 1; }
    new (v._M_finish) CPtr(&a); ++v._M_finish;
    new (v._M_finish) CPtr(&b); ++v._M_finish;
    v.reserve(1);                         // no shrink
    if (v.capacity() != 2 || v.clears != 0) { std::printf("0077e580_TEST FAIL\n"); return 1; }
    v.reserve(5);
    if (v.capacity() != 5 || v.size() != 2 || v.clears != 1 || v._M_start[0].PItem != &a || v._M_start[1].PItem != &b) { std::printf("0077e580_TEST FAIL\n"); return 1; }
    std::printf("0077e580_TEST PASS\n");
    return 0;
}