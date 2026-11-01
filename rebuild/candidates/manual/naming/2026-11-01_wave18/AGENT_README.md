# Naming pass wave 18 (2026-11-01): 1,486 landed rows respelled to proven names, all byte-identical

Inputs: brief `naming/briefs/BRIEF_W18.md` (+ W17 / W16), the wave 17 README / AGENT_README / DEFERRED / scripts, `ESTABLISHED_SPELLINGS.tsv`
(re-copied after the coordinator's 0x0129BA3C `?DELTA_NOTIONAL_ZERO@@3MB` decision), QUEUE_W18_input.tsv (301 rows at the end: 520 1,
c678 38, c680 19, c681 31, c682 18, c683 13, cg10 181) and DEFERRED_W17_INPUT.tsv (397 rows).

`scripts/compose18.py ov_final` (with `compose18_steps.py`) re-runs every stage, in order, on fresh copies of the current repo. hand18/ holds
the per-family hand stages; fam_glob18 / fam_bitset18 / fam_eps18 / fam_gren18 / fam_q18 are the drivers (`AGENT_README.md` has the commands).
ov_final has 1,518 files: 1,486 sources plus 32 behaviour tests that defined a renamed global / static (rfail 4, own 1, pclass 1, pgui 17,
thingmgr 7, eps 1, gpdm 1).

**Recheck** against HEAD (commits up to d6647cf7d; the 35 rows chunks 680/682/683 landed during the wave are indexed in `idx/delta.jsonl` and
merged into `idx/full.jsonl`) with vland --recheck-landed through scripts/vrun.py (`logs/recheck_final.log`, `runs/final.grades.tsv`,
`runs/final.pairs.jsonl`): **1,486 / 1,486 PASS (1 MATCH, 1,485 RELOCATION_MATCH).** No byte moved.
- 1,477 rows come from the full recheck of ov_final (`runs/rk_final.log`); 9 rows whose tests then needed the singleton / static model
  (7 CThing accessors, 0x0076C830, and 0x007E5230 with its pgui edit reverted) were fixed and rechecked from ov_final (`runs/rk_t22.log`,
  `runs/rk_t23.log`); the two stl rows 0x004E34B2 / 0x004DD782 failed (DIFFER 30v35 / LINK_FAIL) and are reverted + DEFERRED.
- The 12 ConstantFPS rows that DIFFER are reverted and DEFERRED (see the decision below).

**BINDINGS.tsv:** 153 RETIRE / 12 ADD / 39 KEEP. `runs/reloc_bindings.after.tsv` = the current repo table minus the RETIREs plus the ADDs;
`bindings_multi_check.py` prints 0 (`logs/bindings_multi_check.log`). Index: `idx/full.jsonl` = HEAD (+ delta), `idx/after.jsonl` = HEAD + ov_final.

**LABELS.tsv:** 23 labels (bare catalog spelling). **RENAMES.tsv:** 13 rows, repo-rooted paths, bare new_name. Apply both after the content commits.

