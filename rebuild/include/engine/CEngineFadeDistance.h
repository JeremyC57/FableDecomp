// Retail opt-in (Codex 2026-11-10): complete original types from both PDBs and native field/virtual offsets; see 00b396e0 NAMING_EVIDENCE.md.
#ifndef FABLE_ENGINE_CENGINEFADEDISTANCE_H
#define FABLE_ENGINE_CENGINEFADEDISTANCE_H
class CEngineFadeDistance {
public:
#ifdef FABLE_RETAIL_CENGINEFADEDISTANCE_INLINE_CTOR
 // Opt-in (wf1 row 0x00BFA920 CLocalDetailPrimitiveMesh::AddObjectsToPrimitiveRenderer): the two-float constructor is inline in
 // retail (0x00BFA939: the two distances stored straight into the local, no call).
 CEngineFadeDistance() { }
 CEngineFadeDistance(float fadeBeginDistance,float maxDrawDistance):FadeBeginDistance(fadeBeginDistance),MaxDrawDistance(maxDrawDistance) { }
#else
 CEngineFadeDistance();
 CEngineFadeDistance(float,float);
#endif
 float FadeBeginDistance,MaxDrawDistance;
};
#endif
