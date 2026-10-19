// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: wrong callee: CPersistContext::Transfer float/long overloads swapped (0x431061 float, 0x431102 long))
// CEngineDef::Transfer(CPersistContext&) -- transfer the engine definition's scalar fields in the retail order
// Retail: Fable.exe @ 0x00433FE0, 208 bytes. Float fields go through CPersistContext::Transfer(float&) @ 0x00431061
// (fldz/fld/fstp body), long fields through CPersistContext::Transfer(long&) @ 0x00431102 (calls WriteLong 0x993eb0).
// Local model: no engine/CEngineDef.h exists. Field names/types are the PDB's (fable_types.h), but the retail object
// places them 4 bytes earlier than the Ego_r layout (floats 0x3C..0x5C, longs 0x60..0x7C), as the lea offsets show.
struct CPersistContext {
 void Transfer(float&); // 0x00431061
 void Transfer(long&);  // 0x00431102
};
struct CEngineDef {
 unsigned char _base[0x3c];
 float LODErrorTolerance, CharacterLODErrorTolerance, LODErrorFactor, SeaHeight;              // 0x3c..0x48
 float GamePrimitiveDefaultFadeStart, GamePrimitiveDefaultFadeRangeRatio;                     // 0x4c, 0x50
 float LocalDetailDefaultFadeStart, LocalDetailDefaultFadeRangeRatio, FOV_2D;                 // 0x54, 0x58, 0x5c
 long LocalDetailBooleanAlphaDefaultAlphaRef, DefaultPrimitiveAlphaRef;                       // 0x60, 0x64
 long TestStaticMesh, TestAnimatedMesh, TestAnim, TestGraphic;                                // 0x68..0x74
 long InvalidTextureStandin, InvalidThemeStandin;                                             // 0x78, 0x7c
 void Transfer(CPersistContext&);
};
void CEngineDef::Transfer(CPersistContext& c) {
 c.Transfer(LODErrorTolerance); c.Transfer(CharacterLODErrorTolerance); c.Transfer(LODErrorFactor); c.Transfer(SeaHeight);
 c.Transfer(LocalDetailBooleanAlphaDefaultAlphaRef); c.Transfer(DefaultPrimitiveAlphaRef);
 c.Transfer(GamePrimitiveDefaultFadeStart); c.Transfer(GamePrimitiveDefaultFadeRangeRatio);
 c.Transfer(LocalDetailDefaultFadeStart); c.Transfer(LocalDetailDefaultFadeRangeRatio);
 c.Transfer(TestStaticMesh); c.Transfer(TestAnimatedMesh); c.Transfer(TestAnim); c.Transfer(TestGraphic);
 c.Transfer(FOV_2D); c.Transfer(InvalidTextureStandin); c.Transfer(InvalidThemeStandin);
}