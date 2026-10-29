# Hand chunk 569 (CEngineInternalPrimitiveMeshBase .. NScript::CV_AssassinAttacksScript): 3 of 5 byte-exact

## What matched (3)
- **std::__introsort_loop<CActiveScriptPair*, CActiveScriptPair, int, CActiveScriptLess>** 0x00CBD700 (RELOCATION_MATCH 969v969, 1 grade) --
  the STLport 4.6 `_algo.c` introsort loop over CScriptBase::EntityScripts (pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>>,
  CKeyPairCompareLess). Shape: the STLport text verbatim under `#pragma inline_recursion(on)`; VC7.1 expands the recursion five
  levels, only the outer level inlines `__median` and the pivot copy. Name evidence: (1) STLport source structure (threshold 16,
  median-of-three, `__unguarded_partition`, `partial_sort` fallback at depth 0, self call at 0x00CBD9DB); (2) the caller sort<>
  0x00CBDEF0 passes `__lg(n) * 2` and runs the 16-element final insertion sort, called from CScriptBase::OnScriptedEntityActivated
  0x00CB8960 on the Ego_r.pdb member CScriptBase::EntityScripts (element type = 16-byte pair, unsigned 64-bit compare ja/jb high word first).
- **NScript::CQ_WaspBossScript::GuildmasterHelp** 0x00E12B30 (RELOCATION_MATCH 993v993, 2 grades) -- the Wasp Boss quest's guild-master
  guidance thread: waits for the cutscene, gets "QueenHornet", installs a CIsThingAlive predicate, then sends GUIDANCE_10..50 as the
  boss phase reaches 3 / 4 / 6 and its health reaches CScriptDef::WB_LowHealth. Shape: a named `CScriptDef* def = pGlobalScriptDef;`
  re-read at the end of the wait loop keeps it in a callee-saved register across GetHealth (the first grade spilled it: 1003v993).
  Name evidence: Ego_r.pdb / FableWin.pdb ?GuildmasterHelp@CQ_WaspBossScript@NScript@@QAEXXZ; the spawned-function name string
  "GuildmasterHelp" and the address 0x00E12B30 stored at 0x00E12A37 by DoMission 0x00E12580 (already bound with this symbol).
  WB_LowHealth: Ego_r CScriptDef +0xEE4 -> retail +0xE34 (Dinkumware-vector shrink before the member).
- **NScript::CV_AssassinAttacksScript::WatchForAssassinDead(CScriptThing)** 0x00E39140 (RELOCATION_MATCH 967v967, 2 grades + compile-only
  probes) -- per-assassin thread: each frame asks the assassin who killed it; on a kill gives the quest card once, records the region
  of the kill, clears that region's assassin flag and, when the hero killed it, bumps the region kill count and gives
  CScriptDef::AssassinExperience. Shape: `~CScriptThing` `__forceinline` with `#pragma inline_depth(0)` around exactly the returns retail
  calls 0x004AA840 at; `for(;;)` with the terminate check at the top and `break` exits; `__forceinline` CBasicString/CCharString
  `operator==` and CScriptThing::GetName; wrappers as `Data == 0 || Data->IsNull()` / `Data != 0 && Data->IsAlive()`.
  Name evidence: Ego_r.pdb / FableWin.pdb ?WatchForAssassinDead@CV_AssassinAttacksScript@NScript@@QAEXVCScriptThing@@@Z; the spawner stores
  0x00E39140 in a CSpawnedFunc1<CV_AssassinAttacksScript, CScriptThing> (vtable 0x012E37BC) named "WatchForAssassinDead" (0x012E37D8)
  at 0x00E39081; `ret 0Ch` = this + by-value CScriptThing. AssassinExperience: Ego_r +0x418 -> retail +0x3E4.
  CScriptThing::NullString = 0x0143E8EC: GetName's null arm returns it, and its atexit sits 0x1E before pGlobalScriptDef's in both builds
  (retail 0x0122AE60 / 0x0122AE7E) as the Ego_r static CScriptThing::NullString does (const-ness unproven; declared non-const).

