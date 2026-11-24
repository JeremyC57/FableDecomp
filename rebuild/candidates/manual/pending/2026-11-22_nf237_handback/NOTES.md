# NF237 notes (2026-11-22, round 11b): CODEXP27 + AUTH24 NAME_FIXES (137 queue lines)

## 1. Triage (work/census0.tsv, census1.tsv, pdbcheck.tsv, off.tsv)
- census.py: per line, the landed self row and its `// Leaf:`, every reloc_bindings spelling at the address with its witness row, and every
  landed row that references the address (rel32 + absolute scan of Fable.exe). PDB check: llvm-pdbutil publics of Ego_r / FableWin
  (`work/pub_*.txt`; the Ego_d publics dump is empty, Ego_d / Ego_r TYPE records used instead where cited).
- Off set (work/off.tsv): NF236 rows.txt; every lock not `released`; pass-28 RESERVED.tsv; the witness rows AND targets of the 54 held p27
  bindings (`batches/2026-11-22_landing_p27/logs/reloc_bindings_held_split_growth.tsv`); 0x0042BEC0; 0x00B4D370 / 0x00BD3070.
  22 lines need an edit in a row of that set and are `deferred` (APPLIED); lines whose self leaf and table spellings already equal the
  claim are `done-already` even when a held row references them. Lines 134 / 135 only needed the self rows (the AUTH24
  binder 0x00839420 already binds the const spellings), so they were done without touching 0x00839420.
- 60 lines are `done-already`: the self leaf equals the claim and every table spelling equals it. Those marked `no table spelling:
  vtable slots unchecked` are virtual slots or unreferenced: the table holds no vtable-slot spelling, and vtcheck was NOT run on them.
- Every claimed spelling I acted on is in both Ego_r and FableWin publics except IsSubtitlingEnabled / GetShowTargetingStatus (FableWin
  public + Ego_r TYPE record and member offsets: second source).

## 2. Done (8 rows, all RELOCATION_MATCH / MATCH + PASS, recheck_final.log)
- **CMaterialDef (L134 / L135):** self rows 0x008EE790 / 0x008EE720 onto the const PDB forms, in place. 0x008EE790's local stand-in
  `CBlendedEffectMap::find` becomes const (QAE -> QBE) because a const member must call a const find; retail 0x008EE890 is the STLport
  const `_Rb_tree::find` (its own row's leaf), so the stand-in stays a stand-in: 0x008EE890 2 -> 2. Full port onto CMaterialDef.h's
  `std::map` member (binding the STLport spelling, 2 -> 1) is a follow-up (the find must stay out of line: it is a separate retail function).
  0x008EE720 still binds the 0x0049D800 stand-in `?GetRandomGenerator@CEngineRandomOwner@@...`; its siblings 0x008EE5A0 / 0x008EE600 bind
  the PDB `?GetLocalSeed@CWorld@@QBEPAKXZ` (Ego_r + FableWin). Respelling it needs CWorld from its header (renaming the local stand-in to
  CWorld would be a new local model): follow-up, 0x0049D800 2 -> 1.
- **CUserProfileManager getters (L26 + 8 callees):** 0x0048DB35 self -> IsSubtitlingEnabled (was a free-function stand-in named after
  IsCameraResetting); 0x00548790's eight `GetOption<addr>` stand-ins -> the FableWin spellings 0x00548B50 binds; 0x00548790 bound
  `?IsCameraResetting@..` at 0x00548630 while 0x00548B50 binds it at 0x00548690 (one name, two addresses): 0x00548630 reads +0x30 = Ego_r
  UsingHeadPhones, so it is IsUsingHeadPhones. 0x00548B50's `IsShowTargetingStatus` is in no PDB: GetShowTargetingStatus (FableWin public;
  Ego_r method + ShowTargetingStatus +116 = retail +0x6C; engine/CUserProfileManager.h already declares it). Census (callers.py: each getter has exactly two
  retail callers, 0x00548790 and 0x00548B50): binder spellings 2 -> 1 at 0x0048DB35 and at each of 0x00548630 / 650 / 670 / 690 / 6B0 /
  6D0 / 6F0 / 740; counting self leaves too, 0x0048DB35 3 -> 1 and each getter 3 -> 2 (their self rows are stand-ins or carry no
  `// Leaf:`; not respelled here). Offsets checked against Ego_r.pdb (retail = Ego_r - 8 after the two 16 -> 12-byte containers):
  +0x6C ShowTargetingStatus 116, +0x74 ScreenAspectRatio 124, +0x79 CameraResetting 129. The getters' own self rows are stand-ins with wrong catalog names
  (NAME_FIXES.tsv lists the catalog fixes for 0x00548630 ... 0x00548740; the self rows were not respelled).
  The 0x0048DB35 HEAD test used inline `__asm` (purity breach); the new test has none.
