// Retail opt-in (Codex 2026-11-10): complete original types from both PDBs and native field/virtual offsets; see 00b396e0 NAMING_EVIDENCE.md.
#ifndef FABLE_ENGINE_CENGINESCENEGRIDPOS_H
#define FABLE_ENGINE_CENGINESCENEGRIDPOS_H
class CEngineSceneGridPos {
public:
#ifdef FABLE_RETAIL_CENGINESCENEGRIDPOS_INLINE
 // Opt-in (hand3c, row 0x00B93B30 CEngineSceneMap::AddPrimitive): the FableWin.pdb bodies, inline in retail.
 // ??0CEngineSceneGridPos@@QAE@XZ (0x02E1CDA0: `mov byte ptr [eax],0FFh`), ??0CEngineSceneGridPos@@QAE@CEE@Z (0x02E1B520: the three
 // bytes in order) and ?ConvertToLevel@CEngineSceneGridPos@@QBE?AV1@J@Z (0x02E1B340: asserts lev <= Level, then builds
 // (lev, X >> (Level - lev), Y >> (Level - lev)), Y first; Ego_d parameter lev). The assert is compiled out in retail.
 CEngineSceneGridPos() : Level(-1) {}
 CEngineSceneGridPos(signed char level,unsigned char x,unsigned char y) : Level(level),X(x),Y(y) {}
 bool IsValid()const;
 void Invalidate();
 CEngineSceneGridPos ConvertToLevel(long lev)const { return CEngineSceneGridPos((signed char)lev,X>>(Level-lev),Y>>(Level-lev)); }
#else
 CEngineSceneGridPos();
 CEngineSceneGridPos(signed char,unsigned char,unsigned char);
 bool IsValid()const;
 void Invalidate();
 CEngineSceneGridPos ConvertToLevel(long)const;
#endif
 signed char Level;
 unsigned char X,Y;
};
#endif
