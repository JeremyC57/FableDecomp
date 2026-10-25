// Behaviour test for 00c31f00: CMemoryStream destructor -- frees the buffer when present, clears cursor and buffer
#include <cstdio>
static int g_frees = 0;
struct IDataStream { virtual bool Read(void*, unsigned long) = 0; virtual ~IDataStream() {} };
struct CMemoryStream : IDataStream {
    char* m_pBuffer; char* m_pCursor; unsigned long m_remaining;
    virtual bool Read(void*, unsigned long) { return false; }
    virtual ~CMemoryStream() { if (m_pBuffer) { ::operator delete(m_pBuffer); ++g_frees; m_pCursor = 0; m_pBuffer = 0; } }
};
static bool Check()
{
    { CMemoryStream s; s.m_pBuffer = (char*)::operator new(8); s.m_pCursor = s.m_pBuffer + 2; s.m_remaining = 6; }
    { CMemoryStream e; e.m_pBuffer = 0; e.m_pCursor = 0; e.m_remaining = 0; }
    IDataStream* p = new CMemoryStream; ((CMemoryStream*)p)->m_pBuffer = (char*)::operator new(4); delete p;
    return g_frees == 2 && sizeof(CMemoryStream) == 0x10;
}
int main()
{
    if (!Check()) { std::printf("00c31f00_TEST FAIL\n"); return 1; }
    std::printf("00c31f00_TEST PASS\n");
    return 0;
}