- **Binder respells:** 0x004D3F50 (CTCHeroOpinionDeedLog ctor CThing& : 0x006C5A80 2 -> 1); 0x006EB390 (private ABE: 0x006EAE20 3 -> 2);
  0x009AE150 (a BODY change, not only a respell: an explicit `context.CheckUnknownSymbols()` after `Flags |= 5`, the retail order, and the
  local out-of-line `~CPersistContext` declaration goes; evidence: engine/CPersistContext.h has an empty `~CPersistContext() {}`, 0x009BC120
  calls CheckUnknownSymbols explicitly, and no retail unwind path calls 0x009BBBF0; 0x009BBBF0 2 -> 1).
- Every changed row: sc.sh objdump diff v HEAD = only the respelled symbols (work/sc_final.log); pick-any (work/pk_final.tsv): the only
  changed DEF copies are the two row-local stand-in inlines (find, vector operator[]), no owner, unbound; everything else identical to
  HEAD. Link (work/lnk_final.txt): 0 LNK2005 / LNK4006 over the 8 objects. No header changed (hdr/README.md): no equivalence run needed.
- No address grows: every TABLE_CHANGES add replaces a delete at the same target, or is the address's only spelling.

- `selfcheck.py` (NF214 brief) is not in `$S/w22/nftools`: no selfcheck ran. CBlendedEffectMap also appears in 0x0045C112, but only in a
  comment (that row is on stlport/fable_map.h): the find stand-in's only emitter is 0x008EE790.

## 3. Open families (not started; in the order I would take them)
1. **NUISystem list family** L13-L18: 0x0042AB1C's self leaf `??1list_T_0042ab1c@std@@` is a stand-in AND it emits
   `?Base_0042a141@CActiveFile@@` for 0x0042A141 (L14); 0x0042ABAF / 0x0042ABCA self rows are spelled ~list where they are
   ~_List_base, and 0x00583815 binds the list names there; 0x0042AADC / 0x00429FBE `_M_create_node` stand-ins (L13). One STLport-header unit.
2. **CPlayerGui / NPlayerGui family** L20-L24, L38, L39: seven `struct T` self rows; CPlayerGui.h / CGamePlayerInterface.h /
   CTCInventoryWeapons.h already declare the members (p27 opt-in FABLE_RETAIL_CDRAWABILITYBUTTONS_WEAPONABILITIES_PASS27), but the bodies
   reach NPlayerGui draw classes (+0x218 / +0x220 counted pointers) that need their own header members.
3. L4 / L5 (0x0074FCE0 `FableVector_0074fce0` stand-in namespace for the pair<long,CCharString> STLport helpers), L36 (0x005518E0
   `RetailUI::Find`), L40 (0x005F88C8 `AllocateInsertNode`), L117 / L122 self rows, L119 (0x00B3E580's `Destroy_00b3e1a0` DeleteFunc).
4. L3 remainder: 0x006EB9F0 models `CCombatManager_Retail` (stand-in token) -> port onto CCombatManager.h.
5. L27 (0x00499A70 gets CProgressDisplay from fable_boot.h, public: needs a fable_boot.h opt-in or a port onto engine/CProgressDisplay.h);
   L28 (0x0063E480 defines a local non-template CThingSearchTools::GetAllThingsInRoughArea with CThingFilter_All).
6. The other `open: self row respell` lines (STATUS-less): L29, L32, L34, L48, L50, L66, L83, L92, L95, L96, L100-L103, L107, L109,
   L110, L114, L115, L118: self rows whose leaf is a stand-in or missing; binders already right where any exist.

## 4. Decisions needed from the coordinator
1. The 22 deferred lines: most are deferred only because a referencing row is a witness of a held p27 binding (NF236's). If NF236 only
   edits the held bindings' lines, many of these (e.g. L45 0x0065C781 private UpdateTorchTasks, L88 0x0090FDB0 / 0x009387C0 protected
   SitInSeat) are one-line in-place respells: release them to the next naming agent once NF236 lands.
2. L130-L133 are debt lines, not spelling claims: 0x0051C920 -> 0x00517850 `~_Rb_tree<unsigned long>` binding (signed erase, wrong;
   correct address unproven), the 0x00628000 / 0x00627BE0 one-TU pair, the JackDragon IsStillRunnable same-TU copies, the
   GetSubDef<CTavernGameShoveHaPennyDef> explicit-v-implicit instance. They need rulings, not respells.
3. L105 0x00993B10: ~CDataInputStream is a real (FableWin) function; header-only binders call the implicit one where retail calls
   ~CDataStream. Accept the binding as debt, or open a header item (inline the implicit dtor)?
4. L136 0x0122DEE0 (`__real@bf800000` v `?g_fMinusOne@@3MA`): ~100 referencing rows, several held (NF236 / AUTH24). Needs a ruling on
   which spelling a float-constant address takes before anyone respells.
5. L137 0x0129BA3C `?DELTA_NOTIONAL_ZERO@@3MB`: the table already has only this spelling; only the label `?g_NormaliseEpsilon@@3MB` is
   wrong (ghidra labels_*.tsv / catalog fix, coordinator side).

## Descriptive names
None introduced (every new spelling is a PDB spelling).
- PDB publics were dumped with `llvm-pdbutil dump -publics $S/codex27/pdbx/{Ego_r,FableWin}.4k.pdb` (deleted after use for disk; rerun to reproduce).
