#include <cstdio>
#include <cstring>
// CThingBuildingDef::Transfer(CPersistContext&) -- persist the building definition: CThingBaseDef part, die-off timer, sim building def, graphic, wall/floor materials, camera set, reverb type/level, outside occlusion, then the four named flag bits
// Leaf: ?Transfer@CThingBuildingDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00454B8E, length 364 bytes (virtual, `ret 4`; unaligned entry, def module).
// Pragma: #pragma optimize("s", on).
// Callees: CThingBaseDef::Transfer @ 0x00676BE0; CPersistContext typed transfers Transfer(long&) @ 0x00431102,
//   Transfer(unsigned long&) @ 0x00431020, Transfer(float&) @ 0x00431061, Transfer(CEngineGraphic&) @ 0x00451E11,
//   Transfer(EReverbEnvironmentType&) @ 0x00454CFA (enum instantiation right after this body),
//   Transfer<bool>(name, value, default) @ 0x004045C0 (catalogued Transfer<signed char>).
// Identity: vtable 0x01233144 (CThingBuildingDef, RTTI) slot 18 is this body.
// Layout: Ego_r.pdb CThingBuildingDef (0xa8) minus 4 (retail def base is 4 bytes smaller, chunk 185/215 lessons):
//   DieOffTimer +0x70, SimBuildingDef +0x74, Graphic +0x78 (16-byte CEngineGraphic), WallMaterial +0x88,
//   FloorMaterial +0x8c, ReverbEnvironmentType +0x90, ReverbEnvironmentLevel +0x94, OutsideOcclusionLevel +0x98,
//   CameraManagerSetIndex +0x9c, flag bits at +0xa0. No generated engine header for the class.
// Shape: the four flags share ONE bool local (separate locals each take a fresh frame byte), homed in the dead context-argument slot ([ebp+0Bh]) and the default is a
//   one-byte temporary at [ebp-1]; the bit is merged back with xor/and/xor (template of landed CEngineThemeDef 0x0045979B).
enum EReverbEnvironmentType { REVERB_ENVIRONMENT_DEFAULT };

struct CEngineGraphic {
    unsigned char _body[0x10];
};

struct CPersistContext {
    void Transfer(long& value);                                              // 0x00431102
    void Transfer(unsigned long& value);                                     // 0x00431020
    void Transfer(float& value);                                             // 0x00431061
    void Transfer(CEngineGraphic& value);                                    // 0x00451E11
    void Transfer(EReverbEnvironmentType& value);                            // 0x00454CFA
    void Transfer(const char* name, bool& value, const bool& defaultValue);  // 0x004045C0
};

struct CThingBaseDef {
    virtual void Transfer(CPersistContext& context);                         // 0x00676BE0
    unsigned char _body[0x70 - 4];
};

struct CThingBuildingDef : CThingBaseDef {          // vtable 0x01233144
    long                   DieOffTimer;             // +0x70
    unsigned long          SimBuildingDef;          // +0x74
    CEngineGraphic         Graphic;                 // +0x78
    long                   WallMaterial;            // +0x88
    long                   FloorMaterial;           // +0x8c
    EReverbEnvironmentType ReverbEnvironmentType;   // +0x90
    float                  ReverbEnvironmentLevel;  // +0x94
    float                  OutsideOcclusionLevel;   // +0x98
    long                   CameraManagerSetIndex;   // +0x9c
    bool                   UseHighDetailQuadTree : 1;   // +0xa0 bit 0
    bool                   IsSelfIlluminating : 1;      // +0xa0 bit 1
    bool                   IsPartOfHeroGuild : 1;       // +0xa0 bit 2
    bool                   HasWeatherMask : 1;          // +0xa0 bit 3
    virtual void Transfer(CPersistContext& context);
};

void CThingBuildingDef::Transfer(CPersistContext& context)
{
    CThingBaseDef::Transfer(context);
    context.Transfer(DieOffTimer);
    context.Transfer(SimBuildingDef);
    context.Transfer(Graphic);
    context.Transfer(WallMaterial);
    context.Transfer(FloorMaterial);
    context.Transfer(CameraManagerSetIndex);
    context.Transfer(ReverbEnvironmentType);
    context.Transfer(ReverbEnvironmentLevel);
    context.Transfer(OutsideOcclusionLevel);
    bool flag = UseHighDetailQuadTree;
    context.Transfer("UseHighDetailQuadTree", flag, false);
    UseHighDetailQuadTree = flag;
    flag = IsSelfIlluminating;
    context.Transfer("IsSelfIlluminating", flag, false);
    IsSelfIlluminating = flag;
    flag = IsPartOfHeroGuild;
    context.Transfer("IsPartOfHeroGuild", flag, false);
    IsPartOfHeroGuild = flag;
    flag = HasWeatherMask;
    context.Transfer("HasWeatherMask", flag, true);
    HasWeatherMask = flag;
}


static int g_order[32]; static int g_n = 0;
static void Log(int k) { if (g_n < 32) g_order[g_n++] = k; }
void CThingBaseDef::Transfer(CPersistContext&) { Log(1); }
void CPersistContext::Transfer(long& v) { Log(2); v += 1; }
void CPersistContext::Transfer(unsigned long& v) { Log(3); v += 1; }
void CPersistContext::Transfer(float& v) { Log(4); v += 1.0f; }
void CPersistContext::Transfer(CEngineGraphic& v) { Log(5); v._body[0] = 0x5a; }
void CPersistContext::Transfer(EReverbEnvironmentType& v) { Log(6); v = (EReverbEnvironmentType)3; }
static const char* g_names[4]; static bool g_defaults[4]; static int g_nb = 0;
void CPersistContext::Transfer(const char* name, bool& value, const bool& defaultValue)
{ if (g_nb < 4) { g_names[g_nb] = name; g_defaults[g_nb] = defaultValue; ++g_nb; } value = !value; }

int main()
{
    static CThingBuildingDef d;
    CPersistContext ctx;
    d.DieOffTimer = 10; d.CameraManagerSetIndex = 20; d.ReverbEnvironmentLevel = 0.5f;
    d.UseHighDetailQuadTree = true; d.IsSelfIlluminating = false; d.IsPartOfHeroGuild = true; d.HasWeatherMask = false;
    d.Transfer(ctx);
    static const int want[10] = { 1, 2, 3, 5, 2, 2, 2, 6, 4, 4 };
    for (int i = 0; i < 10; ++i) if (g_order[i] != want[i]) { std::printf("00454b8e_TEST FAIL\n"); return 1; }
    if (g_n != 10 || g_nb != 4 || d.DieOffTimer != 11 || d.CameraManagerSetIndex != 21 || d.ReverbEnvironmentLevel != 1.5f
        || d.ReverbEnvironmentType != 3 || d.Graphic._body[0] != 0x5a
        || d.UseHighDetailQuadTree || !d.IsSelfIlluminating || d.IsPartOfHeroGuild || !d.HasWeatherMask
        || std::strcmp(g_names[0], "UseHighDetailQuadTree") != 0 || std::strcmp(g_names[3], "HasWeatherMask") != 0
        || g_defaults[0] || g_defaults[1] || g_defaults[2] || !g_defaults[3]) { std::printf("00454b8e_TEST FAIL\n"); return 1; }
    std::printf("00454b8e_TEST PASS\n");
    return 0;
}
