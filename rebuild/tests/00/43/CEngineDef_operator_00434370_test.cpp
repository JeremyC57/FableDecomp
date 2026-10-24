// Behaviour test for CEngineDef::operator= (0x00434370): the base part and all seventeen tuning values are copied and the
// result is the left operand.
#include <cstdio>
class CParentDefClassBase {
public:
    CParentDefClassBase() : globalIndex(0), baseAssigns(0) {}
    virtual ~CParentDefClassBase() {}
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { globalIndex = o.globalIndex; ++baseAssigns; return *this; }
    long globalIndex; int baseAssigns;
};
class CEngineDef : public CParentDefClassBase {
public:
    float LODErrorTolerance, CharacterLODErrorTolerance, LODErrorFactor, SeaHeight, GamePrimitiveDefaultFadeStart,
          GamePrimitiveDefaultFadeRangeRatio, LocalDetailDefaultFadeStart, LocalDetailDefaultFadeRangeRatio, FOV_2D;
    long LocalDetailBooleanAlphaDefaultAlphaRef, DefaultPrimitiveAlphaRef, TestStaticMesh, TestAnimatedMesh, TestAnim,
         TestGraphic, InvalidTextureStandin, InvalidThemeStandin;
};
int main()
{
    CEngineDef a, b;
    a.globalIndex = 42;
    float* fa = &a.LODErrorTolerance; float* fb = &b.LODErrorTolerance;
    a.LODErrorTolerance = 0.5f; a.CharacterLODErrorTolerance = 1.5f; a.LODErrorFactor = 2.5f; a.SeaHeight = -3.0f;
    a.GamePrimitiveDefaultFadeStart = 40.0f; a.GamePrimitiveDefaultFadeRangeRatio = 0.25f; a.LocalDetailDefaultFadeStart = 12.0f;
    a.LocalDetailDefaultFadeRangeRatio = 0.75f; a.FOV_2D = 60.0f;
    a.LocalDetailBooleanAlphaDefaultAlphaRef = 128; a.DefaultPrimitiveAlphaRef = 64; a.TestStaticMesh = 1; a.TestAnimatedMesh = 2;
    a.TestAnim = 3; a.TestGraphic = 4; a.InvalidTextureStandin = 5; a.InvalidThemeStandin = 6;
    (void)fa; (void)fb;
    CEngineDef& r = (b = a);
    int bad = 0;
    if (&r != &b || b.globalIndex != 42 || b.baseAssigns != 1) bad = 1;
    if (b.LODErrorTolerance != 0.5f || b.CharacterLODErrorTolerance != 1.5f || b.LODErrorFactor != 2.5f || b.SeaHeight != -3.0f) bad = 1;
    if (b.GamePrimitiveDefaultFadeStart != 40.0f || b.GamePrimitiveDefaultFadeRangeRatio != 0.25f || b.LocalDetailDefaultFadeStart != 12.0f) bad = 1;
    if (b.LocalDetailDefaultFadeRangeRatio != 0.75f || b.FOV_2D != 60.0f) bad = 1;
    if (b.LocalDetailBooleanAlphaDefaultAlphaRef != 128 || b.DefaultPrimitiveAlphaRef != 64 || b.TestStaticMesh != 1 || b.TestAnimatedMesh != 2) bad = 1;
    if (b.TestAnim != 3 || b.TestGraphic != 4 || b.InvalidTextureStandin != 5 || b.InvalidThemeStandin != 6) bad = 1;
    if (bad) { std::printf("00434370_TEST FAIL\n"); return 1; }
    std::printf("00434370_TEST PASS\n");
    return 0;
}