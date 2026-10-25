// behaviour test for 0x00C22B40 PKCFrallocStatic::Push: ring append with an "Overflow" warning when full
#include <cstdio>
#include <cstring>

static char g_log[256];
static int g_breaks = 0;
typedef void (*PFNLOGSINK)(const char* text);
static void Sink(const char* text) { std::strcat(g_log, text); }
static PFNLOGSINK g_pfnLogSink = Sink;

class CString {                                 // stand-in for the library string (fixed buffer)
public:
    char m_pchData[200];
    CString() { m_pchData[0] = 0; }
    CString* Append(const char* text) { std::strcat(m_pchData, text); return this; }
    CString* AppendNumber(long value) { char b[16]; std::sprintf(b, "%ld", value); return Append(b); }
};
struct CDebugBreak { CDebugBreak() { ++g_breaks; } };

struct PKCFrallocStatic {
    unsigned int m_capacity, m_count, m_write, m_10;
    void** m_pBlocks;
    void Push(void* block);
};

void PKCFrallocStatic::Push(void* block)
{
    if (m_count >= m_capacity) {
        {
            CString line;
            line.Append("d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h")->Append("(")->AppendNumber(85)->Append(") : ")->Append("Overflow")->Append("\n");
            if (g_pfnLogSink)
                g_pfnLogSink(line.m_pchData);
        }
        CDebugBreak();
    }
    m_pBlocks[m_write] = block;
    m_write = (m_write + 1) % m_capacity;
    ++m_count;
}

int main()
{
    void* slots[3] = { 0, 0, 0 };
    int a, b, c;
    PKCFrallocStatic r; r.m_capacity = 3; r.m_count = 1; r.m_write = 1; r.m_10 = 0; r.m_pBlocks = slots;
    g_log[0] = 0;
    r.Push(&a); r.Push(&b);
    if (slots[1] != &a || slots[2] != &b || r.m_write != 0 || r.m_count != 3 || g_breaks != 0 || g_log[0] != 0) { std::printf("00c22b40_TEST FAIL\n"); return 1; }
    r.Push(&c);                                 // full: warns, still writes and wraps
    if (g_breaks != 1 || std::strcmp(g_log, "d:\\dev\\audio\\ver_fable_1.5_b\\libpk\\PKCFrallocStatic.h(85) : Overflow\n") != 0) { std::printf("00c22b40_TEST FAIL\n"); return 1; }
    if (slots[0] != &c || r.m_write != 1 || r.m_count != 4) { std::printf("00c22b40_TEST FAIL\n"); return 1; }
    std::printf("00c22b40_TEST PASS\n");
    return 0;
}