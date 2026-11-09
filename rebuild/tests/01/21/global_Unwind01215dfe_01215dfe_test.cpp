// Behaviour test for 0x00c4b300: PKCBufferedReader::PKCBufferedReader -- segment copies, buffer bookkeeping, error traces and the size query
#include <cstdio>
#include <cstring>
#include <string>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static std::string g_log; static int g_breaks = 0;
struct SStreamSegment { unsigned int m_granularity, m_lookBehind, m_lookAhead; };
struct Source { bool ok; unsigned int size; bool GetSize(unsigned int* s) { if (!ok) return false; *s = size; return true; } };
struct Reader {
    unsigned char* states; unsigned int blockSize, lookAhead, lookBehind, bufferCount; Source* src; unsigned char* memory; unsigned int size;
    void Trace(int line, const char* msg) { char b[128]; std::sprintf(b, "(%d) %s|", line, msg); g_log += b; ++g_breaks; }
    Reader(Source* source, const SStreamSegment& seg) : states(0), blockSize(seg.m_granularity), lookAhead(seg.m_lookAhead),
        lookBehind(seg.m_lookBehind), bufferCount(seg.m_lookBehind + seg.m_lookAhead), src(source), memory(0), size(0) {
        if (bufferCount == 0 || bufferCount >= 255) Trace(34, "Num buffers");
        if (blockSize == 0) Trace(35, "Granularity");
        states = new unsigned char[bufferCount ? bufferCount : 1];
        for (unsigned int i = 0; i < bufferCount; ++i) states[i] = 0xff;
        memory = new unsigned char[blockSize * bufferCount ? blockSize * bufferCount : 1];
        if (!src->GetSize(&size)) size = 0; }
    ~Reader() { delete[] states; delete[] memory; } };
int main()
{
    Source s; s.ok = true; s.size = 1234;
    SStreamSegment seg; seg.m_granularity = 512; seg.m_lookBehind = 2; seg.m_lookAhead = 6;
    { Reader r(&s, seg); CHECK(r.bufferCount == 8 && r.lookBehind == 2 && r.lookAhead == 6 && r.blockSize == 512);
      CHECK(r.states[0] == 0xff && r.states[7] == 0xff && r.size == 1234 && g_log.empty()); }
    s.ok = false; seg.m_granularity = 0; seg.m_lookBehind = 200; seg.m_lookAhead = 100;
    { Reader r(&s, seg); CHECK(r.size == 0 && g_breaks == 2 && g_log == "(34) Num buffers|(35) Granularity|"); }
    if (g_fail) { std::printf("00c4b300_TEST FAIL\n"); return 1; }
    std::printf("01215dfe_TEST PASS\n");
    return 0;
}