#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_order = 0, g_code = 0, g_synth = 0; static void* g_codes = 0; static long g_count1 = 0, g_count2 = 0; static void* g_src = 0;
struct SCodecSource { long m_mode; long m_count; void* m_pCodes; };
static void* __fastcall BuildCodeTable(void* codes, long count, long flags) { g_code = ++g_order; g_codes = codes; g_count1 = count; return (void*)0x100; }
static void* __fastcall BuildSynthesisTable(SCodecSource* source, long count, long flags) { g_synth = ++g_order; g_src = source; g_count2 = count; return (void*)0x200; }
struct CCodecStage { long m_mode; long m_count; long m_remaining; SCodecSource* m_pSource; void* m_pSynthesis; void* m_pCodeTable; long m_18, m_1c, m_20, m_24, m_28; };
static long __fastcall InitCodecStage(CCodecStage* stage, SCodecSource* source)
{
    std::memset(stage, 0, sizeof(*stage)); stage->m_pSource = source; stage->m_count = source->m_count; stage->m_remaining = source->m_count; stage->m_mode = source->m_mode;
    stage->m_pCodeTable = BuildCodeTable(source->m_pCodes, source->m_count, 0); stage->m_pSynthesis = BuildSynthesisTable(source, source->m_count, 0); return 0;
}
int main()
{
    CCodecStage st; std::memset(&st, 0xee, sizeof(st)); SCodecSource src; src.m_mode = 3; src.m_count = 12; src.m_pCodes = (void*)0x44;
    bool ok = sizeof(CCodecStage) == 0x2c && InitCodecStage(&st, &src) == 0 && st.m_pSource == &src && st.m_count == 12 && st.m_remaining == 12 && st.m_mode == 3 && st.m_28 == 0
        && g_code == 1 && g_synth == 2 && g_codes == (void*)0x44 && g_count1 == 12 && g_src == &src && g_count2 == 12 && st.m_pCodeTable == (void*)0x100 && st.m_pSynthesis == (void*)0x200;
    if (!ok) { std::printf("00c954a0_TEST FAIL\n"); return 1; }
    std::printf("00c954a0_TEST PASS\n"); return 0;
}