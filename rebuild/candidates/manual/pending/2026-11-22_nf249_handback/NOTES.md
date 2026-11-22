# NF249 notes (2026-11-22, round 17b): UnsheatheWeapon to protected, 0x00890D80, the CEngine ctor / dtor (step 3); 0x00418DCA not started

Base: the repo tree with nf247 / nf244 landed. NF248 runs beside me (its rows.txt / QUEUE_LINES addresses checked: no overlap); none of my
rows is in `pending/2026-11-22_codex_pass28/RESERVED.tsv`. Locks: work/locks.log (0x006648D0 / 0x006649C0 new directories, the rest were
`released`). Tools: NF247's set re-pathed (tools/), plus mp.py (payload + oracle from rows/), incl.py (includer list of a header from
today's tree, transitive through rebuild/include), symdiff2.sh. Read-first list: the brief chain NF247 -> NF215, NF214 and its read-first
list (NF92 / NF88-90 LESSONS, NF44 sections, EXTRA_RULES 2026-11-05 / 2026-11-20 / 2026-11-21 sections, ASM_RULES headings; GOTCHAS by grep only).
Flags lines added: none. Descriptive names added: none (every new spelling is a PDB name, or the compiler's `__real@0000000000000000`).

## Entry 1 (q12690): UnsheatheWeapon QAE -> IAE -- done (4 rows + the header opt-in)
- Evidence: ?UnsheatheWeapon@CThingCreatureBase@@IAEXJ_N@Z in Ego_r.pdb (0x00540710; ghidra_out/bsim_port_audit_final.tsv pairs it with retail
  0x006648D0), FableWin.pdb (0x01D94EA0) and Ego_d.pdb; no PDB has QAE. Census (callers.py): the only E8 callers are 0x006649C0 (x2),
  0x00664A10, 0x00664A50; no absolute reference (absrefs.py); table line `?UnsheatheWeapon@...@@QAEXJ_N@Z 006648d0 006649c0`.
- Change: in-place (each row already modelled CThingCreatureBase locally at HEAD): the self row's local declaration under `protected:` and
  its Leaf line; 0x006649C0 / 0x00664A50 declare it under `protected:` / `public:` in their HEAD local models (in-place respell: same class
  token, no new symbol); 0x00664A10 through the NF247 opt-in, which now reads `protected: void UnsheatheWeapon(...); public:` on the same line.
- Proof: symbol diffs (work/sd1.txt): 0x006648D0 DEF QAE -> IAE only; each binder UND QAE -> IAE only. Link (4 objects, HEAD and new): 0
  duplicates. vland: 4 / 4 RELOCATION_MATCH + PASS. 0x006648D0: 1 -> 1 spelling (now the PDB one). Header: hdr/README.md.
- Tests unchanged: all four are standalone host models (0x006648D0's calls UnsheatheWeapon from main on its own model: HEAD debt).

## Entry 2 (q12691): 0x00890D80 EntityUnsheatheRangedWeapon -- done (header port, first form)
- HEAD: `#define CThingCreatureBase CThingCreatureBaseLayout` around the header, local CThingCreatureBase / CThingNative (member-pointer slot-77
  call), local ScriptMethods (79 slots) / CBaseClassNative / CCPPointerInfo / CountedScriptImp models of CScriptThing, the CGameEvent rename,
  local CCombatManager / CMainGameComponent, the stand-in CGameScriptInterfaceMethods (self leaf `?EntityUnsheatheRangedWeapon@CGameScriptInterfaceMethods@@QBEXABUScriptMethods@@_N@Z`, no Leaf line).
- Now (rows/00890d80.cpp): CGameScriptInterface.h / CScriptThing.h (NSCRIPT), CThingCreatureBase.h retail (ORIGINAL_VTABLE: slot 77
  IsPlayerCharacter; FINISHACTION; UNSHEATHERANGEDWEAPON), CTCCarrying.h PLAYERHIT_PASS26, CWorld.h + COMBATMANAGER (inline GetCombatManager),
  CCombatManager.h UNSHEATH, CGameEvent.h INLINE (ctor(long), SetPlayer, AddData), EGameEventType.h (GAME_EVENT_CONTROLLED_CREATURE_UNSHEATHE_RANGED_WEAPON
  = 34), CMainGameComponent.h ADDGAMEEVENT (Get()). /GS from the catalog. First grade RELOCATION_MATCH; vland PASS with no WARN.
- Bindings (work/symdiff_00890d80.txt, HEAD at /GS): self -> ?EntityUnsheatheRangedWeapon@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z (Ego_r +
  FableWin; header slot 509); 0x006A1620 -> ?IsThingCarryingRangedWeapon@CTCCarrying@@SI_NABVCThing@@@Z; 0x00664A10 -> ?UnsheatheRangedWeapon@CThingCreatureBase@@QAEXXZ;
  0x006EBDD0 -> ?CreatureUnsheathWeapon@CCombatManager@@QAEXAAVCThingCreatureBase@@W4EUnsheatheWeaponType@@@Z (Ego_r + FableWin); 0x013B86A0 ->
  ?PMainGameComponent@CMainGameComponent@@1PAV1@A.
- Census: 0x006A1620 3 -> 2 (PDB + 0x00890250's `?EntityWieldsRanged@@YI_NPAUEntity@@@Z`, authoring); 0x00664A10 2 -> 1; 0x013B86A0 loses
  `?g_MainGameComponent@@3PAVCMainGameComponent@@A` (no other landed row emits it; 0x00890C80 / 0x008A0A20 mention it in comments only);
  0x006EBDD0 count-neutral (0x00890C80 keeps `QAEXPAUCThingNative@@J@Z`; the PDB spelling was already bound by 0x00979F70); 0x00890D80 1 -> 1
  (its only reference is the unlanded CGameScriptInterface vtable 0x01260F0C slot 509, .rdata 0x01261700).
- New pick-any copies: ??0CGameEvent@@QAE@J@Z EQ owner 0x004159B0 (pk.py); Get / GetCombatManager / GetPlayerNumber / GetTypeFlags / SetPlayer
  have no owner. Link (0x00890D80 + 0x004159B0 0x0041649C 0x006A1620 0x006EBDD0 0x00662650 0x00664A10): HEAD 0, new 1 LNK4006 =
  ??0CGameEvent against the strong owner 0x004159B0: the known HEAD debt of every CGAMEEVENT_INLINE row (landing 2026-11-20 hand3c3_b10:
  "make the owners pick-any", queued there).
- Test unchanged (a 919-line standalone host model: HEAD debt).

## Entry 3 (q12692): the CEngine ctor / dtor, step 3 -- done (the header, then both rows; each graded first or second time)
- Claims checked: retail vtable 0x012A0F3C slot 1 = 0x0099A340 (tools/rvt.py); Ego_r / FableWin / Ego_d have ?Validate@CInitBaseClass@@UBEXXZ and
  no ?Validate@CIEngine; CEngine members and CEngine::CPreloadStats from Ego_r.pdb records, CPreloadStats / CEngineVideoOptions / CIEngine
  ctors from FableWin.pdb + Ego_d.pdb (??0CIEngine@@IAE@XZ protected); the retail ctor / dtor bodies give the member order, the inline
  ~CIEngine, the inline ~CSimpleMemoryPool and the 0.0 double constant.
- Header: five add-only opt-ins (hdr/README.md); equiv_check over 151 includers 0 differing; pin_asserts 0; assert_names_check 0.
- 0x00B260B0 (ctor; rows/00b260b0.cpp): `CEngine::CEngine() : CurrentCamera(0), FrameMemoryPool(0, 4096) { IsInitialised = false; }` (the HEAD
  body's two statements on the typed members). Form 1 SRC_FAIL (C2036: CArray<CCamera> with an incomplete CCamera; CCamera.h added to the typed
  block), form 2 RELOCATION_MATCH. Self leaf `??0EngineNative@@QAE@XZ` (HEAD) -> ??0CEngine@@QAE@XZ (binder 0x00B26340 already binds it: 2 -> 1).
  Data bindings: 0x0122DE50 ??_7NativeDeviceResetCallback -> ??_7CDeviceResetCallback@@6B@ (2 -> 1), 0x012A0F3C / 0x012A0F30 ??_7EngineNative
  -> ??_7CEngine@@6BCIEngine@@@ / ??_7CEngine@@6BCDeviceResetCallback@@@ (2 -> 1 each), 0x0122ED70 ?InitialPreloadTime@@3NB -> __real@0000000000000000
  (3 -> 3: the compiler's constant replaces one stand-in; decision 4).
- 0x00B261D0 (dtor; rows/00b261d0.cpp): `CEngine::~CEngine() { Uninitialise(); }`: the typed members' destructors give the rest. First form
  RELOCATION_MATCH. The body's own relocations equal HEAD's (work/e3/xh.txt = xn.txt); only its vtables change (HEAD emitted 1- and 2-slot
  stand-in tables under the real ??_7CEngine names).
- Vtables: both rows now emit the same two tables (work/e3/vt_c2.txt = vt_d1.txt: 86 + 2 slots). Slot census against retail 0x012A0F3C /
  0x012A0F30 and today's Leaf lines (work/e3/slots.tsv): 46 SAME, 34 targets with no mangled Leaf line, 8 DIFF: slot 0 ??_E (compiler; HEAD
  ctor ??_E too, the self leaf is ??_G), slots 14 / 85 (bare Leaf lines `SetVideoOptions` / `Uninitialise`), slots 11 / 18 / 39 / 49 / 50
  (self rows on `CEngine_Methods` / `CIEngine_Methods` stand-ins: 2 spellings before (self + EngineNative) and after (self + PDB);
  NAME_FIXES). OnPreDeviceReset / OnPostDeviceReset (0x00B26010 / 0x00B25D00) lose the HEAD dtor's MAE and the ctor's EngineNative binders and gain the PDB EAE.
- pick-any (owners by address, work/owners.tsv; objects rebuilt in work/o4): ??_GCEngine EQ 0x00B2AB10, ??_GCIEngine EQ 0x00B23770 (needed the
  inline ~CIEngine: with the out-of-line declaration it was 30 v 36), ??0CIEngine@@IAE@XZ EQ 0x00B23750 (unreferenced copy; the landed self row
  spells QAE: NAME_FIXES), ??1CSimpleMemoryPool EQ 0x00987F10, ??_GCSimpleMemoryPool EQ 0x00987F30, ??_H EQ 0x004038C0, ??_I EQ 0x00401400; the
  rest (STLport vector / allocator helpers, CArray ctor / dtor, CDefPointer ctor / dtor / Release, the inline ctors) have no catalog owner.
- Table: every reloc_bindings line whose witness is one of my seven rows was listed; the four 0x00B260B0 stand-in lines go (TABLE_CHANGES),
  the rest are still emitted. Link: work/lk3.txt (see the last section).
- Tests unchanged (standalone host models; 0x00B260B0's models EngineNative: HEAD debt).

## Entry 4 (q12659, 0x00418DCA) -- not started (context; the brief puts it last and only with the 469-includer proof).

## Rule slips
- Not read in full: docs/COORDINATION.md, the NF92 BRIEF / NOTES, GOTCHAS.md (grep only), the ASM_RULES body (headings only).
- I graded and staged 0x00890D80 before claiming its lock (it read `released`; claimed before hand-back). No other: no git, no foreground
  sleep, every wait by PID, every grade under `timeout` (900 or less), cleanups through tools/clean.py only.

## Self-check
- recheck_final.log: one vland --recheck-landed run over all 7 rows, overlay on: 7 / 7 RELOCATION_MATCH + PASS (one HEAD RELOC_NAME warning
  on 0x00664A50, unchanged code). No __asm / naked / _emit. No new local model of a class that has a header: every row either keeps its
  HEAD local model (q12690, in-place) or drops all of them (0x00890D80, 0x00B260B0, 0x00B261D0).
- Includers of the changed row files: none (grep of rebuild/src, rebuild/tests, rebuild/include for all seven basenames, run).
- Root check: work/root_before.txt v work/root_after.txt.

## Link of the CEngine set (work/lk3.txt; 0x00B260B0 0x00B261D0 0x00B2AB10 0x00B23770 0x00A5C570 0x0099A310 0x00B23750 0x00B26340 0x00B25D80)
- HEAD: 2 LNK4006 (??0CInitBaseClass in 0x00B23770 v 0x0099A310; ??0CIEngine in 0x00B23770 v 0x00B23750): HEAD debt, unchanged.
- New: the same 2 plus ONE NEW: ??0CEngine@@QAE@XZ defined by 0x00B260B0 (now its real self leaf) and by 0x00B2AB10, whose row is a
  ??_GCEngine "TU emitter" with a 1-slot local CEngine model and a strong scaffolding `CEngine::CEngine() { }` (HEAD's ctor row defined
  ??0EngineNative, so they never met). Not fixable inside my two rows: 0x00B2AB10 emits ??_G only by defining the ctor or the dtor, and
  both now have their own strong rows. Both new rows already carry an EQ pick-any ??_GCEngine copy (= 0x00B2AB10's body). Decision 6.

## Decisions for the coordinator
1. q12690: the three local-model rows moved in place (their HEAD models kept; the 0x00664A10 header form only flips the opt-in). Header-port
   them (onto CThingCreatureBase.h with UNSHEATHERANGEDWEAPON / _BEDSLEEP / ORIGINAL_VTABLE, as 0x00664A10) in a later round, or accept in place?
2. The edited opt-in line in CThingCreatureBase.h: `protected: ...; public:` on one line keeps the file's line count (no assert moves). Accept,
   or prefer separate lines (pin_asserts would then pin the shifted asserts)?
3. 0x00890C80 (EntityUnsheatheMeleeWeapon, the same shape) is the obvious next port with the 0x00890D80 recipe (NAME_FIXES): queue it?
4. 0x0122ED70: the ctor now binds `__real@0000000000000000` (the true compiler spelling) instead of `?InitialPreloadTime@@3NB`; count 3 -> 3.
   Accept (and queue 0x00ADF1E0 / 0x0089BDF0's g_clearDouble / g_zeroPitch onto it)?
5. The ??0CGameEvent LNK4006 of 0x00890D80 (strong owner 0x004159B0) is the known debt of all 16 CGAMEEVENT_INLINE rows: accept on the same terms.
6. q12692 one-definition: 0x00B2AB10 (??_GCEngine TU emitter, strong scaffolding ??0CEngine@@QAE@XZ) now clashes with the real ctor row
   0x00B260B0. Options: (a) land the CEngine pair with 0x00B2AB10 re-pointed (e.g. its row graded from 0x00B260B0's object, whose ??_GCEngine
   copy is EQ, or folded as the TU-emitter rule queued at nf159 proposes: one TU emits ctor, dtor, vtables and ??_G); (b) hold the pair until
   that rule exists; (c) accept the LNK2005 as debt (as for static initializer / atexit pairs). The header (5 opt-ins, 0 includers changed)
   can land on its own either way.
