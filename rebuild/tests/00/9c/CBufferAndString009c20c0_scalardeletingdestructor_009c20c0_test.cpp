#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[8] = ""; static int g_n = 0;
class CWideString { public: void* m_pData; ~CWideString() { g_seq[g_n++] = 's'; } };
struct CBufferHolder_009f9f70 { unsigned char _body[0xc]; ~CBufferHolder_009f9f70() { g_seq[g_n++] = 'b'; } };
struct CBufferAndString_009c20c0 { unsigned char _pad[0x14]; CWideString m_name; CBufferHolder_009f9f70 m_buffer; ~CBufferAndString_009c20c0() { } };
static void Destroy_CBufferAndString_009c20c0(CBufferAndString_009c20c0* o) { delete o; }
int main() {
    CBufferAndString_009c20c0* o = new CBufferAndString_009c20c0; Destroy_CBufferAndString_009c20c0(o); if (std::strcmp(g_seq, "bs")) { std::printf("009c20c0_TEST FAIL\n"); return 1; }
    std::printf("009c20c0_TEST PASS\n"); return 0;
}