// Behaviour test for retail 0x004643cf: the candidate source below is embedded verbatim (from src/004643cf.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CThingBuildingDef::operator=(const CThingBuildingDef&) -- CThingBaseDef assignment, then the building-def scalars, graphic and four flag bits
// Retail: Fable.exe (TLC, VC7.1) @ 0x004643CF, 201 bytes.
// Leaf: ??4CThingBuildingDef
// Identity: target of the jmp thunk 0x004643CA in slot 19 of vtable 0x01233144 = CThingBuildingDef (rtti-vtables.tsv);
//   the manifest label "CThingBaseDef::operator=" is propagated.
// Callee: CThingBaseDef::operator= @ 0x00463950 (also the target of the jmp thunk 0x00677390 in slot 19 of
//   vtable 0x0125B664 = CThingBaseDef).
// Layout: PDB (Ego_r) names/types; the TLC CThingBaseDef is 0x70 bytes (PDB 0x74), so retail offset = PDB offset - 4.
//   CEngineGraphic is the PDB's packed 14-byte POD (copied with three movsd + movsw). Bitfield order is the PDB field-list
//   order (struct_layouts_egor.tsv). Local model: no engine/ header exists for these def classes.
#pragma optimize("s", on)
#pragma pack(push, 1)                                   // PDB: sizeof(CEngineGraphic) == 0xe, a natively packed POD
struct CEngineGraphic {
    unsigned long BankIndex;                            // +0x0
    float AnimStep;                                     // +0x4
    float RenderSizeX;                                  // +0x8
    bool AdditiveAlpha;                                 // +0xc
    unsigned char Type;                                 // +0xd
};
#pragma pack(pop)
struct CThingBaseDef {
    unsigned char _base[0x70];                          // CParentDefClassBase + CThingBaseDef fields
    CThingBaseDef& operator=(const CThingBaseDef&);     // 0x00463950
};
enum EReverbEnvironmentType { EReverbEnvironmentType_Force32 = 0x7fffffff };
struct CThingBuildingDef : CThingBaseDef {
    long DieOffTimer;                                   // +0x70
    unsigned long SimBuildingDef;                       // +0x74
    CEngineGraphic Graphic;                             // +0x78
    long WallMaterial;                                  // +0x88
    long FloorMaterial;                                 // +0x8c
    EReverbEnvironmentType ReverbEnvironmentType;       // +0x90
    float ReverbEnvironmentLevel;                       // +0x94
    float OutsideOcclusionLevel;                        // +0x98
    long CameraManagerSetIndex;                         // +0x9c
    bool UseHighDetailQuadTree : 1;                     // +0xa0 bit 0
    bool IsSelfIlluminating : 1;                        // +0xa0 bit 1
    bool IsPartOfHeroGuild : 1;                         // +0xa0 bit 2
    bool HasWeatherMask : 1;                            // +0xa0 bit 3
    CThingBuildingDef& operator=(const CThingBuildingDef&);
};
CThingBuildingDef& CThingBuildingDef::operator=(const CThingBuildingDef& o) {
    CThingBaseDef::operator=(o);
    DieOffTimer = o.DieOffTimer;
    SimBuildingDef = o.SimBuildingDef;
    Graphic = o.Graphic;
    WallMaterial = o.WallMaterial;
    FloorMaterial = o.FloorMaterial;
    ReverbEnvironmentType = o.ReverbEnvironmentType;
    ReverbEnvironmentLevel = o.ReverbEnvironmentLevel;
    OutsideOcclusionLevel = o.OutsideOcclusionLevel;
    CameraManagerSetIndex = o.CameraManagerSetIndex;
    UseHighDetailQuadTree = o.UseHighDetailQuadTree;
    IsSelfIlluminating = o.IsSelfIlluminating;
    IsPartOfHeroGuild = o.IsPartOfHeroGuild;
    HasWeatherMask = o.HasWeatherMask;
    return *this;
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
static int g_base;
CThingBaseDef& CThingBaseDef::operator=(const CThingBaseDef& o) { ++g_base; memcpy(_base, o._base, sizeof _base); return *this; }
int main() {
    CThingBuildingDef a, b; unsigned char* pb = (unsigned char*)&b;
    memset(&a, 0, sizeof a); memset(&b, 0, sizeof b);
    int ok = sizeof(CThingBuildingDef) == 0xa4 && offsetof(CThingBuildingDef, WallMaterial) == 0x88 && offsetof(CThingBuildingDef, CameraManagerSetIndex) == 0x9c;
    a._base[0x6f] = 0x77; a.DieOffTimer = 5; a.SimBuildingDef = 6; a.Graphic.RenderSizeX = 1.25f; a.Graphic.AdditiveAlpha = true;
    a.WallMaterial = 8; a.ReverbEnvironmentType = (EReverbEnvironmentType)3; a.OutsideOcclusionLevel = 0.5f; a.CameraManagerSetIndex = 12;
    a.UseHighDetailQuadTree = 0; a.IsSelfIlluminating = 1; a.IsPartOfHeroGuild = 1; a.HasWeatherMask = 0;
    pb[0xa0] = 0xf9;                                    /* dst bits 1,0,0,1 + bits 4..7 set */
    CThingBuildingDef* r = &(b = a);
    ok = ok && r == &b && g_base == 1 && b._base[0x6f] == 0x77 && b.DieOffTimer == 5 && b.SimBuildingDef == 6;
    ok = ok && b.Graphic.RenderSizeX == 1.25f && b.Graphic.AdditiveAlpha && b.WallMaterial == 8 && b.ReverbEnvironmentType == 3;
    ok = ok && b.OutsideOcclusionLevel == 0.5f && b.CameraManagerSetIndex == 12 && pb[0xa0] == 0xf6;
    printf(ok ? "004643cf_TEST PASS\n" : "004643cf_TEST FAIL\n"); return ok ? 0 : 1;
}