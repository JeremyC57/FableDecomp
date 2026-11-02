# Chunk 715 (NA058) -- 10 of 10 byte-exact (RELOCATION_MATCH), two opt-in branches (2026-11-02)

Rows (`verify.log`: WINS 10, all PASS; seven of ten labels wrong): ten destructors, each named from its stored vtable / slot-0 ??_G and both
PDB name sets, and each TU's emitted ??_G also matches the retail ??_G: ~CTCCoopSpirit 0x00670090, ~CTCDrainLife 0x007C31C0,
~CTCCreatureModeManager 0x0073AEA0 (RemoveAllModes inlined), the private ~CVertexShaderData 0x00A62FF0 (`EAE`), NCreatureMode::
~CModeIceTrollGroundSmashIcicleAssault 0x00743FE0 (tail to the global ~CModeStandBase), ~CGameScriptThing 0x008D4740, ~CXMVPlayer 0x009BDB40
(tail to ~CBaseClassNonCopyable 0x0099A300), CPhysicsMeshInfo::~CMeshData 0x00A2B630 (an implicit dtor needs the destroy-by-name harness or
VC7.1 inlines it into the ??_G), ~CTextureRenderer 0x00A3B970 and NScript::~CV_ArcheryCompetitionScript 0x00E37680 (chunk 713's
FABLE_RETAIL_CLEVELSCRIPTBASE branch).

Coordinator decision (0x00A3B970): both PDBs spell `??1CTextureRenderer@@QAE@XZ`, but there CTextureRenderer was a 28-byte class with no base;
retail's derives from CBaseVideoRenderer (RTTI; `new` pushes 0x180), so its dtor is virtual and VC7.1 can only emit `UAE`. Landed as UAE
(the retail symbol is what the retail class forces); the landed ??_G 0x00A3BDC0 becomes `??_GCTextureRenderer@@UAEPAXI@Z`.

Headers (hdr/README.md): opt-in FABLE_RETAIL_CGAMESCRIPTTHING (engine/CGameScriptThing.h: polymorphic, CScriptThing-derived, sizeof 0x48,
private PDB ctors) and FABLE_RETAIL_CTEXTURE (engine/CTexture.h: 8 bytes, inline `~CTexture() { Uninitialise(); }`, copy ctor, operator=).
Default text of each unchanged under `cpp`; only this chunk's rows define the macros, so no includer recheck was needed. Each overlay was
proved load-bearing by perturbation.

Second spellings at landing (none simple; queued PRIORITY, NAME_FIXES_QUEUE.tsv c715): the STLport list clear 0x009BE060 bound as
CSubtitleList / CRetailList stand-ins by 0x00A3BC20 / 0x00A3BDE0; the eight landed ??_G rows bind stand-in dtor names; the archery ctor
0x00E32350 and vtable 0x012E2EE4 lack `NScript`; vtable 0x0129CD78 carries a second (unnested) spelling; chunk 710's CAnimFilterBase slot order
and its swapped labels for 0x0073E970 / 0x0073E9A0. The agent's 19 NAME_FIXES rows are queued as c715; labels in
ghidra_out/labels_chunk715_2026-10-27.tsv.
