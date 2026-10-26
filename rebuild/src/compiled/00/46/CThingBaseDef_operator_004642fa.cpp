// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
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
class CEngineGraphic { public:
    unsigned long BankIndex;                            // +0x0
    float AnimStep;                                     // +0x4
    float RenderSizeX;                                  // +0x8
    bool AdditiveAlpha;                                 // +0xc
    unsigned char Type;                                 // +0xd
};
#pragma pack(pop)
class CThingBaseDef { public:
    unsigned char _base[0x70];                          // CParentDefClassBase + CThingBaseDef fields
    CThingBaseDef& operator=(const CThingBaseDef&);     // 0x00463950
};
class CThingObjectDef : public CThingBaseDef { public:
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