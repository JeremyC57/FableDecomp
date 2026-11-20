# NF152 notes (2026-11-20, round 10: PRIORITY pick list CDisplayManager / CEngineResourceManager / CEngineShadowRenderer, 17 entries)

## Counts (APPLIED.tsv / STATUS.tsv / recheck_final.log)
- Entries (17): done 8 (q4561, q4562, q4595, q4596, q4597, q4598, q4729, q5011); done-already 6 (q4199, q4507, q11787, q11791, q11804,
  q11805); rejected 0; blocked 2 (q4444, q4733: one address, `blocked: port (family)`, row 0x00418DCA); deferred 1 (q4563: held by NF149,
  row 0x00BE91E0); open 0.
- Rows (STATUS.tsv, 30): ported 8 (0x00B3B030, 0x00B3B060, 0x00B3B0A0, 0x00B3B1F0, 0x00B3B5B0, 0x00B4B890, 0x00B4B8A0, 0x00B4FC10: 3 MATCH +
  5 RELOCATION_MATCH, every test PASS, one recheck_final.log run, no HARD / RELOC_CONFLICT / HEADER_POLICY / RELOC_CHECK_ERROR); not-a-binder 21;
  blocked 1 (0x00418DCA, no form graded).
- RENAMES: 8 section-1 self renames (all 8 ported rows), 5 section-2 respellings (0x00B3B030 / 0x00B3B060 x2, 0x00B4FC10 x1). NAME_FIXES: 5.
  selfcheck.log: 0 problems.
- Header opt-ins: 6, add-only, in engine/CEngineResourceManager.h (2) and engine/CEngineShadowRenderer.h (4); hdr/README.md.
- grades_used: the per-row census compiles (work/probe.tsv, probe_b.tsv, probe_ctor.tsv: one compile per REFS row) are not counted in the
  not-a-binder rows' grades_used.

## Order of work
q11787 / q11791 / q11804 / q11805 / q4199 / q4507 were settled together by one binding probe (work/probe.tsv, 114 lines: every REFS row x target; probe_b.tsv and probe_ctor.tsv add 0x006286F0 and the ctor family);
then q4729 + q4561 + q4562 + q4563 (one unit), q4595 / q4596, q4597 / q4598 / q5011, and q4444 / q4733 last.

## Coordinator decisions
1. **0x00B3B0A0 (q4563), separable self port.** rows/00b3b0a0.cpp is finished (MATCH + PASS). Six of the seven REFS rows already bind
   ?GetIndexBufferMemPool@CEngineResourceManager@@QAE?AW4EIndexBufferPool@@XZ. The seventh, 0x00BE91E0, is locked NF149, and NF149's pending
   hand-back keeps a local `long GetIndexBufferPool()` (?GetIndexBufferPool@CEngineResourceManager@@QAEJXZ) with a cast. Landing this row alone
   replaces the self definition AddChildPrimitive (spellings at the address 3 -> 2) and grows none; that is the nf139 separable form. If you
   prefer the literal rule, hold rows/00b3b0a0.cpp until 0x00BE91E0 is respelled (NAME_FIXES has the fix).
2. **q4444 / q4733 blocked with no form graded** (advisor decision). 0x00418DCA binds 0x009C0190 as the stand-in
   ?AddComponent@CComponentList@@QAEXPAUCComponentListEntry@@@Z. The PDB spelling needs `this` as CDeviceResetCallback*, which this row gets
   only through its HEAD-local CGameComponent base list. CGameComponent has engine/CGameComponent.h, so re-basing is banned, and a
   reinterpret_cast through the stand-in base would only hide it. The port onto the headers is a family plus header work (below). The
   split at 0x009C0190 predates this round; 0x009A6610 / 0x00B26360 and the self row are right; 0x0040CEC0 (the other caller) is parked.

## Needs header claim (prerequisites of the 0x00418DCA port; none attempted)
- engine/CFrameRateSmoother.h: no header exists. Landed rows use 0x30 bytes (+0x16178 .. +0x161A8 in CMainGameComponent); Ego_r says 56, so the
  retail layout is unproven. The constructor 0x00419747 is bound as ??0CFrameRateSmoother@@QAE@XZ.
