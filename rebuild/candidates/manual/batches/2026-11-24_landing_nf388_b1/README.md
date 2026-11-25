# Landing nf388 b1: 0x00B1AE00 is CPSCLight::GetClassTypeName and returns "CPSCLight" (NF385 R2, the literal audit's first fix)

0x00B1AE00 carried 0x00AA7100's self, `C3DMeshFileXBoundingSphereChunk::GetChunkDescription`, and returned its literal "Bounding Sphere". Retail pushes 0x0129F5E8 "CPSCLight". The self is now `?GetClassTypeName@CPSCLight@NParticleEngine@@UBE?AVCCharString@@XZ`: both PDB name sets (FableWin 0x02F52220, Ego_r 0x00ADC280), the catalog label, and the retail literal; no other address carries it. HEAD's duplicate of 0x00AA7100's symbol is gone (/FORCE duplicates 1 -> 0, NF388).
- The new test links the row object and asserts "CPSCLight". It fails against the HEAD text (NF388 mutation check).
- Coordinator checks:
  - the patched grader (7f84efd57a) warns on HEAD (`LITERAL_DIFF +8 'Bounding Sphere', retail 0x0129F5E8 'CPSCLight'`) and not on 0x00AA7100;
  - the landing log shows RELOCATION_MATCH on the new self with no LITERAL_DIFF;
  - self = `// Leaf:` = `self_symbols.tsv`;
  - baselines: bindings multi 1, funclets 0 broken, S units 3 OK.
- T3 nf385 0x00B1AE00 is closed. The port onto engine/CPSCLight.h stays NF385 q8.
