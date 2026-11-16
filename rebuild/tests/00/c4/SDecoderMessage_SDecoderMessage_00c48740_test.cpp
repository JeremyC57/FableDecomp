// Test: SDecoderMessage::~SDecoderMessage -- destroying the record releases exactly its +0x14 name pointer, once.
#include <cstdio>
#include <cstddef>
static int g_dtors = 0; static const void* g_where = 0;
struct CNamePointer { void* m_p0; void* m_p4; ~CNamePointer() { ++g_dtors; g_where = this; } };
struct SDecoderMessage { unsigned char head[0x14]; CNamePointer m_name; unsigned char tail[0x40 - 0x14 - 8]; };
int main()
{
    bool ok = offsetof(SDecoderMessage, m_name) == 0x14 && sizeof(SDecoderMessage) == 0x40;
    const void* where = 0;
    { SDecoderMessage r; where = &r.m_name; }
    ok = ok && g_dtors == 1 && g_where == where;
    std::printf(ok ? "00c48740_TEST PASS\n" : "00c48740_TEST FAIL\n");
    return ok ? 0 : 1;
}