// Behaviour test for the 0x009B6C50 deleting destructor: the nested buffer is freed before the
// name string is destroyed, and `delete` releases the record.
#include <cstdio>
#include <cstdlib>

static int g_events[8];
static int g_count = 0;
static void* g_freed = 0;

static void FreeBlock(void* p) { g_freed = p; g_events[g_count++] = 1; std::free(p); }

struct CCharString {
    void* m_pData;
    ~CCharString() { g_events[g_count++] = 2; }
};

struct CNamedBuffer {
    CCharString m_name;
    long        m_value;
    void*       m_data;
    ~CNamedBuffer() { if (m_data) FreeBlock(m_data); }
};

struct CNamedBufferRecord_009b6c50 {
    long         m_key;
    CNamedBuffer m_buffer;
};

void DeleteNamedBufferRecord(CNamedBufferRecord_009b6c50* record) { delete record; }

int main()
{
    CNamedBufferRecord_009b6c50* r = new CNamedBufferRecord_009b6c50;
    void* block = std::malloc(8);
    r->m_key = 1; r->m_buffer.m_name.m_pData = 0; r->m_buffer.m_value = 2; r->m_buffer.m_data = block;
    DeleteNamedBufferRecord(r);
    bool ok = g_freed == block && g_count == 2 && g_events[0] == 1 && g_events[1] == 2;
    r = new CNamedBufferRecord_009b6c50;
    r->m_buffer.m_data = 0;
    DeleteNamedBufferRecord(r);
    ok = ok && g_count == 3 && g_events[2] == 2;
    if (!ok) {
        std::printf("009b6c50_TEST FAIL\n");
        return 1;
    }
    std::printf("009b6c50_TEST PASS\n");
    return 0;
}