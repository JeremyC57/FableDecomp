// Behaviour test for CVtable_012b4f9c::~CVtable_012b4f9c (0x00C34780): destroying the in-memory stream reader closes it
// (data pointer and size cleared through the class's own Close, even when a derived class is destroyed).
#include <cstdio>
static const unsigned char* g_seenData = (const unsigned char*)1; static unsigned long g_seenSize = 1;
struct CVtable_012b4f9c {
    virtual ~CVtable_012b4f9c() { Close(); g_seenData = m_pData; g_seenSize = m_size; }
    virtual bool SetStreamInput(void* data, unsigned long size) { m_pData = (const unsigned char*)data; m_position = 0; m_size = size; return true; }
    virtual void Close() { m_pData = 0; m_size = 0; }
    const unsigned char* m_pData; unsigned long m_position; unsigned long m_size;
};
struct CVtable_012b4fac : CVtable_012b4f9c { };
int main()
{
    unsigned char buf[8];
    CVtable_012b4f9c* p = new CVtable_012b4fac;
    p->SetStreamInput(buf, sizeof(buf));
    delete p;
    if (g_seenData != 0 || g_seenSize != 0) { std::printf("00c34780_TEST FAIL\n"); return 1; }
    std::printf("00c34780_TEST PASS\n");
    return 0;
}