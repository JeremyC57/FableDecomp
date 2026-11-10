// Retail opt-in (Codex 2026-11-10): complete original types from both PDBs and native field/virtual offsets; see 00b396e0 NAMING_EVIDENCE.md.
#ifndef FABLE_ENGINE_CENGINESCENEGRIDPOS_H
#define FABLE_ENGINE_CENGINESCENEGRIDPOS_H
class CEngineSceneGridPos {
public:
 CEngineSceneGridPos();
 CEngineSceneGridPos(signed char,unsigned char,unsigned char);
 bool IsValid()const;
 void Invalidate();
 CEngineSceneGridPos ConvertToLevel(long)const;
 signed char Level;
 unsigned char X,Y;
};
#endif
