#include <cstdio>
struct CThing { int x; };
static int g_cleared = 0; static void* g_key = 0; static int g_dtors = 0, g_assigns = 0;
struct CTCGraphicAppearance { void ClearMeshEffect(void* k) { ++g_cleared; g_key = k; } };
struct CThingWithAppearance { unsigned char _pad[0x64]; CTCGraphicAppearance* m_pGraphicAppearance; bool alive; CTCGraphicAppearance* GetGraphicAppearance() const { return m_pGraphicAppearance; } };
struct CBaseIntelligentPointer { void* m_pItem; void* m_pInfo; CThingWithAppearance* GetPItem() const { return (CThingWithAppearance*)m_pItem; } ~CBaseIntelligentPointer() { ++g_dtors; } };
struct CIntelligentPointer_CThing : CBaseIntelligentPointer { CIntelligentPointer_CThing(CThing* t) { m_pItem = t; m_pInfo = 0; }
    CIntelligentPointer_CThing& operator=(const CIntelligentPointer_CThing& o) { ++g_assigns; m_pItem = o.m_pItem; return *this; } };
static bool IS_THING_ALIVE(CIntelligentPointer_CThing* p) { return p->m_pItem && ((CThingWithAppearance*)p->m_pItem)->alive; }
namespace NPlayerGui {
struct CDrawHighlightingTargetingCursor { unsigned char _pad[0x28]; CIntelligentPointer_CThing PTargetedThing; unsigned char _pad30[0x14]; long CursorType;
    CDrawHighlightingTargetingCursor() : PTargetedThing(0) { }
    void Clear() { if (IS_THING_ALIVE(&PTargetedThing)) { if (PTargetedThing.GetPItem()->m_pGraphicAppearance != 0) PTargetedThing.GetPItem()->GetGraphicAppearance()->ClearMeshEffect(this); }
        PTargetedThing = (CThing*)0; CursorType = 0; } }; }
int main() { CTCGraphicAppearance ga; CThingWithAppearance thing; thing.m_pGraphicAppearance = &ga; thing.alive = true;
    NPlayerGui::CDrawHighlightingTargetingCursor c; c.PTargetedThing.m_pItem = &thing; c.CursorType = 3; g_dtors = 0; c.Clear();
    if (g_cleared != 1 || g_key != &c || c.PTargetedThing.m_pItem != 0 || c.CursorType != 0 || g_assigns != 1 || g_dtors != 1) { std::printf("00646709_TEST FAIL\n"); return 1; }
    thing.m_pGraphicAppearance = 0; c.PTargetedThing.m_pItem = &thing; c.Clear(); if (g_cleared != 1 || c.PTargetedThing.m_pItem != 0) { std::printf("00646709_TEST FAIL\n"); return 1; } std::printf("00646709_TEST PASS\n"); return 0; }
