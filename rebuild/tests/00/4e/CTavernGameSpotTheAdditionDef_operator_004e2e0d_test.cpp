// Behaviour test for CTavernGameSpotTheAdditionDef::operator= (0x004E2E0D): copies the model, spies the
// base and vector assignments (order and count), checks every scalar and colour lands and *this returns.
#include <cstdio>

static int g_order[4];
static int g_calls = 0;

struct CDefClassBase {
    virtual void v00() {}
    unsigned char _body[0x24];
    CDefClassBase& operator=(const CDefClassBase& other)
    {
        for (int i = 0; i < 0x24; ++i) _body[i] = other._body[i];
        g_order[g_calls++] = 1;
        return *this;
    }
};

struct CLongVector {
    long* m_first; long* m_last; long* m_end;
    CLongVector& operator=(const CLongVector& o)
    { m_first = o.m_first; m_last = o.m_last; m_end = o.m_end; g_order[g_calls++] = 2; return *this; }
};

struct CRGBColour { unsigned char B, G, R, A; };

struct CTavernGameSpotTheAdditionDef : CDefClassBase {
    long        Pointer;
    long        Cover;
    float       CoverWidth;
    float       CoverLength;
    CLongVector Knicknacks;
    CLongVector ItemsPerRound;
    float       JoystickScale;
    float       TimeToThink;
    float       MaxCursorHeight;
    float       KnickKnackRadius;
    float       ImpulseScale;
    long        PointerModel;
    float       PointerHeightOffset;
    float       TimeToMoveCover;
    float       TimeToPauseCover;
    float       MaxCoverHeight;
    float       MinimumKnickKnackGap;
    float       LengthMovementScale;
    float       HeightMovementScale;
    float       HighlightWidth;
    CRGBColour  HighlightColour;
    CRGBColour  HighlightColourError;
    CTavernGameSpotTheAdditionDef& operator=(const CTavernGameSpotTheAdditionDef& other);
};

CTavernGameSpotTheAdditionDef& CTavernGameSpotTheAdditionDef::operator=(const CTavernGameSpotTheAdditionDef& other)
{
    CDefClassBase::operator=(other);
    Pointer = other.Pointer;
    Cover = other.Cover;
    CoverWidth = other.CoverWidth;
    CoverLength = other.CoverLength;
    Knicknacks = other.Knicknacks;
    ItemsPerRound = other.ItemsPerRound;
    JoystickScale = other.JoystickScale;
    TimeToThink = other.TimeToThink;
    MaxCursorHeight = other.MaxCursorHeight;
    KnickKnackRadius = other.KnickKnackRadius;
    ImpulseScale = other.ImpulseScale;
    PointerModel = other.PointerModel;
    PointerHeightOffset = other.PointerHeightOffset;
    TimeToMoveCover = other.TimeToMoveCover;
    TimeToPauseCover = other.TimeToPauseCover;
    MaxCoverHeight = other.MaxCoverHeight;
    MinimumKnickKnackGap = other.MinimumKnickKnackGap;
    LengthMovementScale = other.LengthMovementScale;
    HeightMovementScale = other.HeightMovementScale;
    HighlightWidth = other.HighlightWidth;
    HighlightColour = other.HighlightColour;
    HighlightColourError = other.HighlightColourError;
    return *this;
}

int main()
{
    static long k[3], n[2];
    CTavernGameSpotTheAdditionDef a, b;
    int i;
    for (i = 0; i < 0x24; ++i) { a._body[i] = 0; b._body[i] = (unsigned char)(i + 3); }
    float* pa[] = { &a.CoverWidth, &a.CoverLength, &a.JoystickScale, &a.TimeToThink, &a.MaxCursorHeight, &a.KnickKnackRadius,
        &a.ImpulseScale, &a.PointerHeightOffset, &a.TimeToMoveCover, &a.TimeToPauseCover, &a.MaxCoverHeight,
        &a.MinimumKnickKnackGap, &a.LengthMovementScale, &a.HeightMovementScale, &a.HighlightWidth };
    float* pb[] = { &b.CoverWidth, &b.CoverLength, &b.JoystickScale, &b.TimeToThink, &b.MaxCursorHeight, &b.KnickKnackRadius,
        &b.ImpulseScale, &b.PointerHeightOffset, &b.TimeToMoveCover, &b.TimeToPauseCover, &b.MaxCoverHeight,
        &b.MinimumKnickKnackGap, &b.LengthMovementScale, &b.HeightMovementScale, &b.HighlightWidth };
    for (i = 0; i < 15; ++i) { *pa[i] = 0.0f; *pb[i] = (float)(i + 1) * 0.25f; }
    a.Pointer = 0; b.Pointer = 11; a.Cover = 0; b.Cover = 12; a.PointerModel = 0; b.PointerModel = 13;
    a.Knicknacks.m_first = a.Knicknacks.m_last = a.Knicknacks.m_end = 0;
    a.ItemsPerRound.m_first = a.ItemsPerRound.m_last = a.ItemsPerRound.m_end = 0;
    b.Knicknacks.m_first = k; b.Knicknacks.m_last = k + 3; b.Knicknacks.m_end = k + 3;
    b.ItemsPerRound.m_first = n; b.ItemsPerRound.m_last = n + 2; b.ItemsPerRound.m_end = n + 2;
    a.HighlightColour.B = a.HighlightColour.G = a.HighlightColour.R = a.HighlightColour.A = 0;
    a.HighlightColourError = a.HighlightColour;
    b.HighlightColour.B = 1; b.HighlightColour.G = 2; b.HighlightColour.R = 3; b.HighlightColour.A = 4;
    b.HighlightColourError.B = 5; b.HighlightColourError.G = 6; b.HighlightColourError.R = 7; b.HighlightColourError.A = 8;
    CTavernGameSpotTheAdditionDef& r = (a = b);
    bool ok = (&r == &a) && g_calls == 3 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 2;
    for (i = 0; i < 0x24; ++i) if (a._body[i] != (unsigned char)(i + 3)) ok = false;
    for (i = 0; i < 15; ++i) if (*pa[i] != (float)(i + 1) * 0.25f) ok = false;
    if (a.Pointer != 11 || a.Cover != 12 || a.PointerModel != 13
        || a.Knicknacks.m_first != k || a.Knicknacks.m_last != k + 3 || a.ItemsPerRound.m_end != n + 2
        || a.HighlightColour.B != 1 || a.HighlightColour.G != 2 || a.HighlightColour.R != 3 || a.HighlightColour.A != 4
        || a.HighlightColourError.B != 5 || a.HighlightColourError.A != 8)
        ok = false;
    if (!ok) { std::printf("004e2e0d_TEST FAIL\n"); return 1; }
    std::printf("004e2e0d_TEST PASS\n");
    return 0;
}