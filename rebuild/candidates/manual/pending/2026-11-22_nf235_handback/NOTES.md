# NF235 notes (2026-11-22, round 10): the nf234 follow-ups (q12656 / q12657 done; q12658 blocked with a write-up; q12659 open)

Base: the repo tree with nf234 landed. q-ids: ENTRIES.tsv (q12656 CFrontendGameComponent, q12657 CMeshBankManager, q12658 CEngine, q12659
CMainGameComponent); the BRIEF numbers them one lower (q12655 ... q12658). APPLIED uses the ENTRIES ids.
Locks: NF235 pre-claimed 0x00495620 0x00496070 0x00496CF0 0x00A0A110 0x00A0A150 0x00B260B0 0x00418DCA; claimed new 0x00494370 0x00494350 0x00494360
0x00497720 0x00495FE0 0x004963A0 0x00494900 (census / link only for the last four); claimed 0x00A09D80 from none. At hand-back: NF235 on the 9 changed
rows (rows.txt); 0x00B260B0 0x00418DCA 0x00497720 0x00495FE0 0x004963A0 0x00494900 released. No AUTH24 lock, 0x00B4D370, 0x00BD3070, `$S/w22/auth2*/`,
`$S/w22/c2t*/`, Codex pass 27 row (none of these rows is in codex27/rows_*.tsv) or 0x0042BEC0 touched. Tools: NF234's (re-pathed). Flags lines: none.
Descriptive names added: none (0x013B7D58 moves from the descriptive g_pLastFrontend to the PDB name). EH funclets / includers of the changed
row files: none (grep of the nine file names over rebuild/src and rebuild/tests).

