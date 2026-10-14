#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_calls = 0; static void* g_stream = 0; static long g_value = 0; static long g_count = 0;
struct CBitstream { int x; };
static void __fastcall WriteBits(CBitstream* stream, long value, long count) { ++g_calls; g_stream = stream; g_value = value; g_count = count; }
struct CCodeLengths { unsigned char _pad[8]; long* m_lengths; };
struct CCodeTable { unsigned char _pad0[0xc]; CCodeLengths* m_pLengths; unsigned char _pad1[4]; long* m_codes;
    long __fastcall WriteCode(long index, CBitstream* stream) { WriteBits(stream, m_codes[index], m_pLengths->m_lengths[index]); return m_pLengths->m_lengths[index]; } };
int main()
{
    long lengths[3] = { 3, 5, 9 }; long codes[3] = { 1, 0x1f, 0x155 }; CCodeLengths cl; cl.m_lengths = lengths; CCodeTable t; t.m_pLengths = &cl; t.m_codes = codes; CBitstream s;
    long r = t.WriteCode(2, &s);
    bool ok = r == 9 && g_calls == 1 && g_stream == &s && g_value == 0x155 && g_count == 9;
    if (!ok) { std::printf("00c96140_TEST FAIL\n"); return 1; }
    std::printf("00c96140_TEST PASS\n"); return 0;
}