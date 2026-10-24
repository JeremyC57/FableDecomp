// Behaviour test for 0x009F66A0 vector<CInputEvent>::vector(const vector&): exactly x.size() elements are allocated
// (start/finish/end_of_storage), every element copy-constructed; an empty source gives an all-null vector.
#include <cstdio>
#include <cstdlib>
#include <new>

static int gCopies = 0;

class CInputEvent {
public:
    long m_words[13];
    CInputEvent() {}
    CInputEvent(const CInputEvent& other) { ++gCopies; for (int i = 0; i < 13; ++i) m_words[i] = other.m_words[i]; }
};

template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    vector(const vector& x) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        unsigned int n = x.size();
        _M_start = n != 0 ? static_cast<T*>(std::malloc(n * sizeof(T))) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        T* cur = _M_start;
        for (const T* first = x._M_start; first != x._M_finish; ++first, ++cur)
            new (cur) T(*first);
        _M_finish = cur;
    }
};

int main()
{
    CInputEvent src[3];
    for (int i = 0; i < 3; ++i) for (int w = 0; w < 13; ++w) src[i].m_words[w] = i * 7 + w;
    vector<CInputEvent> a;
    a._M_start = src; a._M_finish = src + 3; a._M_end_of_storage = src + 3;
    vector<CInputEvent> b(a);
    bool ok = b.size() == 3 && b._M_end_of_storage == b._M_finish && gCopies == 3 && b._M_start[2].m_words[12] == 26
           && b._M_start != src;
    std::free(b._M_start);
    vector<CInputEvent> e;
    vector<CInputEvent> f(e);
    ok = ok && f._M_start == 0 && f._M_finish == 0 && f._M_end_of_storage == 0;
    if (!ok) { std::printf("009f66a0_TEST FAIL\n"); return 1; }
    std::printf("009f66a0_TEST PASS\n");
    return 0;
}