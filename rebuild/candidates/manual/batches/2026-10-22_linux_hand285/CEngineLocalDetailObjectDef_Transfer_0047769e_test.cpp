#include <cstdio>
#include <cstring>
struct CPersistContext {
    int longs, floats, bools;
    bool loading;
    CPersistContext() : longs(0), floats(0), bools(0), loading(false) {}
    void Transfer(long& value) { ++longs; if (loading) value = 100 + longs; }
    void Transfer(float& value) { ++floats; if (loading) value = 0.5f * floats; }
    void Transfer(const char* name, bool& value, const bool& defaultValue)
    {
        ++bools;
        if (loading) value = defaultValue;
        if (loading && std::strcmp(name, "HasWindSkew") == 0) value = true;
    }
};
struct CEngineLocalDetailObjectDef {
    long  Mesh, ShadowMesh;
    float Probability, ThemeBlendThreshold, Scale, ScaleRandomElement, FadeStart, FadeEnd;
    long  ZSpriteMesh;
    float ZSpriteFadeStart, ZSpriteFadeEnd, SlopeFadeStart, SlopeFadeEnd;
    long  AlphaRef;
    float AlphaMipBias, WindSkewConstantFactor, WindSkewRandomFactor, WindSkewSpeedFactor;
    bool  CastShadows : 1; bool ReceiveShadows : 1; bool IsRepeatedMesh : 1; bool IsZSprite : 1;
    bool  AlphaIsBoolean : 1; bool HasLandscapeNormalLighting : 1; bool TiltToSlope : 1; bool HasWindSkew : 1;
    void Transfer(CPersistContext& context)
    {
        context.Transfer(Mesh); context.Transfer(ShadowMesh);
        context.Transfer(Probability); context.Transfer(ThemeBlendThreshold); context.Transfer(Scale);
        context.Transfer(ScaleRandomElement); context.Transfer(FadeStart); context.Transfer(FadeEnd);
        context.Transfer(ZSpriteMesh);
        context.Transfer(ZSpriteFadeStart); context.Transfer(ZSpriteFadeEnd);
        context.Transfer(SlopeFadeStart); context.Transfer(SlopeFadeEnd);
        context.Transfer(AlphaRef);
        context.Transfer(AlphaMipBias); context.Transfer(WindSkewConstantFactor);
        context.Transfer(WindSkewRandomFactor); context.Transfer(WindSkewSpeedFactor);
        bool flag = CastShadows; context.Transfer("CastShadows", flag, false); CastShadows = flag;
        flag = ReceiveShadows; context.Transfer("ReceiveShadows", flag, true); ReceiveShadows = flag;
        flag = IsRepeatedMesh; context.Transfer("IsRepeatedMesh", flag, false); IsRepeatedMesh = flag;
        flag = IsZSprite; context.Transfer("IsZSprite", flag, false); IsZSprite = flag;
        flag = AlphaIsBoolean; context.Transfer("AlphaIsBoolean", flag, false); AlphaIsBoolean = flag;
        flag = HasLandscapeNormalLighting; context.Transfer("HasLandscapeNormalLighting", flag, false); HasLandscapeNormalLighting = flag;
        flag = TiltToSlope; context.Transfer("TiltToSlope", flag, false); TiltToSlope = flag;
        flag = HasWindSkew; context.Transfer("HasWindSkew", flag, false); HasWindSkew = flag;
    }
};
int main()
{
    CEngineLocalDetailObjectDef d;
    d.CastShadows = true; d.ReceiveShadows = false; d.IsRepeatedMesh = true; d.IsZSprite = true;
    d.AlphaIsBoolean = true; d.HasLandscapeNormalLighting = true; d.TiltToSlope = true; d.HasWindSkew = false;
    CPersistContext c; c.loading = true;
    d.Transfer(c);
    if (c.longs != 4 || c.floats != 14 || c.bools != 8) { std::printf("0047769e_TEST FAIL\n"); return 1; }
    if (d.Mesh != 101 || d.AlphaRef != 104 || d.WindSkewSpeedFactor != 7.0f) { std::printf("0047769e_TEST FAIL\n"); return 1; }
    if (d.CastShadows || !d.ReceiveShadows || d.IsRepeatedMesh || d.TiltToSlope || !d.HasWindSkew) { std::printf("0047769e_TEST FAIL\n"); return 1; }
    std::printf("0047769e_TEST PASS\n");
    return 0;
}
