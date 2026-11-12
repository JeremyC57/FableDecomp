// Behaviour test for CEngineClothRenderer::CEngineClothRenderer() (retail 0x00BB5040): a portable model of the ctor's logic --
// shader tables sized to 6 and filled by name from vertex shader bank 0, colour-buffer shadow tables only for ps >= 1400 (ps_1_4),
// reference-counted shader assignment, and the scalar member initialisation.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>

struct ShaderData { std::string name; long refs; };
struct VShader {
    ShaderData* Data;
    VShader() : Data(0) {}
    VShader(const VShader& o) : Data(o.Data) { if (Data) ++Data->refs; }
    ~VShader() { if (Data && --Data->refs == 0) delete Data; }
    VShader& operator=(const VShader& o) {
        ShaderData* d = Data;
        if (d) { if (--d->refs == 0) delete d; }
        Data = o.Data; ++Data->refs;
        return *this;
    }
};
struct ShaderResource {
    std::map<std::string, ShaderData*> bank;
    long lookups;
    ShaderResource() : lookups(0) {}
    VShader GetVertexShader(const std::string& n) {
        ++lookups;
        ShaderData*& d = bank[n];
        if (!d) { d = new ShaderData; d->name = n; d->refs = 1; }   // bank keeps one reference
        VShader v; v.Data = d; ++d->refs; return v;
    }
};
static ShaderResource g_bank0;
static unsigned long g_psVersion;

struct ClothModel {
    std::vector<VShader> Tri, DepthShadow, SpotDepthShadow, ColShadow, SpotColShadow, Patch;
    VShader Outline, OutlineSmooth, Tri2D, Back2D, Patch2D, Debug;
    bool ClothDebug; long IndexBufferSize; bool EnableCloth;
    void* PositionBuffer; unsigned long PositionBufferSize; void* NormalBuffer; unsigned long NormalBufferSize;
    static void Fill(std::vector<VShader>& a, ShaderResource* s, const char* suf) {
        static const char* lights[6] = { "", "2POINTLIGHTS_", "2POINTLIGHTS_", "4POINTLIGHTS_", "4POINTLIGHTS_", "5POINTLIGHTS_" };
        a.resize(6);
        for (int i = 0; i < 6; ++i) a[i] = s->GetVertexShader(std::string("VSHADER_STATIC_DIRLIGHT_") + lights[i] + suf);
    }
    ClothModel() : ClothDebug(true), IndexBufferSize(-1) {
        PositionBuffer = 0; PositionBufferSize = 0; NormalBuffer = 0; NormalBufferSize = 0;
        ShaderResource* s = &g_bank0;
        Fill(Tri, s, "FOG");
        Tri2D = s->GetVertexShader("VSHADER_2D_STATIC_DIRLIGHT");
        Back2D = s->GetVertexShader("VSHADER_2D_STATIC_DIRLIGHT_BACKFACE");
        Fill(DepthShadow, s, "FOG_SHADOW");
        Fill(SpotDepthShadow, s, "FOG_SPOT_SHADOW");
        if (g_psVersion >= 1400) {
            Fill(ColShadow, s, "FOG_COLBUFF_SHADOW");
            Fill(SpotColShadow, s, "FOG_SPOT_COLBUFF_SHADOW");
        }
        Outline = s->GetVertexShader("VSHADER_STATIC_OUTLINE");
        OutlineSmooth = s->GetVertexShader("VSHADER_STATIC_OUTLINE_SMOOTH_FILL");
        EnableCloth = true;
        ClothDebug = false;
    }
};

int main()
{
    bool ok = true;
    g_psVersion = 1300;
    {
        ClothModel c;
        ok = ok && g_bank0.lookups == 22 && c.ColShadow.empty() && c.SpotColShadow.empty() && c.Patch.empty();
        ok = ok && c.Tri.size() == 6 && c.Tri[1].Data == c.Tri[2].Data && c.Tri[3].Data == c.Tri[4].Data;
        ok = ok && c.Tri[5].Data->name == "VSHADER_STATIC_DIRLIGHT_5POINTLIGHTS_FOG";
        ok = ok && c.SpotDepthShadow[0].Data->name == "VSHADER_STATIC_DIRLIGHT_FOG_SPOT_SHADOW";
        ok = ok && c.Tri[1].Data->refs == 3;          // bank + elements 1 and 2 (temporaries released)
        ok = ok && c.EnableCloth && !c.ClothDebug && c.IndexBufferSize == -1 && c.PositionBuffer == 0 && c.NormalBufferSize == 0;
        ok = ok && c.Patch2D.Data == 0 && c.Debug.Data == 0 && c.OutlineSmooth.Data->name == "VSHADER_STATIC_OUTLINE_SMOOTH_FILL";
    }
    g_bank0.lookups = 0;
    g_psVersion = 1400;                                // 0x578: ps_1_4 takes the colour-buffer branch (unsigned >=)
    {
        ClothModel c;
        ok = ok && g_bank0.lookups == 34 && c.ColShadow.size() == 6 && c.SpotColShadow.size() == 6;
        ok = ok && c.SpotColShadow[3].Data->name == "VSHADER_STATIC_DIRLIGHT_4POINTLIGHTS_FOG_SPOT_COLBUFF_SHADOW";
        ok = ok && c.Back2D.Data->refs == 2;
    }
    ok = ok && g_bank0.bank["VSHADER_STATIC_OUTLINE"]->refs == 1;   // every member reference dropped again
    if (!ok) { std::printf("00bb5040_TEST FAIL\n"); return 1; }
    std::printf("00bb5040_TEST PASS\n");
    return 0;
}
