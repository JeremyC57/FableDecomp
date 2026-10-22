// Behaviour test for 0x0045912F: the reconstructed body (pragmas stripped) driven through recording stubs.
#include <cstdio>
#include <cstring>
// CThingObjectDef::Transfer(CPersistContext& context) -- persist the object definition: CThingBaseDef part, a zero Health promoted to 1 outside the load/save modes, graphic, rotation time, materials, death object, property, die-off timer, then the five named flag bits
// Leaf: ?Transfer@CThingObjectDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045912F, length 445 bytes (virtual, `ret 4`; unaligned entry, size-optimised
// def module with a frame pointer).
// Identity: vtable 0x01234E94 (CThingObjectDef, RTTI) slot 18 is this body.
// Callees: CThingBaseDef::Transfer 0x00676BE0; CPersistContext typed transfers Transfer(CEngineGraphic&) 0x00451E11,
//   Transfer(float&) 0x00431061, Transfer(long&) 0x00431102, Transfer(unsigned long&) 0x00431020,
//   Transfer<bool>(name, value, default) 0x004045C0.
// Layout: Ego_r.pdb CThingObjectDef (0xA0) minus 4 (retail def base is 4 bytes smaller): base Health +0x50, Graphic +0x70
//   (16-byte CEngineGraphic), RotationTime +0x80, Material +0x84, FloorMaterial +0x88, OnDeathObject +0x8C, Property +0x90,
//   DieOffTimer +0x94, flag bits +0x98. CPersistContext::Mode +0x18 (landed SerialiseGameState rows). No engine header.
// Shape: the CThingBuildingDef::Transfer 0x00454B8E model: the five flags share ONE bool local homed in the dead
//   context-argument slot ([ebp+0Bh]), the `false` default is a one-byte temporary at [ebp-1], and each bit is merged back
//   with xor/and/xor; the constant 1 lives in ebx (bit masks and the mode test).

struct CEngineGraphic {
    unsigned char _body[0x10];
};

struct CPersistContext {
    unsigned char _pad00[0x18];
    long          Mode;                                                      // +0x18
    void Transfer(long& value);                                              // 0x00431102
    void Transfer(unsigned long& value);                                     // 0x00431020
    void Transfer(float& value);                                             // 0x00431061
    void Transfer(CEngineGraphic& value);                                    // 0x00451E11
    void Transfer(const char* name, bool& value, const bool& defaultValue);  // 0x004045C0
};

struct CThingBaseDef {
    virtual void Transfer(CPersistContext& context);                         // 0x00676BE0
    unsigned char _pad04[0x50 - 4];
    float         Health;                                                    // +0x50
    unsigned char _pad54[0x70 - 0x54];
};

struct CThingObjectDef : CThingBaseDef {            // vtable 0x01234E94
    CEngineGraphic Graphic;                         // +0x70
    float          RotationTime;                    // +0x80
    long           Material;                        // +0x84
    long           FloorMaterial;                   // +0x88
    long           OnDeathObject;                   // +0x8c
    unsigned long  Property;                        // +0x90
    long           DieOffTimer;                     // +0x94
    bool           IsFragile : 1;                   // +0x98 bit 0
    bool           IsClothing : 1;                  // +0x98 bit 1
    bool           IsSelfIlluminating : 1;          // +0x98 bit 2
    bool           UseHighDetailQuadTree : 1;       // +0x98 bit 3
    bool           Electrocutable : 1;              // +0x98 bit 4
    virtual void Transfer(CPersistContext& context);
};

void CThingObjectDef::Transfer(CPersistContext& context)
{
    CThingBaseDef::Transfer(context);
    if (context.Mode != 2 && context.Mode != 3 && context.Mode != 1)
    {
        if (Health == 0.0f)
            Health = 1.0f;
    }
    context.Transfer(Graphic);
    context.Transfer(RotationTime);
    context.Transfer(Material);
    context.Transfer(FloorMaterial);
    context.Transfer(OnDeathObject);
    context.Transfer(Property);
    context.Transfer(DieOffTimer);
    bool flag = IsFragile;
    context.Transfer("IsFragile", flag, false);
    IsFragile = flag;
    flag = IsClothing;
    context.Transfer("IsClothing", flag, false);
    IsClothing = flag;
    flag = IsSelfIlluminating;
    context.Transfer("IsSelfIlluminating", flag, false);
    IsSelfIlluminating = flag;
    flag = UseHighDetailQuadTree;
    context.Transfer("UseHighDetailQuadTree", flag, false);
    UseHighDetailQuadTree = flag;
    flag = Electrocutable;
    context.Transfer("Electrocutable", flag, false);
    Electrocutable = flag;
}

static int g_base = 0, g_graphic = 0, g_float = 0, g_long = 0, g_ulong = 0, g_bool = 0;
static bool g_incoming[8];
static const char* g_names[8];
void CThingBaseDef::Transfer(CPersistContext&) { ++g_base; }
void CPersistContext::Transfer(long& v) { ++g_long; v += 1; }
void CPersistContext::Transfer(unsigned long& v) { ++g_ulong; v += 2; }
void CPersistContext::Transfer(float& v) { ++g_float; v *= 2.0f; }
void CPersistContext::Transfer(CEngineGraphic& g) { ++g_graphic; g._body[0] = 9; }
void CPersistContext::Transfer(const char* name, bool& value, const bool& def)
{
    g_incoming[g_bool] = value; g_names[g_bool] = name;
    if (def) value = false;
    value = !value;              // the stub flips every flag
    ++g_bool;
}
static CThingObjectDef d;
static CPersistContext ctx;
int main()
{
    bool ok = true;
    // saving mode: Health left alone
    std::memset(&ctx, 0, sizeof(ctx));
    ctx.Mode = 2;
    d.Health = 0.0f; d.RotationTime = 1.5f; d.Material = 4; d.Property = 10; d.DieOffTimer = 7;
    d.IsFragile = true; d.IsClothing = false; d.IsSelfIlluminating = true; d.UseHighDetailQuadTree = false; d.Electrocutable = true;
    d.Transfer(ctx);
    ok = ok && d.Health == 0.0f && g_base == 1 && g_graphic == 1 && g_float == 1 && g_long == 4 && g_ulong == 1 && g_bool == 5
        && d.RotationTime == 3.0f && d.Material == 5 && d.Property == 12 && d.DieOffTimer == 8 && d.Graphic._body[0] == 9
        && g_incoming[0] && !g_incoming[1] && g_incoming[2] && !g_incoming[3] && g_incoming[4]
        && !d.IsFragile && d.IsClothing && !d.IsSelfIlluminating && d.UseHighDetailQuadTree && !d.Electrocutable
        && std::strcmp(g_names[0], "IsFragile") == 0 && std::strcmp(g_names[4], "Electrocutable") == 0;
    // other mode: a zero Health becomes 1, a non-zero one is kept
    g_bool = 0;
    ctx.Mode = 0; d.Health = 0.0f; d.Transfer(ctx);
    ok = ok && d.Health == 1.0f;
    g_bool = 0;
    ctx.Mode = 5; d.Health = 3.0f; d.Transfer(ctx);
    ok = ok && d.Health == 3.0f;
    g_bool = 0;
    ctx.Mode = 1; d.Health = 0.0f; d.Transfer(ctx);
    ok = ok && d.Health == 0.0f;
    g_bool = 0;
    ctx.Mode = 3; d.Health = 0.0f; d.Transfer(ctx);
    ok = ok && d.Health == 0.0f;
    if (!ok) { std::printf("0045912f_TEST FAIL\n"); return 1; }
    std::printf("0045912f_TEST PASS\n");
    return 0;
}