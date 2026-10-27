#include <cstdio>

// Behaviour model of CVtable_012b4fec::RemoveAt (0x00C34C80): remove the item at index by value.
template <class T>
struct CPtrArray {
    T* m_data; unsigned int m_capacity; unsigned int m_count;
    void Grow(unsigned int n) { T* d = new T[m_capacity + n]; for (unsigned int i = 0; i < m_count; ++i) d[i] = m_data[i]; delete[] m_data; m_data = d; m_capacity += n; }
    unsigned int GetCount() { return m_count; }
    void Add(T v) { if (m_capacity - m_count == 0) Grow(1); m_data[m_count] = v; ++m_count; }
    void Remove(T v)
    {
        int index = -1;
        for (int i = (int)m_count - 1; i >= 0 && index < 0; --i) if (m_data[i] == v) index = i;
        for (unsigned int i = (unsigned int)index; i < m_count - 1; ++i) m_data[i] = m_data[i + 1];
        --m_count;
    }
};
struct List { CPtrArray<int*> m_items; void RemoveAt(unsigned int index, int) { m_items.Remove(m_items.m_data[index]); } };

int main()
{

    List l; l.m_items.m_data = 0; l.m_items.m_capacity = 0; l.m_items.m_count = 0; int a, b, c;
    l.m_items.Add(&a); l.m_items.Add(&b); l.m_items.Add(&c);
    l.RemoveAt(1, 0);
    if (l.m_items.m_count != 2 || l.m_items.m_data[0] != &a || l.m_items.m_data[1] != &c) { std::printf("00c34c80_TEST FAIL\n"); return 1; }
    l.RemoveAt(0, 0);
    if (l.m_items.m_count != 1 || l.m_items.m_data[0] != &c) { std::printf("00c34c80_TEST FAIL\n"); return 1; }
    delete[] l.m_items.m_data;
    std::printf("00c34c80_TEST PASS\n");
    return 0;
}