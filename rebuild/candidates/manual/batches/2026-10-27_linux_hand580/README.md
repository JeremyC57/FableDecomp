# Chunk 580 (2026-10-29): 9 of 10 released pass 9 rows landed (quest-script range + 0x00F3xxxx engine rows)

All RELOCATION_MATCH + PASS (g++ and VC7.1). Engine rows: vector<CGPUScratchBufferTextureManager::CResourceEntry>::
_M_insert_overflow / _M_fill_insert, vector<CFireHeartPatternDef>::_M_insert_overflow, NFractal::CRidgedMultifractal::
GetFractalHeight, CRangeCompressor::Compress (public overload). Script rows: CV_AssassinAttacksScript::CAssassinChest::Main,
CQ_DragonBossFightScript::PlayCutscene, CV_TalentlessBardScript::Main, CPersonalScript_EmptyScript::CStealthGuard::Main.
Parked: 0x00F3D200 CTriangleStripifier2::CheckForReshadedVerts (name probable; DIFFER 285v284, register allocation only:
retail keeps the two local array pointers in ebx/ebp and spills counter/index; next try QFE-4035 or a CArray resize shape),
in ../../pending/2026-10-27_linux_hand580/differ/.
Descriptive globals: 0x0143E980 g_RangeCompressBytesIn, 0x0143E97C g_RangeCompressBytesOut (no PDB data symbol).
NAME_FIXES.tsv: 21 rows queued. Retail models: def classes on FABLE_RETAIL_DEF_CLASS_BASE, CBaseClass from engine/CBaseClass.h;
0x00F36710's CRidgedMultifractal has a real virtual whose vtable retail discarded (no address to bind). Script-class header debt
(CScriptThing, CScriptBase, CEntityScriptBase, the interface, predicates, CEntityScriptBinding, CScriptGameResourceObject*,
CBaseObject, CLevelScriptBase) is listed in NOTES.md pending the migration headers. Check pGlobalScriptDef's declared type
(PDB dynamic initializer) when those land.
