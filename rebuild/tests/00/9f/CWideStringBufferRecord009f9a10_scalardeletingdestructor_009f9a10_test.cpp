// Behaviour test for the 0x009F9A10 deleting destructor: the buffer at the end of the record is
// freed before the wide string member is destroyed.
#include <cstdio>
#include <cstdlib>

static int g_events[8];
static int g_count = 0;
static void* g_freed = 0;

static void FreeBlock(void* p) { g_freed = p; g_events[g_count++] = 1; std::free(p); }

struct CWideString {
    void* m_pData;
    ~CWideString() { g_events[g_count++] = 2; }
};

struct CRawBuffer {
    void* m_first; void* m_last; void* m_end;
    ~CRawBuffer() { if (m_first) FreeBlock(m_first); }
};

struct CWideStringBufferRecord_009f9a10 {
    unsigned char m_header[0x30];
    CWideString   m_text;
    unsigned char m_fields[0x18];
    CRawBuffer    m_buffer;
};

void DeleteWideStringBufferRecord(CWideStringBufferRecord_009f9a10* record) { delete record; }

int main()
{
    CWideStringBufferRecord_009f9a10* r = new CWideStringBufferRecord_009f9a10;
    void* block = std::malloc(8);
    r->m_text.m_pData = 0; r->m_buffer.m_first = block;
    DeleteWideStringBufferRecord(r);
    bool ok = g_freed == block && g_count == 2 && g_events[0] == 1 && g_events[1] == 2;
    r = new CWideStringBufferRecord_009f9a10;
    r->m_buffer.m_first = 0;
    DeleteWideStringBufferRecord(r);
    ok = ok && g_count == 3 && g_events[2] == 2;
    if (!ok) {
        std::printf("009f9a10_TEST FAIL\n");
        return 1;
    }
    std::printf("009f9a10_TEST PASS\n");
    return 0;
}