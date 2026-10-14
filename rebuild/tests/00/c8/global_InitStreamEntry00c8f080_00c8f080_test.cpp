#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_inits = 0; static void* g_init_this = 0;
struct SStreamKey { long m_live; };
struct SStreamState { long m_0; float m_threshold; unsigned char _rest[0x40]; };
struct CByteBuffer { long m_0, m_4; unsigned char* m_pData; unsigned char* m_pCursor; long m_capacity; void Init() { ++g_inits; g_init_this = this; } };
struct CStreamEntry { long m_0; CByteBuffer m_buffer; unsigned char _pad0[0x28]; SStreamKey* m_pKey; long m_44; long m_48; long m_4c; unsigned char _pad1[0x18]; SStreamState* m_pState; long m_6c; };
static long __fastcall InitStreamEntry(SStreamKey* key, CStreamEntry* entry)
{
    std::memset(entry, 0, sizeof(*entry)); entry->m_pKey = key; entry->m_4c = 0; entry->m_44 = 0;
    if (key->m_live != 0) { SStreamState* state = (SStreamState*)std::calloc(1, sizeof(SStreamState)); entry->m_pState = state; entry->m_buffer.Init(); state->m_threshold = -9999.0f; }
    return 0;
}
int main()
{
    CStreamEntry e; std::memset(&e, 0xee, sizeof(e)); SStreamKey k; k.m_live = 0;
    bool ok = sizeof(CStreamEntry) == 0x70 && sizeof(SStreamState) == 0x48 && InitStreamEntry(&k, &e) == 0 && e.m_pKey == &k && e.m_pState == 0 && e.m_0 == 0 && e.m_6c == 0 && g_inits == 0;
    k.m_live = 1; std::memset(&e, 0xee, sizeof(e));
    ok = ok && InitStreamEntry(&k, &e) == 0 && e.m_pKey == &k && e.m_pState != 0 && g_inits == 1 && g_init_this == &e.m_buffer && e.m_pState->m_threshold == -9999.0f && e.m_pState->m_0 == 0;
    std::free(e.m_pState);
    if (!ok) { std::printf("00c8f080_TEST FAIL\n"); return 1; }
    std::printf("00c8f080_TEST PASS\n"); return 0;
}