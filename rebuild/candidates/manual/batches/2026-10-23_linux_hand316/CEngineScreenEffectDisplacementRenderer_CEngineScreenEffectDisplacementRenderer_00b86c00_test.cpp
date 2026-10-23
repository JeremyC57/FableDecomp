#include <cstdio>
#include <cstring>
// Model of the screen displacement renderer constructor: supported only when the init flags carry both 0x80 and 8
// (else the 2D/3D switches are off and the component is disabled); when supported the textures are created, the
// three screen quads are filled (the texture -> back-buffer quad with a half-texel offset of 1/512, the displacement
// quad on the lower-left quarter) and four shaders are looked up by name; then clear size 128 x 128, amount 0.25.
struct Vert { float X, Y, Z; unsigned long Colour; float U, V; };
static const char* g_names[8]; static int g_nameCount = 0; static int g_texInits = 0;
static int Lookup(const char* name) { g_names[g_nameCount++] = name; return g_nameCount; }
static void Quad(Vert* q, float x0, float y0, float x1, float y1, float uOff)
{
    q[0].X = x0; q[0].Y = y0; q[1].X = x1; q[1].Y = y0; q[2].X = x1; q[2].Y = y1; q[3].X = x0; q[3].Y = y1;
    for (int i = 0; i < 4; ++i) q[i].Z = 0.0f;
    q[0].U = uOff; q[0].V = 1.0f + uOff; q[1].U = 1.0f + uOff; q[1].V = 1.0f + uOff;
    q[2].U = 1.0f + uOff; q[2].V = uOff; q[3].U = uOff; q[3].V = uOff;
}
struct Renderer {
    bool RenderEnabled, EffectSupported, Enable2D, Enable3D, TextureView, UsesShadowZ, HasContent;
    long PageHandle, ClearX, ClearY; float Amount;
    Vert BackToTex[4], TexToBack[4], DispToBack[4];
    int VsCopy, PsCopy, VsDisp, PsDisp;
    Renderer(unsigned long flags) : RenderEnabled(true), VsCopy(0), PsCopy(0), VsDisp(0), PsDisp(0)
    {
        if ((flags & 0x80) && (flags & 8)) { EffectSupported = true; Enable2D = true; Enable3D = true; }
        else { EffectSupported = false; Enable2D = false; Enable3D = false; RenderEnabled = false; }
        PageHandle = -1; TextureView = false; UsesShadowZ = true;
        if (EffectSupported) {
            ++g_texInits;
            Quad(BackToTex, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f);
            Quad(TexToBack, -1.0f, -1.0f, 1.0f, 1.0f, 0.001953125f);
            Quad(DispToBack, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f);
            for (long i = 0; i < 4; ++i) { TexToBack[i].Colour = 0xffffffff; BackToTex[i].Colour = 0xffffffff; DispToBack[i].Colour = 0xffffffff; }
            VsCopy = Lookup("VSHADER_SCREEN_FX_COPY"); PsCopy = Lookup("PSHADER_TEXTURE_ONLY");
            VsDisp = Lookup("VSHADER_SCREEN_FX_DISPLACEMENT"); PsDisp = Lookup("PSHADER_SCREEN_FX_DISPLACEMENT");
        }
        ClearX = 0x80; ClearY = 0x80; HasContent = false; Amount = 0.25f;
    }
};
int main()
{
    int bad = 0;
    Renderer off(0x80);
    if (off.EffectSupported || off.Enable2D || off.RenderEnabled || g_nameCount != 0 || g_texInits != 0) bad = 1;
    if (off.PageHandle != -1 || off.ClearX != 128 || off.Amount != 0.25f || !off.UsesShadowZ) bad = 1;
    Renderer on(0x88);
    if (!on.EffectSupported || !on.Enable3D || !on.RenderEnabled || g_nameCount != 4 || g_texInits != 1) bad = 1;
    if (std::strcmp(g_names[3], "PSHADER_SCREEN_FX_DISPLACEMENT") != 0 || on.VsDisp != 3) bad = 1;
    if (on.DispToBack[2].X != 0.0f || on.DispToBack[2].Y != 0.0f || on.DispToBack[3].X != -1.0f) bad = 1;
    if (on.TexToBack[1].U != 1.001953125f || on.TexToBack[3].V != 0.001953125f || on.BackToTex[0].V != 1.0f) bad = 1;
    if (on.DispToBack[1].Colour != 0xffffffff || on.ClearY != 128 || on.HasContent) bad = 1;
    if (bad) { std::printf("00b86c00_TEST FAIL\n"); return 1; }
    std::printf("00b86c00_TEST PASS\n");
    return 0;
}
