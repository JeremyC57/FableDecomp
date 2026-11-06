#include <cstdio>
// Copy of the CRenderStateManager inline SetRenderState / AddStateToUpdateList logic (engine/CRenderStateManager.h retail branch).
struct StateInfo { unsigned int Current; unsigned int Desired; unsigned int BookmarkMask; bool Dirty; };
struct StackElement { StateInfo* State; unsigned int Value; };
struct StateManager {
    StackElement Stack[64]; StateInfo* UpdateList[64]; int StackSize; int UpdateSize; unsigned int CurrentBookmarkMask;
    StateInfo AlphaBlendEnable, SourceAlphaBlendMode, DestAlphaBlendMode, AlphaTestEnable, AlphaRef, AlphaTestFunc, PolyFillMode,
        PolyCullMode, ZBufferEnable, FogEnable, ClippingEnable, FilterMin0, FilterMag0, MipMap0, UVModeU0, UVModeV0, Transform0,
        CoordIndex0, Op0, Arg1_0, Arg2_0, AlphaOp0, AlphaArg1_0, AlphaArg2_0, Op1, AlphaOp1;
    unsigned int VertexFormat;
    void Set(unsigned int value, StateInfo& s)
    {
        if (s.Desired != value) {
            if ((s.BookmarkMask & CurrentBookmarkMask) == 0) {
                s.BookmarkMask |= CurrentBookmarkMask;
                Stack[StackSize].State = &s; Stack[StackSize].Value = s.Desired; ++StackSize;
            }
            s.Desired = value;
            if (!s.Dirty) { s.Dirty = true; UpdateList[UpdateSize] = &s; ++UpdateSize; }
        }
    }
};
// The D3D tables the setters index (retail .data values).
static const unsigned int D3DAlphaBlendModeTable[13] = { 1,2,3,4,5,6,7,8,9,10,11,12,13 };
static const unsigned int D3DAlphaFuncModeTable[8] = { 1,2,3,4,5,6,7,8 };
static const unsigned int D3DPolyFillModeTable[3] = { 3,2,1 };
static const unsigned int D3DPolyCullModeTable[3] = { 3,2,1 };
static const unsigned int D3DTextureFilterModeTable[3] = { 1,2,3 };
static const unsigned int D3DTextureMipMapModeTable[3] = { 1,2,0 };
static const unsigned int D3DTextureUVModeTable[4] = { 1,2,3,4 };
static const unsigned int D3DTextureTransformModeTable[10] = { 0,1,2,3,4,0x100,0x101,0x102,0x103,0x104 };
static const unsigned int D3DTextureOperationModeTable[24] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24 };
static const unsigned int D3DTextureArgumentModeTable[20] = { 1,0,2,3,4,0x21,0x20,0x22,0x23,0x24,0x11,0x10,0x12,0x13,0x14,0x31,0x30,0x32,0x33,0x34 };
static void Reset(StateManager& m)
{
    StateInfo* first = &m.AlphaBlendEnable; StateInfo* last = &m.AlphaOp1;
    for (StateInfo* s = first; s <= last; ++s) { s->Current = 0xDEAD; s->Desired = 0xDEAD; s->BookmarkMask = 0; s->Dirty = false; }
    m.StackSize = 0; m.UpdateSize = 0; m.CurrentBookmarkMask = 1; m.VertexFormat = 0;
}

// 0x009A1720 CStreamingFontBank::PrepareToRender, in retail order.
static void PrepareToRender(StateManager& m, bool textured)
{
    m.Set(1, m.AlphaBlendEnable); m.Set(D3DAlphaBlendModeTable[4], m.SourceAlphaBlendMode); m.Set(D3DAlphaBlendModeTable[5], m.DestAlphaBlendMode);
    m.Set(1, m.AlphaTestEnable); m.Set(8, m.AlphaRef); m.Set(D3DAlphaFuncModeTable[6], m.AlphaTestFunc);
    m.Set(D3DPolyFillModeTable[0], m.PolyFillMode); m.Set(D3DPolyCullModeTable[0], m.PolyCullMode); m.Set(1, m.ZBufferEnable);
    m.Set(0, m.FogEnable); m.Set(1, m.ClippingEnable);
    m.Set(D3DTextureFilterModeTable[1], m.FilterMin0); m.Set(D3DTextureFilterModeTable[1], m.FilterMag0);
    m.Set(D3DTextureMipMapModeTable[0], m.MipMap0); m.Set(D3DTextureTransformModeTable[0], m.Transform0);
    if (textured) {
        m.Set(D3DTextureOperationModeTable[1], m.Op0); m.Set(D3DTextureArgumentModeTable[1], m.Arg1_0);
        m.Set(D3DTextureOperationModeTable[3], m.AlphaOp0); m.Set(D3DTextureArgumentModeTable[2], m.AlphaArg1_0);
        m.Set(D3DTextureArgumentModeTable[1], m.AlphaArg2_0); m.Set(D3DTextureOperationModeTable[0], m.Op1);
        m.Set(D3DTextureOperationModeTable[0], m.AlphaOp1);
    }
}
static StateManager g;
int main()
{
    Reset(g);
    PrepareToRender(g, false);
    if (g.UpdateSize != 15 || g.StackSize != 15 || g.SourceAlphaBlendMode.Desired != 5 || g.DestAlphaBlendMode.Desired != 6
        || g.AlphaRef.Desired != 8 || g.AlphaTestFunc.Desired != 7 || g.PolyCullMode.Desired != 3 || g.FogEnable.Desired != 0
        || g.FilterMag0.Desired != 2 || g.Transform0.Desired != 0 || g.Op0.Desired != 0xDEAD || g.Stack[0].Value != 0xDEAD
        || g.UpdateList[4] != &g.AlphaRef) { std::printf("009a1720_TEST FAIL\n"); return 1; }
    PrepareToRender(g, false);                    // unchanged values queue nothing
    if (g.UpdateSize != 15 || g.StackSize != 15) { std::printf("009a1720_TEST FAIL\n"); return 1; }
    PrepareToRender(g, true);
    if (g.UpdateSize != 22 || g.Op0.Desired != 2 || g.Arg1_0.Desired != 0 || g.AlphaOp0.Desired != 4 || g.AlphaArg1_0.Desired != 2
        || g.AlphaArg2_0.Desired != 0 || g.Op1.Desired != 1 || g.AlphaOp1.Desired != 1 || g.Arg2_0.Desired != 0xDEAD)
        { std::printf("009a1720_TEST FAIL\n"); return 1; }
    g.CurrentBookmarkMask = 2; g.AlphaRef.Desired = 3;  // a new capture block bookmarks the old value once
    PrepareToRender(g, true);
    if (g.StackSize != 23 || g.Stack[22].State != &g.AlphaRef || g.Stack[22].Value != 3 || g.AlphaRef.BookmarkMask != 3 || g.UpdateSize != 22)
        { std::printf("009a1720_TEST FAIL\n"); return 1; }
    std::printf("009a1720_TEST PASS\n");
    return 0;
}