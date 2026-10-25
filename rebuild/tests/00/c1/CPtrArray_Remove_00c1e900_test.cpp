// Behaviour test for 00c1e900: CPtrArray<CBlendEntry*>::Remove -- find the last slot holding the value, shift the later entries down one, drop the count
#include <cstdio>

struct CBlendEntry { int m_id; };

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
    CBlendEntry a = { 1 }, b = { 2 }, c = { 3 };
    CBlendEntry* store[5] = { &a, &b, &c, &b, &a };
    CPtrArray<CBlendEntry*> arr; arr.m_data = store; arr.m_capacity = 5; arr.m_count = 5;
    arr.Remove(&b);                                   // the LAST b (slot 3) goes
    if (arr.m_count != 4 || store[0] != &a || store[1] != &b || store[2] != &c || store[3] != &a) { std::printf("00c1e900_TEST FAIL\n"); return 1; }
    arr.Remove(&a);                                   // last a (slot 3): nothing to shift
    if (arr.m_count != 3 || store[2] != &c) { std::printf("00c1e900_TEST FAIL\n"); return 1; }
    arr.Remove(&a);                                   // slot 0: b, c shift down
    if (arr.m_count != 2 || store[0] != &b || store[1] != &c) { std::printf("00c1e900_TEST FAIL\n"); return 1; }
    std::printf("00c1e900_TEST PASS\n");
    return 0;
}