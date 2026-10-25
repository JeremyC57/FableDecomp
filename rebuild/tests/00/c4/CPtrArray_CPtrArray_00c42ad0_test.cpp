// Behaviour test for 00c42ad0: CPtrArray::~CPtrArray -- remove the last entry until empty, then free the data
#include <cstdio>

static int g_deleted = 0;
struct CTableEntry { int id; };

template <class T>
struct CPtrArray {
    T* m_data;
    unsigned int m_capacity;
    unsigned int m_count;
    int Find(T value)
    {
        int index = -1;
        for (int i = (int)m_count - 1; i >= 0 && index < 0; --i)
            if (m_data[i] == value)
                index = i;
        return index;
    }
    void Remove(T value)
    {
        for (unsigned int i = Find(value); i < m_count - 1; ++i)
            m_data[i] = m_data[i + 1];
        --m_count;
    }
    void Destroy()
    {
        for (unsigned int count = m_count; count != 0; count = m_count)
            Remove(m_data[count - 1]);
        if (m_data) {
            delete[] m_data;
            ++g_deleted;
            m_data = 0;
        }
    }
};

int main()
{
    CTableEntry a = { 1 }, b = { 2 }, c = { 3 };
    CPtrArray<CTableEntry*> arr;
    arr.m_data = new CTableEntry*[4];
    arr.m_capacity = 4;
    arr.m_count = 4;
    arr.m_data[0] = &a; arr.m_data[1] = &b; arr.m_data[2] = &a; arr.m_data[3] = &c;   // a duplicate entry
    arr.Destroy();
    if (arr.m_count != 0 || arr.m_data != 0 || g_deleted != 1 || a.id != 1 || c.id != 3) { std::printf("00c42ad0_TEST FAIL\n"); return 1; }
    CPtrArray<CTableEntry*> empty;
    empty.m_data = 0; empty.m_capacity = 0; empty.m_count = 0;
    empty.Destroy();
    if (empty.m_data != 0 || g_deleted != 1) { std::printf("00c42ad0_TEST FAIL\n"); return 1; }
    std::printf("00c42ad0_TEST PASS\n");
    return 0;
}