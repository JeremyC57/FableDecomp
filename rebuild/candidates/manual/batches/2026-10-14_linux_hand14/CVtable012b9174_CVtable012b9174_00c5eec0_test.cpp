#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
struct CVtable_012b9174 {
    virtual void v00() {}
    unsigned char _body[0x600];
    void* m_pCurrent;
    CVtable_012b9174() : m_pCurrent(0) {}
};
static bool Check()
{
    unsigned char raw[sizeof(CVtable_012b9174)]; std::memset(raw, 0xAB, sizeof raw);
    CVtable_012b9174* p = new (raw) CVtable_012b9174;
    if (p->m_pCurrent != 0) return false;
    if (p->_body[0] != 0xAB || p->_body[0x5ff] != 0xAB) return false;   // the body is untouched
    return true;
}
int main()
{
    if (!Check()) { std::printf("00c5eec0_TEST FAIL\n"); return 1; }
    std::printf("00c5eec0_TEST PASS\n");
    return 0;
}