- engine/CDiskFileWin32.h: the default constructor is inline (FABLE_RETAIL_CDISKFILEWIN32_DEFAULTCTOR); this ctor calls the out-of-line
  0x00415C30, so it needs an out-of-line declaration opt-in.
- engine/CMainGameComponent.h: PPlayerManager, PLUA, PDebugFont (typed only under unrelated macros), EventPackageFile and FrameRateSmoother
  are opaque byte arrays; the constructor must construct them.
- engine/CGameComponent.h: CGameComponent(CGame&) at 0x0051D8E0. The self row defines CGameComponent(CGame*); 0x0042EA8F binds that
  (NAME_FIXES).
- Own-leaf family of 0x00418DCA (work/probe_ctor.tsv): 0x00412F90 binds the PAVCGame stand-in; 0x0042EC7C / 0x00496CF0 bind the PDB
  AAVCGame spelling; 0x0058578B binds nothing there. The header port moves the self leaf to the PDB spelling; 0x00412F90 needs respelling
  in the same hand-back.
- Also: vtables 0x0122F180 / 0x0122F174, which 0x00418DCA binds as ??_7CMainGameComponent@@6BCBaseClass@@@ / 6BCComponentListEntry@@@ while
  0x00418B7A binds the real bases (CBaseClassNonCopyable / CDeviceResetCallback).

## Evidence per unit
- q4729 / q4561 / q4562: both PDBs give the run GetVertexBufferUsage 0x009D9680, GetIndexBufferUsage 0x009D96A0, GetIndexBufferMemPool
  0x009D96D0 (Ego_r; FableWin 0x02DD43A0 / 43E0 / 4440). Retail lays them out at 0x00B3B030 / 0x00B3B060 / (GetVertexBufferMemPool
  0x00B3B090) / 0x00B3B0A0, and the landed callers already bind them. The engine/CEngineResourceManager.h retail branch already declares all
  three, so no opt-in was needed. The rows call CDisplayManager::GetVertexShaderVersion through FABLE_RETAIL_CDISPLAYMANAGER (only that
  declaration; _VERTEXSHADERVERSION is not defined, so it is not declared twice).
- **New PDB spelling on data 0x01436E1C.** The rows use ?GPDisplayManager@NEngineInternal@@3PAVCDisplayManager@@A:
  - it is queue land1011cr's claim (both PDBs PUBLIC32 + GDATA32);
  - the landed 0x00B24690 already binds it;
  - the stand-in g_pResourceOwner_01436e1c leaves the table with these two rows; a grep of rebuild/src/compiled and rebuild/tests finds it
    only in these two rows and their HEAD tests. The other stand-ins at 0x01436E1C (g_pDisplayManager_01436e1c
    x2, g_pEngineDisplay, g_buyHost_b86a10) are untouched.
- q4595 / q4596 / q4597 / q4598 / q5011: both PDBs. Each address's only REFS row is a CEngine forwarder that already binds the PDB spelling
  (probe). q5011's signature (bool (long, float)) comes from both PDBs and agrees with the body: a long store to +0x338 and a float store to
  +0x33C (Ego_r UserVideoOptionShadowBufferSize / UserVideoOptionShadowDistanceScale), with al taken from InitialiseTextures and `ret 8`.
  The body alone does not discriminate: HEAD's void (long, long) form matched the same bytes. The float / bool come from the two PDBs.
- **New PDB spelling on callee 0x00B4B830.** 0x00B4FC10 now calls
  CEngineShadowRenderer::OnReleaseDefaultPoolResources qualified, as the virtual override ?...@@UAEXXZ (both PDBs). 0x00B4D210 already binds
  that spelling. The QAEXXZ binder spelling goes away (binders 2 -> 1). The self row 0x00B4B830 still defines the QAEXXZ stand-in
  (NAME_FIXES; lock `released`, not touched).

