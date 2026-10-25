// Behaviour test for 00c167b0: PKCAutoDeleteArray constructor -- empty (null data, zero count), 8 bytes
#include <cstdio>
template<class T> class PKCAutoDeleteArray {
public:
    T* m_pData; unsigned long m_count;
    PKCAutoDeleteArray() : m_pData(0), m_count(0) {}
};
int main()
{
    PKCAutoDeleteArray<unsigned short>* a = new PKCAutoDeleteArray<unsigned short>;
    bool ok = a->m_pData == 0 && a->m_count == 0 && sizeof(*a) == 8;
    delete a;
    if (!ok) { std::printf("00c167b0_TEST FAIL\n"); return 1; }
    std::printf("00c167b0_TEST PASS\n");
    return 0;
}