## Families, in commit order (`runs/<fam>.rows.txt`, cumulative content in `ov_<fam>/`)
- **rfail 4** (wave 17 [recheck-fail]): 0x00401F30 `?GFGetBuildNumber2@@YAXXZ` with a polymorphic CDiskFileWin32 model whose qualified
  destructor call keeps Close() direct (test rewritten for the vtable); 0x006A2520 `?SetAllCarriedThingsAsBypassFilter@CTCCarrying@@QAEXPAX@Z`;
  0x009E1D00 / 0x009E1D50 `?GFRoundVX/VYToNearestPixel@@YIMN@Z` on CDisplayManager::GetRenderTargetDimensions (tests' stand-ins fixed).
- **qctor 10** (c678 / c681, re-authored byte-identical): the seven quest-script ctors on `(CScriptDataBase*, CGameScriptInterfaceBase&)` --
  0x00E7EF70 (nested `NScript::CV_FishingCompetitionScript::CFishCompOrganiser`, `(CV_FishingCompetitionScript&, CScriptDataBase*, const CScriptThing&)`),
  0x00E1DB00, 0x00E755F0, 0x00D9E210, 0x00CF88C0, 0x00DE0320, 0x00E398F0 -- and the Allocs 0x00CF91C0 / 0x00DF1080
  (`?Alloc@<C>@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z`), 0x00DEC7F0.
- **own 2**: 0x00994B40 `?GetLength@CMemoryDataOutputStream@@QBEKXZ` (new source + test, MATCH; file rename); 0x0074EE90 reads
  `CMainGameComponent::Get()` (the established protected static PMainGameComponent).
- **stl 7**: real STLport 4.6 instantiations replace the placeholder classes: map<CCharString,bool>::_M_erase 0x007ACC50,
  map<CCharString,unsigned long> 0x00579435, map<pair<long,long>,CEngineTextureMorph> 0x00B2EF20 (with vector<CCompositeBlendDesc>),
  _List_base<CCountedPointer<CCompositeSurfaceCache>>::clear 0x00B2EA90, ~_List_base<CDeviceResetCallback*> 0x009C2220,
  vector<CTVertexWaterBackground>::resize 0x00B7FD00, and `??_G?$pair@JVCCharString@@@std@@` 0x004DA68A (the chain of the deferred ~vector).
- **pmgc 320 / pclass 62 / dstgame 184 / pgui 155 / thingmgr 65 / cfps 291** (fam_glob18.py): the class-static globals are reached the way the
  PDB shows retail reached them -- through the inline static accessors: `?Get@CMainGameComponent@@SIPAV1@XZ` for the protected
  `?PMainGameComponent@CMainGameComponent@@1PAV1@A`; `?Get@?$CTBaseSingletonNoDefaultAlloc@V<T>@@@@SIAAV<T>@@XZ` (operator* of the
  CCountedPointer) for the private `0V` PClass of CQuestManager and CPlayerGui; `?GetPCurrentThingManager@CThing@@SIPAVCThingManager@@XZ`
  for the private `?PCurrentThingManager@CThing@@0PAVCThingManager@@A`; `?GetConstantFPS@CMainGameComponent@@SIJXZ` for the protected
  `?ConstantFPS@CMainGameComponent@@1JA`; and `?pDSTGame@@3PAVCGameScriptInterfaceBase@@A` directly. Row-local stand-in structs are renamed to
  the real class (`class X { public:` so the mangling carries V). The inline accessors are emitted as COMDATs beside the leaf (like the
  engine/CCountedPointer.h members): no binding. The 25 behaviour tests that defined the old globals define the statics through the same models.
- **eps 146** (coordinator update): `extern const float DELTA_NOTIONAL_ZERO;` (`?DELTA_NOTIONAL_ZERO@@3MB`) in every TU that spelled EPSILON,
  plus the 0x008754A0 test. 0x00751E80 (two extern lines) is left.
- **bitset 229**: the interface-presence stand-ins (CPresenceBits / CTCBitsRemaining / InterfaceBits / ...) are `std::bitset<274>` (STLport 4.6
  <bitset>; an empty stand-in keeps its empty layout, a 9-word one the `_M_w` words) and their Test / test is `?test@?$bitset@$0BBC@@std@@QBE_NI@Z`.
- **gpdm 35 / isqrt 36**: `?GPDisplayManager@@3PAVCDisplayManager@@A` (was PDisplayManager) and `?GFastOneOverSqrtTable@@3PAKA`
  (was g_InvSqrtMantissaTable), same types, plus the 0x004022B0 test.
- **queue 101**: fam_q18 (fam_q17 + free stand-ins of static members) over the 171 function targets of the queue; the largest are
  CTCDParticleEmitter::Create 0x006E0880 (62), CThingObject::Create 0x00703210 (13), CCodaContext::SetError 0x00C3D090 (14).

## Queue outcomes (QUEUE_W18.tsv, 301 rows)
- 18 respelled, 32 respelled-partial, 188 deferred, 63 already-correct. Each row carries rows_edited, rows_left and detail (the notes are in
  scripts/mkq18_notes.py; the fam_q18 skip categories otherwise). 0x00440730 (HOLD) was not touched.
- Most cg10 rows are "deferred" because their remaining callers use per-row stand-in views (free-function / table / vtable models) that
  the generic driver cannot move: the mega families bitset::test and ConstantFPS are done; CBasicString::Compare, GetPItem, ~CCharString,
  GetSubDef, GetInstance, Transfer<>, AddEvent were not reached.

## Deferred (DEFERRED.tsv, 437 rows)
- The queue rows that are deferred or partial (221), 213 wave 17 rows carried (11 wave 17 rows resolved here), and 3 [recheck-fail] rows:
  the 12 ConstantFPS rows, 0x004E34B2 + 0x004DD782, and the pgui edit of 0x007E5230.

## Items needing a coordinator / owner decision
1. **0x01375550 access.** The catalog / cg10 / c654 spelling is the protected `?ConstantFPS@CMainGameComponent@@1JA` with the inline getter
   `?GetConstantFPS@CMainGameComponent@@SIJXZ` (c654: "read only through GetConstantFPS"). 291 rows hold through the getter, but 12 non-member
   rows read 0x01375550 as an integer-FPU memory operand (`fidiv dword ptr [01375550]`): VC7.1 copies the inlined by-value getter to a stack temp
   (DIFFER 64v51 at 0x00697710, verified), while a direct public read reproduces the bytes exactly. Retail therefore read the static directly from
   CGameCameraManager, CTCHeroStats, CThingCreatureBase, CTCWife, CTCHeroExperience, CTCPhysicsRigidBody, CCreatureAction_IceTrollIcicleAttack,
   CCombatSequence_BattleCharge and CAIStateGroup_JackDragon / _BuyFromSeller: either the member is public (`2JA`) or those classes are friends.
   The 12 rows are DEFERRED; the c682 row's `?g_framesPerSecond@@3JA` descriptive spelling was not applied (the cg10 member spelling was).
2. **The c680 rows without a mangled final**: 0x00C73660 DetectWindowsVersion (its caller needs a signature re-author), 0x009C4F20
   CFileInstaller::ExitThread volatile (member-type change), 0x00C25860 Cross (probable), 0x0063A300 GetThemeSizeZAt (which of two),
   0x0143BB58 / 0x0143BB48 CODA globals (unverified SDK names), 0x01238C8C the CScriptThing vtable (per-row class models) -- all deferred with notes.
3. 0x00B7FFC0 (00b7ffc0 own row) is already std::__false_type; the per-row CArray binder of 0x00BE4F90 stays (already-correct).
4. 0x00C28690 (libpk) stays deferred; 0x00440730 HOLD.

Not reached (next wave): the scope-1 leftovers PeekThingSearchTools 0x008FD000 (31 rows), PeekTypeList 0x0049C780 / 0x00947780, HasTC
0x006A80A0, the 13 GetPWorldSeed stand-in rows, the STL stand-in callers (00ad77f0, 00ad62b0, 00ad7e80, 00ad6ae0, 00a385a0, 00ad3940),
00caf680 (DirectShow); scope 3 destructor families (0099a300 55 rows / 0099a2e0 / 0099a410 / 009fc550 / 00aa75c0 / 00f35b40 / 00cbd510 30);
the mega families after bitset / ConstantFPS.
