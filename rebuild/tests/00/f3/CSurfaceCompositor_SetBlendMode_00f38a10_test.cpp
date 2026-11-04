// Behaviour test for CSurfaceCompositor::SetBlendMode 0x00F38A10: a model of the cached render-state setters (SetRenderState
// skips an unchanged value, otherwise bookmarks the old value once per capture block, stores the new one and queues the state)
// driven by the row's switch: blend 1 additive (ONE, ONE), 2 alpha (SRC_ALPHA, INV_SRC_ALPHA), 3 opaque (blend off, ONE, ZERO),
// 4 multiplicative (ZERO, SRC_COLOR); an unchanged blend leaves the states alone; an alpha target sets stage-0 ops by layer.
#include <cstdio>
struct CStateInfo { unsigned int CurrentState, DesiredState, BookmarkMask; bool DirtyListFlag; };
struct CStateManager {
    CStateInfo AlphaBlendEnable, SrcBlend, DestBlend, ColourOp0, AlphaOp0, ColourWrite;
    CStateInfo* StackState[64]; unsigned int StackValue[64]; int StackSize;
    CStateInfo* Update[64]; int UpdateSize; unsigned int CurrentBookmarkMask;
    void Set(unsigned int value, CStateInfo& s)
    {
        if (s.DesiredState != value) {
            if ((s.BookmarkMask & CurrentBookmarkMask) == 0) {
                s.BookmarkMask |= CurrentBookmarkMask;
                StackState[StackSize] = &s; StackValue[StackSize] = s.DesiredState; ++StackSize;
            }
            s.DesiredState = value;
            if (!s.DirtyListFlag) { s.DirtyListFlag = true; Update[UpdateSize++] = &s; }
        }
    }
};
static const unsigned int BlendTable[13] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };   // D3DBLEND_ZERO.. (0x01396F68)
static const unsigned int OpTable[4] = { 1, 2, 3, 4 };                                           // D3DTOP_DISABLE.. (0x01396E70)
struct CSurfaceCompositor {
    int CurrentBlend; int LayerCount; bool AlphaTarget; CStateManager* rm;
    void SetBlendMode(int blend)
    {
        CStateManager& m = *rm;
        if (blend != CurrentBlend) {
            switch (blend) {
            case 2: m.Set(1, m.AlphaBlendEnable); m.Set(BlendTable[4], m.SrcBlend); m.Set(BlendTable[5], m.DestBlend); break;
            case 1: m.Set(1, m.AlphaBlendEnable); m.Set(BlendTable[1], m.SrcBlend); m.Set(BlendTable[1], m.DestBlend); break;
            case 3: m.Set(0, m.AlphaBlendEnable); m.Set(BlendTable[1], m.SrcBlend); m.Set(BlendTable[0], m.DestBlend); break;
            case 4: m.Set(1, m.AlphaBlendEnable); m.Set(BlendTable[0], m.SrcBlend); m.Set(BlendTable[2], m.DestBlend); break;
            }
            CurrentBlend = blend;
        }
        if (AlphaTarget) {
            if (LayerCount == 0) { m.Set(OpTable[1], m.AlphaOp0); m.Set(OpTable[1], m.ColourOp0); }
            if (LayerCount == 1) { m.Set(OpTable[2], m.AlphaOp0); m.Set(1 | 2 | 4, m.ColourWrite); }
        }
    }
};
int main()
{
    static CStateManager m;
    m.CurrentBookmarkMask = 1;
    CSurfaceCompositor c; c.CurrentBlend = 0; c.LayerCount = 0; c.AlphaTarget = false; c.rm = &m;
    int ok = 1;
    c.SetBlendMode(2);
    ok &= m.AlphaBlendEnable.DesiredState == 1 && m.SrcBlend.DesiredState == 5 && m.DestBlend.DesiredState == 6 && c.CurrentBlend == 2;
    ok &= m.StackSize == 3 && m.UpdateSize == 3;
    c.SetBlendMode(2);                                    // unchanged: no state touched
    ok &= m.StackSize == 3 && m.UpdateSize == 3;
    c.SetBlendMode(1);
    ok &= m.SrcBlend.DesiredState == 2 && m.DestBlend.DesiredState == 2 && m.StackSize == 3;   // already bookmarked, already queued
    c.SetBlendMode(3);
    ok &= m.AlphaBlendEnable.DesiredState == 0 && m.SrcBlend.DesiredState == 2 && m.DestBlend.DesiredState == 1;
    c.SetBlendMode(4);
    ok &= m.AlphaBlendEnable.DesiredState == 1 && m.SrcBlend.DesiredState == 1 && m.DestBlend.DesiredState == 3;
    c.SetBlendMode(9);                                    // unknown blend: only remembered
    ok &= c.CurrentBlend == 9 && m.DestBlend.DesiredState == 3;
    c.AlphaTarget = true; c.SetBlendMode(9);
    ok &= m.AlphaOp0.DesiredState == 2 && m.ColourOp0.DesiredState == 2 && m.ColourWrite.DesiredState == 0;
    c.LayerCount = 1; c.SetBlendMode(9);
    ok &= m.AlphaOp0.DesiredState == 3 && m.ColourOp0.DesiredState == 2 && m.ColourWrite.DesiredState == 7;
    if (!ok) { std::printf("00f38a10_TEST FAIL\n"); return 1; }
    std::printf("00f38a10_TEST PASS\n");
    return 0;
}