## Parked for Fable 5.1 (2)
- 00beb6c0 CEngineInternalPrimitiveMeshBase::Calc2DObjectMatrix DIFFER(970v978): x87 add order in the two inlined translation multiplies (`fadd st(1) ; fadd st(2)` vs reversed where the shared x*0.0f products are CSE'd/spilled), the three zero-translation `mov [eax+24h..2Ch],ecx` stores scheduled before the rotation stores instead of after, and the second snapshot's `mov ecx,0Ch ; mov esi,eax ; lea edi` not interleaved into the first multiply's translation row | tried: vector Set with -right temporaries (988), PreTranslate/PostTranslate at the end (955/984), translation matrices first + 12-float Set (938, frame/cross order now exact), zero stores as separate/chained/first/reversed statements, coordinate-descent over all 3- and 4-term sum forms of both multiplies (best kept, 1 grade) | next: a joint search per column (E1j/E2j/E3j share their x*0 products) or a single shared SetMultiply with PreTranslate/PostTranslate implemented through operator*; check FableWin.pdb inline line info for CMatrix3x4::PreTranslate / PostTranslate bodies
- 00c8f200 _vds_shared_init DIFFER(1006v1007): prologue/epilogue and every `v` access (retail keeps v in EBX, `ret 8`, 8-byte frame; ours reloads it from a stack slot) | tried: verbatim Xiph block.c with __stdcall, retail-offset structs, dllimport calloc/malloc, __fastcall callees (1 grade) | next: lane gap: LTCG-private register convention (static `_vds_shared_init` with v in EBX) -- needs a /GL + /LTCG link of the libvorbis objects

Name evidence for the parked rows:
- 0x00BEB6C0: Ego_r.pdb ?Calc2DObjectMatrix@CEngineInternalPrimitiveMeshBase@@SIXAAVCMatrix3x4@@ABVC3DVector@@1ABVCRightHandedSet@@M@Z at
  0x00AA3920, directly before CalcObjectMatrix 0x00AA3C80 (FableWin.pdb 0x02EE3850 before 0x02EE3A00), as retail 0x00BEB6C0 sits directly
  before CalcObjectMatrix 0x00BEBAA0 (unique PDB name, landed callers agree); static `SI` = __fastcall with (matrix, centre) in ecx/edx and
  position, orientation, scale on the stack (`ret 0Ch`); only caller CEngineInternalPrimitive2DAnimatedMesh::CalcInterpolatedObjectTransform
  0x00BCF670 (passes the bounding-sphere centre and the interpolated position/orientation).
- 0x00C8F200: libvorbis block.c `static int _vds_shared_init(vorbis_dsp_state*, vorbis_info*, int encp)` -- the body is the Xiph source
  line for line (calloc of private_state 0xC0, ilog2 0x00C8F060 of blocksizes, mdct_init, drft_init / vorbis_book_init_* on encp,
  halfrate_flag at codec_setup_info +0xE78, floor/residue look()); "Xiph.Org libVorbis I 20030909" @ 0x012BF240; its callers are the
  vorbis_analysis_init / vorbis_synthesis_init pair.

## Skipped
- none.

## Label corrections
- 0x00BEB6C0 Interpolate_Quaternion_Scale_Matrix -> CEngineInternalPrimitiveMeshBase::Calc2DObjectMatrix (no quaternion, no interpolation).
- 0x00C8F200 AudioSystem_Initialize -> _vds_shared_init (libvorbis).
- 0x00CBD700 CItem_QuickSort -> std::__introsort_loop<CActiveScriptPair*, ...> (not CItem; CScriptBase::EntityScripts).
- 0x00E12B30 HandleQueenHornetDialogue (class CScriptGameResourceObjectScriptedThingBase) -> NScript::CQ_WaspBossScript::GuildmasterHelp.
- 0x00E39140 HandleQuestCardAssassinAttacks (class CScriptGameResourceObjectScriptedThingBase) -> NScript::CV_AssassinAttacksScript::WatchForAssassinDead.
- Landed spellings proven wrong are in NAME_FIXES.tsv (0x00BEB6C0, 0x00E39140, 0x00CBD700, 0x00CB9C20 pair copy ctor, 0x00CBB710 __unguarded_partition, 0x00CBCFC0 __partial_sort, 0x0143E8EC
  CScriptThing::NullString, 0x00411570 CBasicString<char>::Compare).

## Header debt
- engine/CScriptThing.h and engine/CGameScriptInterface.h are flattened data layouts with no virtuals; 0x00E12B30 and 0x00E39140 model
  CScriptThing (vtable 0x01238C8C wrappers) and CGameScriptInterfaceBase (slots from ghidra_out/cgsi_slots_aligned.tsv) locally, as their
  landed siblings (DoMission 0x00E12580, FisticuffsChallenge 0x00E86E60) do. The script classes (CQ_WaspBossScript,
  CV_AssassinAttacksScript) are local models at retail offsets (Ego_r.pdb - 0x14).
