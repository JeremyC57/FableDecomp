// Behaviour test for SBufferRecord00c9dce0::FreeBuffers @ 0x00C9DCE0: every owned block freed, the record zeroed.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_frees = 0;
static void FreeCounted(void* p) { if (p) ++g_frees; std::free(p); }

struct SSubArea00c9dce0 { void* m_a; void* m_b; long m_pad[3]; };
static void ReleaseSubArea(SSubArea00c9dce0* area) { FreeCounted(area->m_a); FreeCounted(area->m_b); area->m_a = area->m_b = 0; }
struct STableBuffer00c9dce0 { void* m_pData; unsigned long m_size; void* m_pAux; unsigned long m_auxSize; };
struct SBufferRecord00c9dce0 {
    unsigned char _head[0x10]; SSubArea00c9dce0 m_area; void* m_pBlock24; unsigned char _pad28[8];
    STableBuffer00c9dce0 m_tables[7]; void* m_pBlockA0; unsigned char _tail[0x10];
    void FreeBuffers()
    {
        ReleaseSubArea(&m_area);
        for (int i = 0; i < 7; ++i)
            FreeCounted(m_tables[i].m_pData);
        FreeCounted(m_pBlock24);
        FreeCounted(m_tables[6].m_pAux);
        FreeCounted(m_pBlockA0);
        std::memset(this, 0, sizeof(*this));
    }
};

int main()
{
    SBufferRecord00c9dce0 r;
    std::memset(&r, 0, sizeof(r));
    r._head[0] = 1;
    r.m_area.m_a = std::malloc(4); r.m_area.m_b = std::malloc(4);
    for (int i = 0; i < 7; ++i) { r.m_tables[i].m_pData = std::malloc(8); r.m_tables[i].m_size = 8; }
    r.m_pBlock24 = std::malloc(4); r.m_tables[6].m_pAux = std::malloc(4); r.m_pBlockA0 = std::malloc(4);
    r.FreeBuffers();
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&r);
    for (unsigned i = 0; i < sizeof(r); ++i) if (bytes[i] != 0) { std::printf("00c9dce0_TEST FAIL\n"); return 1; }
    if (g_frees != 12) { std::printf("00c9dce0_TEST FAIL\n"); return 1; }
    std::printf("00c9dce0_TEST PASS\n");
    return 0;
}
