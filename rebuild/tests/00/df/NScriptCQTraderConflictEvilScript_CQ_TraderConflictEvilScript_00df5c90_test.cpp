#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
#include <new>
static int g_scriptBase = 0, g_timers = 0;
class CScriptBase { public: virtual void v00() { } unsigned char _body[0x3c]; CScriptBase() { ++g_scriptBase; } };
struct __declspec(novtable) CScriptObject : CScriptBase { void* m_pInterface; void* m_pData; CScriptObject(void* data, void* iface) : CScriptBase(), m_pInterface(iface), m_pData(data) { } };
class CGameScriptInterface { public: virtual void v000() { } virtual long RegisterTimer() { return 100 + g_timers++; } };
static CGameScriptInterface g_interface; CGameScriptInterface* g_pGameScriptInterface = &g_interface;
struct SSwordState { long m_swordState; long m_crowdState; long m_pullCount; SSwordState() : m_swordState(0), m_crowdState(0), m_pullCount(0) { } };
struct CV_SwordInTheStoneScript : CScriptObject {
    SSwordState m_state; unsigned char _pad54[0x18]; long m_crowdTimer; virtual void v00() { }
    CV_SwordInTheStoneScript(void* data, void* iface) : CScriptObject(data, iface), m_state() { m_crowdTimer = g_pGameScriptInterface->RegisterTimer(); }
};
int main()
{
    int data, iface; unsigned char raw[sizeof(CV_SwordInTheStoneScript)]; for (unsigned k = 0; k < sizeof(raw); ++k) raw[k] = 0xAA;
    CV_SwordInTheStoneScript* s = new (raw) CV_SwordInTheStoneScript(&data, &iface);
    if (g_scriptBase != 1 || g_timers != 1 || s->m_pData != &data || s->m_pInterface != &iface || s->m_state.m_swordState != 0 || s->m_state.m_crowdState != 0 || s->m_state.m_pullCount != 0 || s->m_crowdTimer != 100 || s->_pad54[0] != 0xAA) { std::printf("00df5c90_TEST FAIL\n"); return 1; }
    std::printf("00df5c90_TEST PASS\n"); return 0;
}