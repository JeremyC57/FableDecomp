#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CSpriteDesc {
    C3DVector Pos; unsigned int Bank, Frame; unsigned short Flags; float Angle, SizeX, SizeY; long Blend;
    unsigned char A, R, G, B; float Fade; long Set, Op;
    CSpriteDesc(const C3DVector& p, unsigned int bank, unsigned int frame, unsigned short flags, float angle, float sx, float sy, long blend,
                unsigned char a, unsigned char r, unsigned char g, unsigned char b, float fade, long set, long op)
        : Pos(p), Bank(bank), Frame(frame), Flags(flags), Angle(angle), SizeX(sx), SizeY(sy), Blend(blend), A(a), R(r), G(g), B(b), Fade(fade), Set(set), Op(op) { }
};
struct CFlareDesc {
    C3DVector WorldPos; unsigned int BankIndex, Frame; float LightSourceRadius, SizeX, SizeY, MinScale, FadeStart, Angle;
    long BlendMode, Identifier; unsigned char Alpha;
};
static CSpriteDesc* g_seen = 0; static unsigned int g_seenFlags = 0;
struct CSprite {
    unsigned int EnginePrimitiveFlags;
    void UpdateSprite(const CSpriteDesc& d, unsigned int flags) { g_seen = new CSpriteDesc(d); g_seenFlags = flags; }
};
struct CFlare : CSprite {
    float LightSourceRadius, MinLightScale;
    void Update(const CFlareDesc& flare, unsigned int flags)
    {
        EnginePrimitiveFlags = flags;
        CSpriteDesc sprite(flare.WorldPos, flare.BankIndex, flare.Frame, 0, flare.Angle, flare.SizeX, flare.SizeY, 4, flare.Alpha, 0xff, 0xff, 0xff, 0.0f, 0, 0);
        UpdateSprite(sprite, flags);
        LightSourceRadius = flare.LightSourceRadius;
        MinLightScale = flare.MinScale;
    }
};
int main()
{
    CFlareDesc f; f.WorldPos.X = 1.0f; f.WorldPos.Y = 2.0f; f.WorldPos.Z = 3.0f; f.BankIndex = 5; f.Frame = 6; f.LightSourceRadius = 7.0f;
    f.SizeX = 8.0f; f.SizeY = 9.0f; f.MinScale = 0.5f; f.FadeStart = 1.0f; f.Angle = 0.25f; f.BlendMode = 1; f.Identifier = 2; f.Alpha = 128;
    CFlare t; t.Update(f, 0x11);
    if (!g_seen || g_seenFlags != 0x11 || t.EnginePrimitiveFlags != 0x11 || g_seen->Pos.Z != 3.0f || g_seen->Bank != 5 || g_seen->Frame != 6
        || g_seen->Angle != 0.25f || g_seen->SizeX != 8.0f || g_seen->Blend != 4 || g_seen->A != 128 || g_seen->R != 0xff || g_seen->Fade != 0.0f
        || t.LightSourceRadius != 7.0f || t.MinLightScale != 0.5f) { std::printf("00bcdfb0_TEST FAIL\n"); return 1; }
    delete g_seen;
    std::printf("00bcdfb0_TEST PASS\n");
    return 0;
}
