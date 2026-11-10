// Retail opt-in (Codex 2026-11-10): complete original types from both PDBs and native field/virtual offsets; see 00b396e0 NAMING_EVIDENCE.md.
#ifndef FABLE_ENGINE_CENGINEFADEDISTANCE_H
#define FABLE_ENGINE_CENGINEFADEDISTANCE_H
class CEngineFadeDistance {
public:
 CEngineFadeDistance();
 CEngineFadeDistance(float,float);
 float FadeBeginDistance,MaxDrawDistance;
};
#endif
