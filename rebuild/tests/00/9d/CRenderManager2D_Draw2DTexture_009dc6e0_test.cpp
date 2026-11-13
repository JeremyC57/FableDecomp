// Behaviour test for 0x009DC6E0 CRenderManager2D::Draw2DTexture (box overload): a copy of the model with recording stand-ins for
// the two out-of-line callees (the box conversion and the triangle queue); checks the four corners, UVs, Z / RHW, colours, the
// blend mode for every flag combination, the triangle fan and the texture pointer.
#include <cstdio>
#include <cstring>
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class CRGBColour { public:
    unsigned char B, G, R, A;
    unsigned char GetR() const { return R; } unsigned char GetG() const { return G; } unsigned char GetB() const { return B; } unsigned char GetA() const { return A; }
};
class CTexture { public: int Tag; };
static float g_ScreenRHW = 0.25f;
class CTVertexRHWColSpecTex1Base { public:
    float X, Y, Z, RHW;
    unsigned long DiffuseColour, SpecularColour;
    float U1, V1;
};
template<class T> class CGenericVertex : public T { public:
    void SetRHWFromZ()
    {
        if (this->Z >= 0.0f && this->Z < 0.01f)
            this->RHW = g_ScreenRHW;
        else if (this->Z >= -0.01f && this->Z < 0.0f)
            this->RHW = -g_ScreenRHW;
        else
            this->RHW = 1.0f / this->Z;
    }
};
class CTVertexRHWColSpecTex1 : public CGenericVertex<CTVertexRHWColSpecTex1Base> {};

static void GFVBoxToABox(const C2DBoxF& v, C2DBoxF& a)
{
    a.TLX = v.TLX * 2.0f; a.TLY = v.TLY * 3.0f; a.BRX = v.BRX * 2.0f; a.BRY = v.BRY * 3.0f;
}

struct STri { CTVertexRHWColSpecTex1 V[3]; long Mode; CTexture* Texture; };
static STri g_Tris[8];
static int g_TriCount = 0;

class CRenderManager2D { public:
    void Add2DTri(const CTVertexRHWColSpecTex1& a, const CTVertexRHWColSpecTex1& b, const CTVertexRHWColSpecTex1& c, long mode,
                  CTexture* texture)
    {
        STri& t = g_Tris[g_TriCount++];
        t.V[0] = a; t.V[1] = b; t.V[2] = c; t.Mode = mode; t.Texture = texture;
    }
    bool Draw2DTexture(const C2DBoxF& box, const CRGBColour& colour, CTexture& texture, unsigned long flags)
    {
        unsigned long argb = ((unsigned long)colour.GetA() << 24) | ((unsigned long)colour.GetR() << 16) | ((unsigned long)colour.GetG() << 8) | colour.GetB();
        C2DBoxF screen;
        GFVBoxToABox(box, screen);
        CTVertexRHWColSpecTex1 v[4];
        v[0].X = screen.TLX; v[0].Y = screen.TLY;
        v[1].X = screen.BRX; v[1].Y = screen.TLY;
        v[2].X = screen.BRX; v[2].Y = screen.BRY;
        v[3].X = screen.TLX; v[3].Y = screen.BRY;
        v[0].U1 = 0.0f; v[0].V1 = 0.0f;
        v[1].U1 = 1.0f; v[1].V1 = 0.0f;
        v[2].U1 = 1.0f; v[2].V1 = 1.0f;
        v[3].U1 = 0.0f; v[3].V1 = 1.0f;
        for (int i = 0; i < 4; ++i) {
            v[i].Z = 0.0f;
            v[i].SetRHWFromZ();
            v[i].SpecularColour = 0;
            v[i].DiffuseColour = argb;
        }
        long mode;
        if (flags & 2)
            mode = 2;
        else if (flags & 1)
            mode = 3;
        else
            mode = 1;
        Add2DTri(v[0], v[1], v[2], mode, &texture);
        Add2DTri(v[0], v[2], v[3], mode, &texture);
        return true;
    }
};

static bool VertexIs(const CTVertexRHWColSpecTex1& v, float x, float y, float u, float w, unsigned long argb)
{
    return v.X == x && v.Y == y && v.Z == 0.0f && v.RHW == 0.25f && v.DiffuseColour == argb && v.SpecularColour == 0 &&
           v.U1 == u && v.V1 == w;
}

int main()
{
    CRenderManager2D rm;
    CTexture tex; tex.Tag = 7;
    C2DBoxF box; box.TLX = 1.0f; box.TLY = 2.0f; box.BRX = 5.0f; box.BRY = 7.0f;
    CRGBColour col; col.B = 0x11; col.G = 0x22; col.R = 0x33; col.A = 0x44;
    const unsigned long argb = 0x44332211UL;
    static const long expectMode[4] = { 1, 3, 2, 2 };
    bool ok = true;
    for (unsigned long flags = 0; flags < 4; ++flags) {
        g_TriCount = 0;
        std::memset(g_Tris, 0xCC, sizeof(g_Tris));
        if (!rm.Draw2DTexture(box, col, tex, flags)) ok = false;
        if (g_TriCount != 2) { ok = false; break; }
        const STri& a = g_Tris[0];
        const STri& b = g_Tris[1];
        if (a.Mode != expectMode[flags] || b.Mode != expectMode[flags]) ok = false;
        if (a.Texture != &tex || b.Texture != &tex) ok = false;
        // triangle (0, 1, 2): TL, TR, BR
        if (!VertexIs(a.V[0], 2.0f, 6.0f, 0.0f, 0.0f, argb)) ok = false;
        if (!VertexIs(a.V[1], 10.0f, 6.0f, 1.0f, 0.0f, argb)) ok = false;
        if (!VertexIs(a.V[2], 10.0f, 21.0f, 1.0f, 1.0f, argb)) ok = false;
        // triangle (0, 2, 3): TL, BR, BL
        if (!VertexIs(b.V[0], 2.0f, 6.0f, 0.0f, 0.0f, argb)) ok = false;
        if (!VertexIs(b.V[1], 10.0f, 21.0f, 1.0f, 1.0f, argb)) ok = false;
        if (!VertexIs(b.V[2], 2.0f, 21.0f, 0.0f, 1.0f, argb)) ok = false;
    }
    // flags above bit 1 are ignored by the mode choice
    g_TriCount = 0;
    rm.Draw2DTexture(box, col, tex, 0xFFFFFFFCUL);
    if (g_TriCount != 2 || g_Tris[0].Mode != 1 || g_Tris[1].Mode != 1) ok = false;
    if (!ok) { std::printf("009dc6e0_TEST FAIL\n"); return 1; }
    std::printf("009dc6e0_TEST PASS\n");
    return 0;
}