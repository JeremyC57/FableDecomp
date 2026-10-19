// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test exercises a hand-written stand-in, not the candidate)
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