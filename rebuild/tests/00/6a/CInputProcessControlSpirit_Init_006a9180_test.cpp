#include <cstdio>
#include <cstring>
static char g_seq[32] = "";
class CThingPlayerCreature { public: int x; };
struct CBaseIntelligentPointer { virtual void v00() { } CThingPlayerCreature* m_pItem; CBaseIntelligentPointer() { m_pItem = 0; std::strcat(g_seq, "b"); }
    void SetPItem(CThingPlayerCreature* item) { m_pItem = item; std::strcat(g_seq, "s"); } CThingPlayerCreature* GetPItem() const { return m_pItem; } ~CBaseIntelligentPointer() { std::strcat(g_seq, "d"); } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer() : CBaseIntelligentPointer() { } CIntelligentPointer(CThingPlayerCreature* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; } };
class CPlayer { public: CThingPlayerCreature* m_pCreature; CThingPlayerCreature* GetPlayerCreature() { std::strcat(g_seq, "g"); return m_pCreature; } };
class CInputProcessControlSpirit { public: virtual void v00() { } virtual void v01() { } virtual void Init() { PCreature = CIntelligentPointer(MyPlayer->GetPlayerCreature()); }
    unsigned char _pad04[0x10]; void* World; CPlayer* MyPlayer; void* DefinitionManager; void* Component; void* PGamePlayerInterface; CIntelligentPointer PCreature; };
int main()
{
    CThingPlayerCreature creature; CPlayer player; player.m_pCreature = &creature; CInputProcessControlSpirit p; p.MyPlayer = &player; g_seq[0] = 0;
    p.Init(); if (std::strcmp(g_seq, "gbssd") || p.PCreature.GetPItem() != &creature) { std::printf("006a9180_TEST FAIL\n"); return 1; }
    std::printf("006a9180_TEST PASS\n");
    return 0;
}