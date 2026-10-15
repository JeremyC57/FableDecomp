#include <cstdio>

// CPackedUIntArray destructor (0066c000): delete[] the word buffer, then clear the pointer and the count.
struct CPackedUIntArray {
    unsigned long* m_pData;
    unsigned long  m_count;
    ~CPackedUIntArray();
};
CPackedUIntArray::~CPackedUIntArray()
{
    delete[] m_pData;
    m_pData = 0;
    m_count = 0;
}

int main()
{
    CPackedUIntArray* a = new CPackedUIntArray;
    a->m_pData = new unsigned long[4];
    a->m_count = 4;
    a->~CPackedUIntArray();
    bool ok = a->m_pData == 0 && a->m_count == 0;
    a->~CPackedUIntArray();                     // delete[] of the cleared null buffer is harmless
    ok = ok && a->m_pData == 0;
    operator delete(a);
    if (!ok) {
        std::printf("0066c000_TEST FAIL\n");
        return 1;
    }
    std::printf("0066c000_TEST PASS\n");
    return 0;
}
