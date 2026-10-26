#include <cstdio>
static int g_notified = 0, g_lastProperty = 0;
class CBaseObject { public: unsigned char _pad[0x28]; unsigned long m_flags28; void OnPropertyChanged(long id) { ++g_notified; g_lastProperty = id; } };
struct CBaseIntelligentPointer { virtual void v00() { } CBaseObject* m_pItem; CBaseObject* GetPItem() const { return m_pItem; } };
class CCreatureAction_WhisperSommersaultForward { public:
    virtual void v00() { } unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; unsigned char _pad10[0x114 - 0x10]; unsigned char Finished;
    void PerformAction() { if (PCreature.GetPItem()->m_flags28 & 0x2000) PCreature.GetPItem()->OnPropertyChanged(77); Finished = 1; }
};
int main()
{
    CBaseObject creature; creature.m_flags28 = 0; CCreatureAction_WhisperSommersaultForward a; a.PCreature.m_pItem = &creature; a.Finished = 0;
    a.PerformAction(); if (g_notified != 0 || a.Finished != 1) { std::printf("008c0b40_TEST FAIL\n"); return 1; }
    creature.m_flags28 = 0x2000; a.Finished = 0; a.PerformAction(); if (g_notified != 1 || g_lastProperty != 77 || a.Finished != 1) { std::printf("008c0b40_TEST FAIL\n"); return 1; }
    std::printf("008c0b40_TEST PASS\n"); return 0;
}