#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00C3AAC0: exercises the same logic through a copy of the model (compiled separately from the source)
static void* g_source = 0; static long g_params[3]; static int g_reset = 0;
struct SReaderParams_00c3aac0 { long m_size, m_offset, m_mode; SReaderParams_00c3aac0(long size, long offset, long mode) : m_size(size), m_offset(offset), m_mode(mode) {} };
struct CReaderBase_00c4b300 {
    virtual void v00() {}
    long m_words[3];
    CReaderBase_00c4b300(void* source, const SReaderParams_00c3aac0& params) { g_source = source; g_params[0] = params.m_size; g_params[1] = params.m_offset; g_params[2] = params.m_mode; m_words[0] = m_words[1] = m_words[2] = 5; }
    void Reset() { ++g_reset; m_words[0] = m_words[1] = m_words[2] = 0; }
};
struct CBufferedReader_00c3aac0 : CReaderBase_00c4b300 {
    CBufferedReader_00c3aac0(void* source, long size) : CReaderBase_00c4b300(source, SReaderParams_00c3aac0(size, 0, 2)) { Reset(); }
};
static bool Run()
{
    int block; CBufferedReader_00c3aac0 reader(&block, 4096);
    return g_source == &block && g_params[0] == 4096 && g_params[1] == 0 && g_params[2] == 2 && g_reset == 1 && reader.m_words[2] == 0;
}
int main()
{
    if (!Run()) { std::printf("00c3aac0_TEST FAIL\n"); return 1; }
    std::printf("00c3aac0_TEST PASS\n");
    return 0;
}
