#include <cstdio>
struct CRGBColour { unsigned long m_argb; };
struct CLongVector {                             // std::vector<long> (retail 12 bytes)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
};
struct CPersistContext {
    void Transfer(long& value);                  // 0x00431102
    void Transfer(float& value);                 // 0x00431061
    void Transfer(CLongVector& value);           // 0x00466A47
    void Transfer(CRGBColour& value);            // 0x004595A3
};
struct CTavernGameSpotTheAdditionDef {
    unsigned char _head[0x24];                   // CDefClassBase body after the vptr (links, index, flag bits)
    long        Pointer;                         // +0x28
    long        Cover;                           // +0x2c
    float       CoverWidth;                      // +0x30
    float       CoverLength;                     // +0x34
    CLongVector Knicknacks;                      // +0x38
    CLongVector ItemsPerRound;                   // +0x44
    float       JoystickScale;                   // +0x50
    float       TimeToThink;                     // +0x54
    float       MaxCursorHeight;                 // +0x58
    float       KnickKnackRadius;                // +0x5c
    float       ImpulseScale;                    // +0x60
    long        PointerModel;                    // +0x64
    float       PointerHeightOffset;             // +0x68
    float       TimeToMoveCover;                 // +0x6c
    float       TimeToPauseCover;                // +0x70
    float       MaxCoverHeight;                  // +0x74
    float       MinimumKnickKnackGap;            // +0x78
    float       LengthMovementScale;             // +0x7c
    float       HeightMovementScale;             // +0x80
    float       HighlightWidth;                  // +0x84
    CRGBColour  HighlightColour;                 // +0x88
    CRGBColour  HighlightColourError;            // +0x8c
    virtual void Transfer(CPersistContext& context);
};

void CTavernGameSpotTheAdditionDef::Transfer(CPersistContext& context)
{
    context.Transfer(Pointer);
    context.Transfer(Cover);
    context.Transfer(CoverWidth);
    context.Transfer(CoverLength);
    context.Transfer(Knicknacks);
    context.Transfer(ItemsPerRound);
    context.Transfer(JoystickScale);
    context.Transfer(TimeToThink);
    context.Transfer(MaxCursorHeight);
    context.Transfer(KnickKnackRadius);
    context.Transfer(ImpulseScale);
    context.Transfer(PointerModel);
    context.Transfer(PointerHeightOffset);
    context.Transfer(TimeToMoveCover);
    context.Transfer(TimeToPauseCover);
    context.Transfer(MaxCoverHeight);
    context.Transfer(MinimumKnickKnackGap);
    context.Transfer(LengthMovementScale);
    context.Transfer(HeightMovementScale);
    context.Transfer(HighlightWidth);
    context.Transfer(HighlightColour);
    context.Transfer(HighlightColourError);
}
static const void* g_addr[32]; static char g_kind[32]; static int g_n = 0;
void CPersistContext::Transfer(long& v)        { g_addr[g_n] = &v; g_kind[g_n++] = 'l'; v += 1; }
void CPersistContext::Transfer(float& v)       { g_addr[g_n] = &v; g_kind[g_n++] = 'f'; v *= 2.0f; }
void CPersistContext::Transfer(CLongVector& v) { g_addr[g_n] = &v; g_kind[g_n++] = 'v'; }
void CPersistContext::Transfer(CRGBColour& v)  { g_addr[g_n] = &v; g_kind[g_n++] = 'c'; v.m_argb = 0xff00ff00u; }
int main()
{
    CTavernGameSpotTheAdditionDef d;
    d.Pointer = 4; d.PointerModel = 9; d.HighlightWidth = 1.5f; d.HighlightColourError.m_argb = 0;
    CPersistContext ctx;
    CTavernGameSpotTheAdditionDef* p = &d;
    p->Transfer(ctx);
    const char* expect = "llffvvfffffl" "ffffffffcc";
    bool ok = g_n == 22;
    for (int i = 0; ok && i < 22; ++i) ok = g_kind[i] == expect[i];
    ok = ok && g_addr[0] == &d.Pointer && g_addr[4] == &d.Knicknacks && g_addr[5] == &d.ItemsPerRound
        && g_addr[11] == &d.PointerModel && g_addr[19] == &d.HighlightWidth && g_addr[21] == &d.HighlightColourError
        && d.Pointer == 5 && d.PointerModel == 10 && d.HighlightWidth == 3.0f && d.HighlightColourError.m_argb == 0xff00ff00u;
    if (!ok) { std::printf("004e7fd8_TEST FAIL\n"); return 1; }
    std::printf("004e7fd8_TEST PASS\n");
    return 0;
}