## Descriptive names
- 0x00B4FC10 parameter names `shadow_buffer_size` / `shadow_distance_scale` are descriptive; there is no Ego_d parameter record.
- No other made-up names.

## Defined symbols (objdump -t, new object vs HEAD object)
- 0x00B3B030 / 0x00B3B060 / 0x00B3B0A0 / 0x00B4B890 / 0x00B4B8A0 / 0x00B4FC10: own leaf only. HEAD defined only its old leaf:
  - SAJXZ statics on 0x00B3B030 / 0x00B3B060;
  - ?AddChildPrimitive@@YIHXZ;
  - ?Get@T / ?Hook@T;
  - ?SetShadowMapSize.
- 0x00B3B1F0: own leaf + ??0CTexture@@QAE@XZ (HEAD: the same inline + ?AllocateTexture).
- 0x00B3B5B0: own leaf + ??1CTexture@@QAE@XZ + ??_GCTexture@@QAEPAXI@Z (HEAD: the same two + ?FreeTexture). These are shared-header inlines (pick-any).

## Tests
All 8 were rewritten on the shared headers, and each now links its row object:
- the zeroed-storage `this` pattern is used throughout;
- recording stand-ins: GetVertexShaderVersion / GPDisplayManager, CTexture::Initialise / Uninitialise, OnReleaseDefaultPoolResources /
  InitialiseTextures;
- 0x00B3B0A0 / 0x00B4B890 / 0x00B4B8A0 were host copies or print-only stubs before;
- PassPatterns are unchanged.

## Files and catalog the coordinator may rename
- Source / test file names follow the old labels:
  - CEngineResourceManager_GetIndexBufferUsage_00b3b030 -> GetVertexBufferUsage;
  - _GetIndexBufferUsage00b3b060_00b3b060;
  - global_AddChildPrimitive_00b3b0a0;
  - CEngineResourceManager_AllocateTexture_00b3b1f0;
  - CEngineResourceManager_FreeTexture_00b3b5b0;
  - CEngine_DeallocateShadowBuffer_00b4b890;
  - gapscan_sub00b4b8a0_00b4b8a0;
  - CEngineShadowRenderer_SetShadowMapSize_00b4fc10.
- Catalog Module: 0x00B3B0A0 `_global`, 0x00B4B890 `CEngine` and 0x00B4B8A0 `_gapscan` should become CEngineResourceManager /
  CEngineShadowRenderer. Catalog names are already right for all 8.
- No EH funclet or other source includes any changed row (grep of rebuild/src and rebuild/tests).

## Locks
- rows.txt: 17 rows.
- Claimed beyond rows.txt (owner was `released`): 0x00405650, 0x0040F0E0, 0x00499D90. All three were probed and found right, and were not
  changed. Please release them.
- None of my rows (the 8 changed rows, 0x00418DCA, the three claimed rows and the binders) is `handed off` / `reserved (acctB` in
  UNMATCHED_ROWS.tsv (grep per address).
- HELD.txt: no intersection with my REFS rows or changed rows.
- Pending hand-backs: NF149's rows/ holds 0x00B620A0, 0x00B6D940, 0x00BBF380, 0x00BC3E10, 0x00BE3660 and 0x00BE91E0 from my REFS lists.
  Each was grepped for the getter spellings: all keep the PDB spellings except 0x00BE91E0 (GetIndexBufferPool, decision 1).
  NF147 / NF148 / NF150 / NF151 / RP1 rows/ have no intersection.

## Flags lines
None. Every changed row grades with its build entry's (empty) CompilerFlags.

## Rule slips
- Entries were not taken strictly in the brief's order. One probe settled the six done-already entries first, and q4444 / q4733 (CDisplayManager
  group) were taken last, after the resource-manager and shadow-renderer units.
- The first probe's `column` call failed (not installed). The Bash call itself exited 0.
- One late selfcheck call ran outside the repo root with `$S` unset; it wrote nothing. It was rerun from the repo root, and selfcheck.log
  shows 0 problems.
