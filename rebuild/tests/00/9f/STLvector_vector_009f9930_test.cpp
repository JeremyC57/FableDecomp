// Behaviour test for 0x009F9930 vector<CProfileTreeEntry*> copy constructor: allocate exactly x.size() pointers and memmove
// them across; an empty source leaves all three pointers null.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct CProfileTreeEntry;

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    vector(const vector& x) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        unsigned int n = x.size();
        _M_start = n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        const T* first = x._M_start;
        const T* last = x._M_finish;
        _M_finish = (last == first) ? _M_start
            : (T*)((char*)std::memmove(_M_start, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first));
    }
};

int main()
{
    CProfileTreeEntry* items[3] = { (CProfileTreeEntry*)0x10, (CProfileTreeEntry*)0x20, (CProfileTreeEntry*)0x30 };
    vector<CProfileTreeEntry*> a;
    a._M_start = items; a._M_finish = items + 3; a._M_end_of_storage = items + 3;
    vector<CProfileTreeEntry*> b(a);
    bool ok = b.size() == 3 && b._M_end_of_storage == b._M_finish && b._M_start != items
           && b._M_start[0] == items[0] && b._M_start[2] == items[2];
    std::free(b._M_start);
    vector<CProfileTreeEntry*> e;
    vector<CProfileTreeEntry*> f(e);
    ok = ok && f._M_start == 0 && f._M_finish == 0 && f._M_end_of_storage == 0;
    if (!ok) { std::printf("009f9930_TEST FAIL\n"); return 1; }
    std::printf("009f9930_TEST PASS\n");
    return 0;
}