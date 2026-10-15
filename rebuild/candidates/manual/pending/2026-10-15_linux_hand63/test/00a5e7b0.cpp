// Behaviour test for the 0x00A5E7B0 deleting destructor: the nested buffer's block is freed
// before its polymorphic member is destroyed.
#include <cstdio>
#include <cstdlib>

static int g_events[8];
static int g_count = 0;
static void* g_freed = 0;

static void FreeBlock(void* p) { g_freed = p; g_events[g_count++] = 1; std::free(p); }

struct CBaseClass {
    virtual ~CBaseClass() { g_events[g_count++] = 2; }
};

struct CObjectBuffer {
    CBaseClass m_object;
    void*      m_data;
    ~CObjectBuffer() { if (m_data) FreeBlock(m_data); }
};

struct CObjectBufferRecord_00a5e7b0 {
    long          m_key;
    CObjectBuffer m_buffer;
};

void DeleteObjectBufferRecord(CObjectBufferRecord_00a5e7b0* record) { delete record; }

int main()
{
    CObjectBufferRecord_00a5e7b0* r = new CObjectBufferRecord_00a5e7b0;
    void* block = std::malloc(8);
    r->m_key = 3; r->m_buffer.m_data = block;
    DeleteObjectBufferRecord(r);
    bool ok = g_freed == block && g_count == 2 && g_events[0] == 1 && g_events[1] == 2;
    r = new CObjectBufferRecord_00a5e7b0;
    r->m_buffer.m_data = 0;
    DeleteObjectBufferRecord(r);
    ok = ok && g_count == 3 && g_events[2] == 2;
    if (!ok) {
        std::printf("00a5e7b0_TEST FAIL\n");
        return 1;
    }
    std::printf("00a5e7b0_TEST PASS\n");
    return 0;
}
