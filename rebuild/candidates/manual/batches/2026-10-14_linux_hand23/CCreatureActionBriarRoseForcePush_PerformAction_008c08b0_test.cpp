#include <cstdio>
#include <cstring>
struct CTCParamsBase;
struct CCharString { char* m_pData; CCharString(const char* text, long length = -1) : m_pData((char*)text) { (void)length; } ~CCharString() { m_pData = 0; } };
struct CTCForcePushPower { unsigned char _pad[0x10]; long m_level; unsigned char _pad14[0x10]; unsigned char m_active; };
static CTCForcePushPower g_power; static bool g_activate; static CTCParamsBase* g_params; static int g_ok;
struct CThing { CTCForcePushPower* AddTC(const CCharString& name, bool activate, CTCParamsBase* params) { g_ok = std::strcmp(name.m_pData, "CTCForcePushPower") == 0; g_activate = activate; g_params = params; return &g_power; } };
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CCreatureAction_BriarRoseForcePush { virtual void PerformAction(); unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; };
void CCreatureAction_BriarRoseForcePush::PerformAction() { CTCForcePushPower* pPower = (CTCForcePushPower*)PCreature.GetPItem()->AddTC("CTCForcePushPower", true, 0); pPower->m_level = 1; pPower->m_active = 1; }
int main()
{
    CThing c; CCreatureAction_BriarRoseForcePush a; a.PCreature.m_pItem = &c; std::memset(&g_power, 0, sizeof(g_power)); a.PerformAction();
    if (!g_ok || g_activate != true || g_params != 0 || g_power.m_level != 1 || g_power.m_active != 1) { std::printf("008c08b0_TEST FAIL\n"); return 1; }
    std::printf("008c08b0_TEST PASS\n");
    return 0;
}
