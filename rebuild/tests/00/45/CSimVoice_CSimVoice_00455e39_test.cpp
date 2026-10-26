#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
#include <string.h>
#include <new>
static int g_instances = 0;
class CInitBaseClass { public: virtual ~CInitBaseClass() { } bool m_initialised; CInitBaseClass() : m_initialised(false) { ++g_instances; } };
struct SVoiceEntry { long first; long second; };
struct CKeyPairCompareLess { };
struct CVoiceMap {
    SVoiceEntry* m_pFirst; SVoiceEntry* m_pLast; SVoiceEntry* m_pEnd; CKeyPairCompareLess m_compare; bool m_dirty;
    CVoiceMap() : m_pFirst(0), m_pLast(0), m_pEnd(0), m_dirty(false) { }
};
class CSimVoice : public CInitBaseClass { public: CVoiceMap Voices; CSimVoice() { } };
int main()
{
    unsigned char raw[sizeof(CSimVoice)]; memset(raw, 0xcd, sizeof(raw));
    CSimVoice* v = new (raw) CSimVoice;
    if (g_instances != 1 || v->m_initialised || v->Voices.m_pFirst || v->Voices.m_pLast || v->Voices.m_pEnd || v->Voices.m_dirty) { std::printf("00455e39_TEST FAIL\n"); return 1; }
    v->~CSimVoice();
    std::printf("00455e39_TEST PASS\n");
    return 0;
}