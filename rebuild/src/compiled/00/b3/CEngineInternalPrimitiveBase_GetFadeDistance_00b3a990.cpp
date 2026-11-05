// Leaf: ?CEngineInternalPrimitiveBase_GetFadeDistance@@YIPAUPair@@PAUFD@@HPAU1@@Z
struct Pair { int a; int b; };
struct FD { char pad[0x14]; int f14; int f18; };
Pair* __fastcall CEngineInternalPrimitiveBase_GetFadeDistance(FD* self, int _edx, Pair* out){ out->a=self->f14; out->b=self->f18; return out; }
// Stand-in (owner rule 2026-11-05): CEngineInternalPrimitiveBase_GetFadeDistance for CEngineInternalPrimitiveBase::GetFadeDistance; RTTI vtable slot ?GetFadeDistance@CEngineInternalPrimitiveBase@@UBE?AVCEngineFadeDistance@@XZ + PDB name set (egor/fablewin).
