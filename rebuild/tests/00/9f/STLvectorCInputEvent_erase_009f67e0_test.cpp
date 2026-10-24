// Behaviour test for 0x009F67E0 vector<CInputEvent>::erase(first, last): the tail [last, end) is assigned down onto first, the
// vacated elements are destroyed, finish moves back by (last - first) and first is returned.
#include <cstdio>

static int gDestroyed = 0;

class CInputEvent {
public:
    long m_words[13];
    ~CInputEvent() { ++gDestroyed; }
};

template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* erase(T* first, T* last)
    {
        T* result = first;
        for (int n = (int)(_M_finish - last); n > 0; --n) {
            *result = *last;
            ++last;
            ++result;
        }
        for (T* p = result; p != _M_finish; ++p)
            p->~T();
        _M_finish = result;
        return first;
    }
};

int main()
{
    CInputEvent arr[5];
    for (int i = 0; i < 5; ++i) for (int w = 0; w < 13; ++w) arr[i].m_words[w] = i;
    vector<CInputEvent> v;
    v._M_start = arr; v._M_finish = arr + 5; v._M_end_of_storage = arr + 5;
    gDestroyed = 0;
    CInputEvent* r = v.erase(arr + 1, arr + 3);
    bool ok = r == arr + 1 && v._M_finish == arr + 3 && gDestroyed == 2
           && arr[0].m_words[0] == 0 && arr[1].m_words[0] == 3 && arr[2].m_words[12] == 4;
    r = v.erase(arr + 2, arr + 3);
    ok = ok && r == arr + 2 && v._M_finish == arr + 2 && gDestroyed == 3;
    gDestroyed = 0;
    if (!ok) { std::printf("009f67e0_TEST FAIL\n"); return 1; }
    std::printf("009f67e0_TEST PASS\n");
    return 0;
}