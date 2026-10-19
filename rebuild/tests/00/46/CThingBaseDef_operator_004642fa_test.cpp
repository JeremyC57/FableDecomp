// Behaviour test for retail 0x004642fa: the candidate source below is embedded verbatim (from src/004642fa.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CThingObjectDef::operator=(const CThingObjectDef&) -- CThingBaseDef assignment, then the object-def graphic, scalars and five flag bits
// Retail: Fable.exe (TLC, VC7.1) @ 0x004642FA, 208 bytes.
// Leaf: ??4CThingObjectDef
// Identity: target of the jmp thunk 0x004642F5 in slot 19 of vtable 0x01234E94 = CThingObjectDef (rtti-vtables.tsv);
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
struct CThingObjectDef : CThingBaseDef {
    CEngineGraphic Graphic;                             // +0x70
    float RotationTime;                                 // +0x80
    long Material;                                      // +0x84
    long FloorMaterial;                                 // +0x88
    long OnDeathObject;                                 // +0x8c
    unsigned long Property;                             // +0x90
    long DieOffTimer;                                   // +0x94
    bool IsFragile : 1;                                 // +0x98 bit 0
    bool IsClothing : 1;                                // +0x98 bit 1
    bool IsSelfIlluminating : 1;                        // +0x98 bit 2
    bool UseHighDetailQuadTree : 1;                     // +0x98 bit 3
    bool Electrocutable : 1;                            // +0x98 bit 4
    CThingObjectDef& operator=(const CThingObjectDef&);
};
CThingObjectDef& CThingObjectDef::operator=(const CThingObjectDef& o) {
    CThingBaseDef::operator=(o);
    Graphic = o.Graphic;
    RotationTime = o.RotationTime;
    Material = o.Material;
    FloorMaterial = o.FloorMaterial;
    OnDeathObject = o.OnDeathObject;
    Property = o.Property;
    DieOffTimer = o.DieOffTimer;
    IsFragile = o.IsFragile;
    IsClothing = o.IsClothing;
    IsSelfIlluminating = o.IsSelfIlluminating;
    UseHighDetailQuadTree = o.UseHighDetailQuadTree;
    Electrocutable = o.Electrocutable;
    return *this;
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
static int g_base;
CThingBaseDef& CThingBaseDef::operator=(const CThingBaseDef& o) { ++g_base; memcpy(_base, o._base, sizeof _base); return *this; }
int main() {
    CThingObjectDef a, b; unsigned char* pb = (unsigned char*)&b;
    memset(&a, 0, sizeof a); memset(&b, 0, sizeof b);
    int ok = sizeof(CThingObjectDef) == 0x9c && offsetof(CThingObjectDef, RotationTime) == 0x80 && offsetof(CThingObjectDef, DieOffTimer) == 0x94;
    a._base[0x10] = 0x5a; a.Graphic.BankIndex = 11; a.Graphic.Type = 3; a.RotationTime = 2.0f; a.Material = 4; a.Property = 9; a.DieOffTimer = 30;
    a.IsFragile = 1; a.IsClothing = 0; a.IsSelfIlluminating = 1; a.UseHighDetailQuadTree = 0; a.Electrocutable = 1;
    pb[0x98] = 0xea;                                    /* dst bits: 0,1,0,1,0 + bits 5..7 set */
    CThingObjectDef* r = &(b = a);
    ok = ok && r == &b && g_base == 1 && b._base[0x10] == 0x5a && b.Graphic.BankIndex == 11 && b.Graphic.Type == 3;
    ok = ok && b.RotationTime == 2.0f && b.Material == 4 && b.Property == 9 && b.DieOffTimer == 30;
    ok = ok && pb[0x98] == 0xf5;                        /* bits 0..4 = 1,0,1,0,1; bits 5..7 kept */
    printf(ok ? "004642fa_TEST PASS\n" : "004642fa_TEST FAIL\n"); return ok ? 0 : 1;
}