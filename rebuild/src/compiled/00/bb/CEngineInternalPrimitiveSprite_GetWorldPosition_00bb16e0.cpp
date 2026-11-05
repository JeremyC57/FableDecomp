// Leaf: ?GetWorldPosition@CEngineInternalPrimitiveSprite@@QAEEPAUVec3@@@Z
struct Vec3 { int a, b, c; };
struct CEngineInternalPrimitiveSprite { char pad[0x48]; Vec3 pos; unsigned char GetWorldPosition(Vec3* out); };
unsigned char CEngineInternalPrimitiveSprite::GetWorldPosition(Vec3* out) { *out = this->pos; return 1; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetWorldPosition@CEngineInternalPrimitiveSprite@@UAE_NAAVC3DVector@@@Z); evidence: RTTI vtable slot + PDB name set.
