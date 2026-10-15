#include <cstdio>
// Behaviour test for CVtable_012b1a54::CVtable_012b1a54(void*, long, long) (0x00C08750).
struct CVtable_012b1a54 {
    virtual void v00() { }
    long m_index; long m_frame; void* m_pSecond; void* m_pFirst;
    CVtable_012b1a54(void* first, long index, long frame) : m_index(index), m_frame(frame), m_pSecond(0), m_pFirst(first) { }
};
int main()
{
    int x;
    CVtable_012b1a54 o(&x, 5, -3);
    if (o.m_index != 5 || o.m_frame != -3 || o.m_pSecond != 0 || o.m_pFirst != &x) { std::printf("00c08750_TEST FAIL\n"); return 1; }
    std::printf("00c08750_TEST PASS\n");
    return 0;
}