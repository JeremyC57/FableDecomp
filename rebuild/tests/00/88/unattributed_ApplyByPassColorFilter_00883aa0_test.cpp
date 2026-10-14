#include <cstdio>
#include <cstring>
static int g_calls = 0; static bool g_last;
struct CTCGraphicAppearance { void SetByPassColorFilter(bool b) { ++g_calls; g_last = b; } };
struct CThing { unsigned char _pad00[0x64]; CTCGraphicAppearance* m_pGraphicAppearance; unsigned char _pad68[0x29]; unsigned char m_flags91; };
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CByPassColorFilterRequest { CBaseIntelligentPointer PThing; bool ByPass;
    void ApplyByPassColorFilter() { if (PThing.GetPItem() != 0) { if (!(PThing.GetPItem()->m_flags91 & 1)) { CThing* thing = PThing.GetPItem(); if (thing->m_pGraphicAppearance) thing->m_pGraphicAppearance->SetByPassColorFilter(ByPass); } } } };
int main()
{
    CTCGraphicAppearance ga; CThing t; std::memset(&t, 0, sizeof(t)); t.m_pGraphicAppearance = &ga;
    CByPassColorFilterRequest r; r.PThing.m_pItem = 0; r.ByPass = true; r.ApplyByPassColorFilter();
    r.PThing.m_pItem = &t; t.m_flags91 = 1; r.ApplyByPassColorFilter();
    t.m_flags91 = 0; t.m_pGraphicAppearance = 0; r.ApplyByPassColorFilter();
    int before = g_calls; t.m_pGraphicAppearance = &ga; r.ApplyByPassColorFilter();
    if (before != 0 || g_calls != 1 || g_last != true || (char*)&r.ByPass - (char*)&r != 8) { std::printf("00883aa0_TEST FAIL\n"); return 1; }
    std::printf("00883aa0_TEST PASS\n");
    return 0;
}