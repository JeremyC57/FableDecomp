#include <cstdio>
struct CPlayerGui { int x; }; struct CThing { int x; };
static CPlayerGui* g_gui = 0; static long g_reserve = 0;
struct CIntelligentPointer_CThing { void* m_pItem; void* m_pInfo; CIntelligentPointer_CThing(CThing* t) : m_pItem(t), m_pInfo(0) { } };
struct CRGBColour { unsigned char R, G, B, A; CRGBColour(unsigned char b, unsigned char g, unsigned char r, unsigned char a) { B = b; G = g; R = r; A = a; } };
namespace NPlayerGui {
struct CRetailHandleVector { void* m_pFirst; void* m_pLast; void* m_pEnd; void Reserve(long n) { g_reserve = n; } };
struct CDrawBase { virtual void v00() { } unsigned char _body[0x1c - 4]; CRetailHandleVector PrimitiveHandles; CDrawBase(CPlayerGui* g, void* l) { g_gui = g; } };
struct CDrawHighlightingTargetingCursor : CDrawBase { CIntelligentPointer_CThing PTargetedThing; CRGBColour CurrentColour; float CurrentWidthScreenSpace, CurrentWidthWorldSpace; long CurrentFadeStep, Width, CursorType;
    CDrawHighlightingTargetingCursor(CPlayerGui* g, void* l) : CDrawBase(g, l), PTargetedThing(0), CurrentColour(0, 0, 0, 0xff), CurrentWidthScreenSpace(0.0f), CurrentWidthWorldSpace(0.0f), CurrentFadeStep(0), Width(0), CursorType(0) { PrimitiveHandles.Reserve(2); } }; }
int main() { CPlayerGui gui; int list; NPlayerGui::CDrawHighlightingTargetingCursor c(&gui, &list);
    if (g_gui != &gui || c.PTargetedThing.m_pItem || c.CurrentColour.R || c.CurrentColour.G || c.CurrentColour.B || c.CurrentColour.A != 0xff || c.CurrentWidthScreenSpace != 0.0f || c.CurrentWidthWorldSpace != 0.0f || c.CurrentFadeStep || c.Width || c.CursorType || g_reserve != 2) { std::printf("006542a4_TEST FAIL\n"); return 1; } std::printf("006542a4_TEST PASS\n"); return 0; }
