#include <cstdio>

static int g_entriesDestroyed = 0;

struct SEntry {                                    // 0x0c-byte entry with a destructor
    unsigned char _body[0x0c];
    ~SEntry() { ++g_entriesDestroyed; }
};

struct COwner {
    SEntry* m_pEntries; unsigned char* m_pBuffer;
    virtual void v00() { }
    ~COwner()
    {
        delete[] m_pBuffer;
        delete[] m_pEntries;
    }
};

int main()
{
    {
        COwner o; o.m_pBuffer = new unsigned char[16]; o.m_pEntries = new SEntry[3];
    }
    if (g_entriesDestroyed != 3) { std::printf("00c24240_TEST FAIL\n"); return 1; }
    {
        COwner o; o.m_pBuffer = 0; o.m_pEntries = 0;  // both empty: nothing to destroy
    }
    if (g_entriesDestroyed != 3) { std::printf("00c24240_TEST FAIL\n"); return 1; }
    std::printf("00c24240_TEST PASS\n");
    return 0;
}