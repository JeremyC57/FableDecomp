// Behaviour test for 00c1ebf0: CPtrArray<CDisplayElement*>::Remove -- find the last slot holding the value, shift the later entries down one, drop the count
#include <cstdio>

struct CDisplayElement { int m_id; };

template <class T>
struct CPtrArray {
    T* m_data;
    unsigned int m_capacity;
    unsigned int m_count;
    long Find(T value)
    {
        long index = -1;
        for (long i = (long)m_count - 1; i >= 0 && index < 0; --i)
            if (m_data[i] == value)
                index = i;
        return index;
    }
    void Remove(T value)
    {
        for (unsigned int i = (unsigned int)Find(value); i < m_count - 1; ++i)
            m_data[i] = m_data[i + 1];
        --m_count;
    }
};

int main()
{
    CDisplayElement e[4] = { { 0 }, { 1 }, { 2 }, { 3 } };
    CDisplayElement* store[4] = { &e[0], &e[1], &e[2], &e[3] };
    CPtrArray<CDisplayElement*> arr; arr.m_data = store; arr.m_capacity = 4; arr.m_count = 4;
    arr.Remove(&e[1]);
    if (arr.m_count != 3 || store[0] != &e[0] || store[1] != &e[2] || store[2] != &e[3]) { std::printf("00c1ebf0_TEST FAIL\n"); return 1; }
    arr.Remove(&e[3]);
    if (arr.m_count != 2 || store[1] != &e[2]) { std::printf("00c1ebf0_TEST FAIL\n"); return 1; }
    arr.Remove(&e[0]); arr.Remove(&e[2]);
    if (arr.m_count != 0) { std::printf("00c1ebf0_TEST FAIL\n"); return 1; }
    std::printf("00c1ebf0_TEST PASS\n");
    return 0;
}