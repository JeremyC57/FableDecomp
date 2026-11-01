# Chunk 683 (NA026, gap-scan rows + two 513-1024 rows) -- 8 of 10 landed (2026-11-01)

Eight RELOCATION_MATCH rows landed (`verify.log`; bindings multi-check 0). The gap-scan bodies were identified first, none a library copy:
0x0121E480 and 0x0121F2D0 are the dynamic initializers of the PDB statics `COpinionReactionManager::AttitudeStrings` and
`CTCFireheartMinigame::BlockNames[4][4]` (.CRT$XCU slots, PDB donors, users); 0x00CEFAC0 / 0x00EFFE10 / 0x00D50800 are the Main of
CGameflowAssistanceScript / COracleMinigameTestScript / CQ_GuildTrainingDepartureScript (vtable slot 2) and 0x00ED83C0 CV_TalentlessBardScript::Init
(slot 3; retail vector<bool> is the 12-byte byte vector); 0x00C6D150 is slot 4 of the libpk delay-effect vtable 0x012BED7C (descriptive
CVtable_012bed7c::Update, same shape as the landed siblings). CDiskFileWin32::Open 0x0099A6A0 (640 bytes, PDB name) landed at probe 6.
Labels: `ghidra_out/labels_chunk683_2026-10-27.tsv` (incl. the parked GetReflectionTexture's PDB class).

Decisions at landing (coordinator, under the owner delegation):
- 0x00C400D0 (libpk direction-filter lookup, no PDB name): one descriptive name `LookupDirectionFilterPair_00c400d0` for all three callers; the two
  landed Update siblings' `Animation_ComputeFrameOffset` is wrong -> queued c683 for naming wave 18.
- differ/00c6eef0.cpp carries two same-TU `__asm` kernels copied from the landed retail_asm bodies 0x00C6ED70 / 0x00C6ECD0 (the ramp kernel's
  private initial-in-ecx convention only arises as a static same-TU callee). It is a parked differ, not a candidate; any retry must reach the
  kernels through the landed retail_asm rows (a static same-TU copy with the same `// Retail-asm:` evidence lines, never new asm), and the
  coordinator reviews purity before landing.
- `_$E2` RELOC_CONFLICT on the two initializers: the per-TU static-destructor thunk (as 0x01219F00) -- expected.
- Header debt (engine/CDiskFileWin32.h flattens Pathname/AccessType; CVtable_012bed7c +0x1E4 float m_gains[6] / +0x1FC long m_delays[6]; landed
  OnPersist 0x00ED85A0 models SongThingAvailable as a 0x14 bit vector where retail has a 0xC byte vector) -> migration wave 10 / c683 queue.
- 0x00BEBCA0 PreloadReflectionTexture (parked, chunk 675): the double (end-begin)/0x2C size computation is solved by an STLport-style vector member
  (start/finish/end_of_storage) in differ/00ba9830.cpp -> retry it in the next pairs chunk with that model.

Parked: CVtable_012b8a8c::Process 0x00C6EEF0 (DIFFER 511v507, 24 probes, /GS: only the current/previous record swap), CEnginePrimitiveManagerMeshBase::
GetReflectionTexture 0x00BA9830 (570v570, 25 probes: `push ebx` sunk past the lookup diamond, the GOTCHAS-known lane gap). Best sources in
`../../pending/2026-10-27_linux_hand683/differ/`. NAME_FIXES.tsv: 12 rows (queued c683). Lessons in LESSONS.md.