## Entry 1 (q12656): the CFrontendGameComponent family -- done (six rows, first form each)
Vtable names (all three PDB publics + RTTI COL 0x013330F0 / 0x0133309C): ??_7CFrontendGameComponent@@6BCBaseClassNonCopyable@@@ (0x01238768: ??_G
0x00497720, Init 0x00415920, Run 0x00496CF0, ChangeTextureColourDepth 0x00495610, SetQuit 0x00494370) and ??_7CFrontendGameComponent@@6BCDeviceResetCallback@@@
(0x0123875C: OnPreDeviceReset 0x00494350, OnPostDeviceReset 0x00494360). On the header the emitters' vtables grow from 1 slot each to 5 + 2, so every
slot row joins (a slot row whose self leaf differs from the slot's PDB spelling would go 1 -> 2): the family is six rows.
- 0x00495620 (dtor) / 0x00496070 (ctor): ported onto CFrontendGameComponent.h (new CTOR_DTOR layout branch + SETQUIT), CGameComponent.h (DTOR_INLINE in
  the dtor), fable_string.h, fable_containers.h, CTexture.h, CUserProfileManager.h (new DEFAULTPROFILENAME), GFGetTime.h. Both RELOCATION_MATCH on the
  first form; vtcheck: every slot of both class vtables (and CGameComponent's two in the dtor) binds the symbol of the row at retail's slot target.
  The ctor's two expanded helpers are the real private members, defined `inline` in the row as retail's TU-mates: LoadProfile (?LoadProfile@
  CFrontendGameComponent@@AAEXABVCWideString@@_N@Z, PDBs; was the made-up static CUserProfileManager::LoadProfile of the local model) and SetMode
  (the PDB AAEXW4EGameMode spelling, the landed owner 0x00495FE0's body verbatim; was ?SetMode@..@@QAEXJ@Z). Their pick-any copies EQ 0x00494410 /
  0x00495FE0 (SetMode's one unmatched relocation is the L"" literal COMDAT ??_C@_11LOCGONAA@ at 0x0122D70C, no table line).
- 0x00496CF0 Run: self leaf -> ?Run@CFrontendGameComponent@@UAE_NPAPAVCGameComponent@@@Z (PDBs; slot 2) on the headers (CTOR_DTOR, CNewFrontendGameComponent.h
  CTOR_DTOR for its ctor, CMainGameComponent.h, CMainGameComponentInit.h new INLINE_CTOR_DTOR, CGameJoystickManager.h new MAINPLAYERJOYSTICK).
  RELOCATION_MATCH first form; symbol diff = exactly the self leaf, Input / Output QAE -> AAE, the 0x013B7D58 respell. Kept in place: the HEAD
  CSystemManager local model on the partial ESYSTEMRETURN_ONLY mode (the full header needs the Win32 types; REJECTED) and the header-less
  CNewFrontendGameComponentInit shape.
- 0x00494370 / 0x00494350 / 0x00494360: leaf-only ports onto the header (SETQUIT / default path), MATCH, new header tests.
- 0x00497720 (??_G slot 0): unchanged; both emitters' ??_GCFrontendGameComponent@@UAEPAXI@Z pick-any copies EQ it (bytes + relocations). Its own
  stand-in leaf / binder (?CFrontendGameComponent_dtor@@.. for 0x00495620) wait on the TU-emitter rule (as 0x004330D2; NAME_FIXES).

## Entry 2 (q12657): the CMeshBankManager family -- done (three rows, two forms on 0x00A0A150 / 0x00A0A110 each)
Vtable names (PDB publics + RTTI COL 0x01356AB4 / 0x01356A64): ??_7CMeshBankManager@@6BCBaseClassNonCopyable@@@ (0x0129C98C: ??_G 0x00A0A8A0) /
??_7CMeshBankManager@@6BCDeviceResetCallback@@@ (0x0129C980: OnPreDeviceReset 0x00A09D90, OnPostDeviceReset 0x00A09D80); the ctor's inline
CDeviceResetCallback ctor stores ??_7CDeviceResetCallback@@6B@ (0x0122DE50, both slots _purecall).
- 0x00A0A110 (ctor) / 0x00A0A150 (dtor) on CMeshBankManager.h (new CTOR_DTOR) + CCountedPointer.h + fable_containers.h. Form 1 matched on both but
  left new NEQ pick-any copies (REJECTED); form 2: the dtor takes Reset(0) over the Data-first RELEASEDATA body (the dead first Data store folds:
  Info then Data zeroed, retail 0x00A0A193 / 0x00A0A196) with two new size bodies (= owners 0x0042918C / 0x0042FC86) and DELETEDATA_DECL_ONLY; both
  rows take FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE (retail calls clear 0x00A0A720). Every pick-any copy with an owner is EQ (work/pk_00a0a1*_new.tsv).
- 0x00A09D80 OnPostDeviceReset: leaf-only port (MATCH, new header test). 0x00A09D90 unchanged (its self leaf is the PDB one). 0x00A0A8A0 (??_G)
  unchanged: both emitters' ??_GCMeshBankManager copies EQ it.

## Spelling census (vtable slots included; HEAD -> this hand-back; sources: absrefs / callers / fam.sh objects, work/census_*.txt, work/absrefs_*.tsv)
- 0x01238768 2 -> 1 (6BCBase in 0x00496070 goes); 0x0123875C 1 -> 1 (stand-in -> PDB); 0x0122ECA4 2 -> 1 (0x00495620's 6BCGameComponentInterface goes);
  0x0122ECB0 1 -> 1.
- 0x00496CF0 1 -> 1 (QAE -> UAE); 0x00494370 1 -> 1 (QAE -> UAE); 0x00494350 2 -> 1 (?v00@CGameComponentInterface@@UAEXXZ goes); 0x00494360 1 -> 1;
  0x00415920 / 0x00495610 / 0x00415930 1 -> 1 (new slot bindings use the self spellings); 0x00497720 unchanged (??_E slot spelling as at HEAD).
- 0x004963A0 2 -> 1, 0x00494900 2 -> 1 (Run binds AAE); 0x00495FE0 1 -> 1 (the pick-any copy now has the owner's spelling); 0x00494410 1 -> 1.
- 0x013B7D58 3 -> 3 (g_pLastFrontend -> the PDB GOldGameComponentToDelete; the other two spellings are 0x004189C2 / 0x00412F90 + 0x0042EC7C).
- 0x0129C98C 1 -> 1; 0x0129C980 1 -> 1 (stand-in -> PDB); 0x0122DE50 3 -> 2 (only 0x00B260B0's NativeDeviceResetCallback left); 0x00A09D90 3 -> 1 (the
  two ?v00@ stand-ins go); 0x00A09D80 1 -> 1; 0x00A0A720 4 -> 3 (?clear@CBankInfoList goes; 0x00A0A840 / 0x00A0A880 bind CActiveFile stand-ins, NAME_FIXES);
  0x00BFEA08 gains __purecall bindings in place of the ?v00@ stand-ins (?v00@CGameComponentInterface / ?v00@CMeshBankListener were each bound to two addresses).
No address grows.

## Link (work/lnk1, work/lnk2: the new objects + owners 0x00495FE0 0x00497720 0x004131A0 0x004131D0 0x00415940 0x00494410 0x00412130 0x00415920 0x00495610
0x004963A0 0x00494900 0x00495780 0x0051D8E0 / 0x00A0A720 0x00A0A8A0 0x0042918C 0x0042FC86 0x00428AE7 0x0041C780 0x0041C7A0 0x0041C7C0 0x00A09D90)
- Family 1: LNK2005 0; LNK4006 2 at HEAD -> 4: the two new ones are 0x00496070's pick-any LoadProfile / SetMode copies against the owners' strong
  definitions, the pattern HEAD already has (0x004963A0's SetMode copy v 0x00495FE0, 0x00496CF0's CMainGameComponentInit ctor v 0x004131A0); every
  such copy EQ its owner. Decision 1.
- Family 2: 0 duplicates (HEAD 0).

## Entry 3 (q12658): 0x00B260B0 CEngine::CEngine -- blocked (write-up; not a 3-form failure, no form graded)
The row models CEngine as `struct EngineNative : CIEngineNative, NativeDeviceResetCallback` with 83 local override declarations (each binds an
`@EngineNative@@` stand-in for a CEngine slot of 0x012A0F3C) plus header-less member shapes (NativeArray, NativeDefPointer, PreloadStatsNative,
VideoOptionsNative ...). Renaming NativeDeviceResetCallback to CDeviceResetCallback, or re-basing EngineNative on the shared CDeviceResetCallback,
is a stand-in -> real-class rename / a re-base of a local model of a class with a header (nf95b; base-list rule): not allowed. What the port needs:
engine/CEngine.h opt-in with the 83 CEngine overrides in their PDB spellings (two sources each: PDB + vtable slot), the CIEngine.h slot order
checked against 0x012A0F3C's 86 slots, the typed members (CEngineVideoOptions, the preload stats, STLport vectors, CDefPointers, CSimpleMemoryPool),
then the census of the ~86 slot addresses (each loses its EngineNative stand-in: no growth possible). Then 0x0122DE50 2 -> 1 and 0x012A0F30 /
0x012A0F3C 2 -> 1 (table lines witness 0x00B261D0 already carry the PDB names). Too large for this pass's remaining context: queued (decision 3).
The landed dtor 0x00B261D0 (the table witness of ??_7CEngine@@6BCDeviceResetCallback@@@ / @@6BCIEngine@@@) is no header route either: it defines
local CInitBaseClass / CIEngine / CDeviceResetCallback / CEngineVideoOptions / CSimpleMemoryPool / CCamera models of its own (it emits the PDB names
from local classes named as the real ones). A CEngine.h port should take both rows together.

## Entry 4 (q12659): 0x00418DCA CMainGameComponent ctor -- open (not started; the coordinator's order puts it last, after a hand-back-ready 1-3)

## Tests
New header tests: 0x00494370, 0x00494350, 0x00494360, 0x00A09D80. Owed (HEAD host-model tests kept, PASS): 0x00495620, 0x00496070, 0x00496CF0,
0x00A0A110, 0x00A0A150. Layout check (work/size_probe.txt): the CTOR_DTOR branches compile to sizeof 0x1E60 (CFrontendGameComponent) and 0xC with
MeshDataBanks at +8 (CMeshBankManager) under VC7.1.

## Header proof
pin_asserts 7 pinned (CGameJoystickManager.h 4, CMeshBankManager.h 3; old line numbers kept), 0 in the other four headers; assert_names_check 0
mismatches; equiv_check rebuild/include v work/t/include (the six overlaid headers): 0 of 57,124 landed rows differ (sources + tests;
work/equiv.log, work/equiv.tsv empty), so no landed includer changes.
recheck_final.log: the 9 changed rows, all MATCH / RELOCATION_MATCH + PASS (final hdr/ and rows/). WARNs: 0x00496070 RELOC_NAME on 0x004065D0
(the catalog label; NAME_FIXES, decision 4) and 0x00496CF0 SHARED_HEADER_AVAILABLE CSystemManager (the in-place local model; decision 5).
After-census from the final rows (work/census_after.txt), vtcheck of all nine emitted vtables (work/vtcheck_final.txt: 29 slots, every one at
retail's target, 0 DIFF; all NOLINE: vtable slot symbols carry no table line, as at HEAD). The final objects define exactly the symbols of the
graded ones.
Pick-any (work/pk_fin.tsv; HEAD in work/pk_<addr>_head.tsv): 15 EQ, 58 NO-OWNER, 5 NEQ, none new: the four ~CCountedPointer<CGraphicDataBank> /
<const CFontBank> copies (53 v 53) are identical in the HEAD objects of 0x00495620 / 0x00496070; SetMode's copy differs only by the L"" literal
relocation (bytes equal). The list dtor copies of 0x00A0A110 / 0x00A0A150 equal 0x00A0A840 / 0x00A0A880 in bytes (work/listdtor_cmp.tsv).

## Rule slips
- The equivalence run used `timeout 1800` (it is cpp-only, ~15 min); every grade ran under 900 or less.
- 0x00A09D80's lock was claimed after its draft was written (the directory was free; no other owner).
- STATUS / APPLIED were written per family, not after every row.
- 0x00496CF0: two SRC_FAIL compiles (a stray HEAD line in the draft, then the full CSystemManager.h) before the matching form; grades_used 3.

## Decisions for the coordinator
1. 0x00496070: LoadProfile / SetMode defined `inline` in the ctor row (retail TU-mates, owner bodies verbatim, pick-any copies EQ) give two more
   LNK4006 in a combined link with the owners 0x00494410 / 0x00495FE0, the pattern 0x004963A0 / 0x00496CF0 already land. Accept, or wait for the
   one-TU rule?
2. 0x013B7D58: bound as the PDB ?GOldGameComponentToDelete@@3PAVCGameComponent@@A by 0x00496CF0 (3 -> 3). Respell the other three binders
   (0x004189C2, 0x00412F90, 0x0042EC7C; extern lines only) next round to reach 1?
3. 0x00B260B0: queue the CEngine.h override opt-in as its own unit (write-up above)?
4. 0x004065D0: the catalog / self leaf say GetAutoSaveFileName, the body returns L"Default" and both binders bind GetDefaultProfileName (NAME_FIXES).
   Rename the row (self leaf + const, one referencing spelling already)?
5. 0x00496CF0: the HEAD in-place CSystemManager local model stays (full header needs Win32 types). Accept as in place?
6. Header edits to existing lines (default path preprocessed-identical, equiv_check): CMeshBankManager.h `#ifndef ..VIRTUAL_DTOR` -> `#if !defined(..VIRTUAL_DTOR)
   && !defined(..CTOR_DTOR)` (same line), the CCountedPointer.h size-block `#if defined(...)` line gains two terms, seven FABLE_STATIC_ASSERTs pinned to
   _AT form (CGameJoystickManager.h 4, CMeshBankManager.h 3). Accept as add-only?
