// Behaviour test for 0x009F65E0 vector<CInputEvent>::_M_allocate_and_copy: malloc n elements (none for n == 0) and
// copy-construct [first, last) into the start of the block, returning the block.
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

struct __false_type { __false_type() {} };

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(std::malloc(n * sizeof(T))) : 0; }
};

template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    struct Proxy : allocator<T> { T* _M_data; } _M_end_of_storage;
    T* _M_allocate_and_copy(unsigned int n, const T* first, const T* last)
    {
        T* result = _M_end_of_storage.allocate(n);
        T* cur = result;
        for (; first != last; ++first, ++cur)
            new (cur) T(*first);
        return result;
    }
};

int main()
{
    CInputEvent src[2];
    for (int i = 0; i < 2; ++i) for (int w = 0; w < 13; ++w) src[i].m_words[w] = i * 10 + w;
    vector<CInputEvent> v;
    CInputEvent* p = v._M_allocate_and_copy(5, src, src + 2);
    bool ok = p != 0 && gCopies == 2 && p[0].m_words[3] == 3 && p[1].m_words[12] == 22;
    std::free(p);
    ok = ok && v._M_allocate_and_copy(0, src, src) == 0 && gCopies == 2;
    if (!ok) { std::printf("009f65e0_TEST FAIL\n"); return 1; }
    std::printf("009f65e0_TEST PASS\n");
    return 0;
}