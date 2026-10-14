#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_killed = 0; static long g_reason = -1; static int g_baseDtor = 0;
struct CThing_00811040 { unsigned char _pad[0x91]; unsigned char m_flags; void Kill(long reason) { ++g_killed; g_reason = reason; } };
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CThing_00811040* GetPItem() { return (CThing_00811040*)m_pItem; } ~CBaseIntelligentPointer() { ++g_baseDtor; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } };
namespace NEntityEvents { struct CEventBase { virtual void v00() { } long m_eventType; ~CEventBase() { } };
struct CEventSoldItem : CEventBase { CIntelligentPointer m_giver; CIntelligentPointer m_item; bool m_ownsItem; virtual void v00() { }
    ~CEventSoldItem() { if (m_ownsItem) { if (m_item.GetPItem()) { if (!(m_item.GetPItem()->m_flags & 1)) m_item.GetPItem()->Kill(0); } } } }; }
int main() {
    CThing_00811040 t; t.m_flags = 0; { NEntityEvents::CEventSoldItem e; e.m_ownsItem = true; e.m_item.m_pItem = &t; e.m_giver.m_pItem = 0; } if (g_killed != 1 || g_reason != 0 || g_baseDtor != 2) { std::printf("00811040_TEST FAIL\n"); return 1; }
    t.m_flags = 1; { NEntityEvents::CEventSoldItem e; e.m_ownsItem = true; e.m_item.m_pItem = &t; e.m_giver.m_pItem = 0; } if (g_killed != 1) { std::printf("00811040_TEST FAIL\n"); return 1; }
    t.m_flags = 0; { NEntityEvents::CEventSoldItem e; e.m_ownsItem = false; e.m_item.m_pItem = &t; e.m_giver.m_pItem = 0; } if (g_killed != 1) { std::printf("00811040_TEST FAIL\n"); return 1; }
    { NEntityEvents::CEventSoldItem e; e.m_ownsItem = true; e.m_item.m_pItem = 0; e.m_giver.m_pItem = 0; } if (g_killed != 1 || g_baseDtor != 8) { std::printf("00811040_TEST FAIL\n"); return 1; }
    std::printf("00811040_TEST PASS\n"); return 0;
}