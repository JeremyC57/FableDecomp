// Behaviour test for the STLport deque<T(0x30)>::back() instance at 0x00913960.
#include <cstdio>

struct SElement30 { long id; unsigned char _data[0x2c]; };

template <class T>
struct CDequeIterator {
    T* _M_cur;
    T* _M_first;
    T* _M_last;
    T** _M_node;
    static unsigned long buffer_size() { return sizeof(T) < 128 ? 128 / sizeof(T) : 1; }
    void _M_set_node(T** newNode) { _M_node = newNode; _M_first = *newNode; _M_last = _M_first + buffer_size(); }
    CDequeIterator& operator--()
    {
        if (_M_cur == _M_first) {
            _M_set_node(_M_node - 1);
            _M_cur = _M_last;
        }
        --_M_cur;
        return *this;
    }
    T& operator*() const { return *_M_cur; }
};

class CUserProfileManager { public:
    CDequeIterator<SElement30> _M_start;
    CDequeIterator<SElement30> _M_finish;
    SElement30& GetAssignedInputsPassive();
};

SElement30& CUserProfileManager::GetAssignedInputsPassive()
{
    CDequeIterator<SElement30> tmp = _M_finish;
    --tmp;
    return *tmp;
}

int main()
{
    SElement30 bufA[2], bufB[2];
    bufA[0].id = 1; bufA[1].id = 2; bufB[0].id = 3; bufB[1].id = 4;
    SElement30* map[2] = { bufA, bufB };
    CUserProfileManager d;
    // finish in the middle of node 1: back is bufB[0]
    d._M_finish._M_node = &map[1]; d._M_finish._M_first = bufB; d._M_finish._M_last = bufB + 2; d._M_finish._M_cur = bufB + 1;
    if (d.GetAssignedInputsPassive().id != 3) { std::printf("00913960_TEST FAIL\n"); return 1; }
    // finish at the start of node 1: back is the last element of node 0
    d._M_finish._M_cur = bufB;
    if (d.GetAssignedInputsPassive().id != 2 || d._M_finish._M_cur != bufB) { std::printf("00913960_TEST FAIL\n"); return 1; }
    std::printf("00913960_TEST PASS\n");
    return 0;
}