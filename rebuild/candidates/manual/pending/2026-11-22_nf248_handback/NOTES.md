# NF248 notes (2026-11-22, round 17): NF241's 39 open CODEXP27 lines, 0x0049D800, 0x01296F14

Handed back early (context). 10 rows changed, all RELOCATION_MATCH / MATCH + test PASS in one `vland --recheck-landed` run
(recheck_final.log). No header changed (no hdr/ / orig/). No new split: every address touched ends on fewer spellings or the same count.

## Census (work/census0.txt, work/cen2.txt)
census.py (exe E8/E9 + absolute scan mapped to landed rows) plus a HEAD compile of every self row and every code referencer
(tools/hc.sh, work/hc/: grade, relocations, DEF/UND per row). Vtable-slot binders: tools/dref.py (new: the data slot holding the
address, the vtable start code references, the emitter rows), then vtcheck.py on the emitter objects.

## Done (8 lines; 0 binders left on a wrong spelling at the touched addresses)
- **L2 0x00666310** `?OnCreate@CThingCreatureBase@@MAE_NXZ`: self already right. Both code binders 0x006A9EF0 (CThingPlayerCreature)
  / 0x00833A70 (CThingAICreature) bound QAE from row-local models: in-place respell (protected virtual OnCreate; the explicit
  `__vftable` member becomes the implicit vptr; in 0x006A9EF0 the local empty CThing base gets its protected virtual dtor
  `??1CThing@@MAE@XZ`, both PDBs, so the vptr stays at +0: without it the empty base sat at +4, DIFFER). Their own leaves move QAE ->
  MAE (both PDBs `?OnCreate@CThingPlayerCreature@@MAE_NXZ` / `?OnCreate@CThingAICreature@@MAE_NXZ`); nothing binds those addresses
  (vtable emitters 0x0052AB20 / 0x006AA230 / 0x00831F80 / 0x008320C0 model a 1-slot vtable). 0x00666310: 2 -> 1; table line replaced.
- **L23 / L24 0x00446B40 / 0x00446B90**: self rows off the `CGamePlayerInterface_Methods` stand-in onto engine/CGamePlayerInterface.h's
  existing `FABLE_RETAIL_CDRAWABILITYBUTTONS_WEAPONABILITIES_PASS27` declarations (binder 0x0064B1AC's opt-in). The row-local enums
  (HEAD) moved above the include. The opaque `Controls` (+0x7A8) is viewed through a file-static inline `ControlsMapOf`: a plain
  reference local let VC7.1 CSE the second end() read (64 v 72 bytes); the call boundary keeps retail's re-read of `[edi+0x7ac]`.
  2 -> 1 each (FableWin + Ego_r S_PUB32). The queue's "CButtonMap::IsActionOnButton" label is not in the catalog any more.
- **L27 0x00497B60**: binder 0x00499A70 ported off the fable_boot.h CProgressDisplay model onto engine/CProgressDisplay.h (private
  CalculateNextTextTag, AAE). Its own leaf moves QAE -> UAE (`?SetToDisplayText@CProgressDisplay@@UAEX_N@Z`, both PDBs; vtable
  0x012388B4 slot 5; emitters 0x00499CE0 / 0x00499FB0 model 3 slots, so nothing binds it). The 0x0122D70E literal keeps HEAD's
  `?g_FableEmptyText_0122D70E@@3QBDB` spelling (that address has 6 table spellings; not touched). Test rewritten on the shared header
  (raw storage, qualified non-virtual call, offsets +0x44 / +0x7C; the literal needs an `extern` declaration for external linkage).
- **L34 0x004DDAB4**: self off `CTCTargetingBase_Retail` / `CIntelligentPointer_CThing` onto engine/CTCTargetingBase.h +
  engine/CIntelligentPointer.h (`_CTOR_OUTOFLINE`), size pragmas kept. 2 -> 1 (binder 0x00802E00 already right). Its 0x004A96F0
  relocation moves from the `CIntelligentPointer_CThing` stand-in (still used by 0x006542A4) to the existing
  `??0?$CIntelligentPointer@VCThing@@@@QAE@PAVCThing@@@Z` (witness 0x004E2250): count-neutral for 0x004A96F0 (7 spellings).
  Catalog rename SetTargetedThing -> SetPTargetedThing (vland STANDIN_LEAF warning until then). Vtable slots (dref.py): slot 17
  of 0x01241240 (no landed emitter), slot 34 of 0x012419A4 (emitter 0x004DEBD4 models 1 slot) and of 0x01271E74 (emitter
  0x007F58E0 models 28): no landed row binds the address through a slot.
- **L36 0x005348E0**: self = explicit instantiation of stlport/fable_algorithm.h `__find` (FIND_RANDOM_DISPATCH + FIND_NOINLINE_BODY):
  MATCH. Binders 0x005518E0 / 0x00551BC0 in place: the RetailUI::Find stand-in body dropped, the local Lookup entries typed unsigned
  long (second source: engine/CTable.h, Ego_r layout: PHorizontalSeparations +0x168 / PVerticalSeparations +0x16C are
  `vector<unsigned long>*`; first: the landed binders' __find<unsigned long*, unsigned long>), std::__find called with the same two opt-ins as the landed binder 0x00531090 (a bare declaration gave DIFFER: the body must be
  visible, the wf15 note in the header). Their new pick-any `__find` copies are EQ 0x005348E0 (112 B). 3 -> 1; table line deleted.
- **L117 0x00B09D50**: self off the Node / List stand-in onto the shared `_List_base<CSpriteTrailEntry>::clear` (LIST_CLEAR_OUTOFCLASS,
  explicit instantiation; engine/CEnginePrimitiveSpriteTrail.h). 2 -> 1 (binder 0x00B08530 already right).
- **L108-L110**: done-already (landed nf243).

## Proof
- recheck_final.log: the 10 rows in one run. pk.py HEAD v new (work/hcpk_h1.tsv v work/fin_f1.pk.tsv): no (row, symbol) EQ -> NEQ;
  2 HEAD NEQ stand-in Find copies replaced by 2 EQ `__find` copies; the 11 NEQ copies are NEQ at HEAD (local models untouched).
- Link of the 10 new objects (work/fin_f1.lnk.txt): 1 LNK4006, `?Grow@ChildrenVector@RetailUI@@...` between 0x005518E0 / 0x00551BC0:
  both rows DEF it at HEAD too (work/hc/sym_*.txt): HEAD debt.
- Defined-symbol diffs (sc.sh, per row in STATUS): only the row's own leaf, removed stand-ins, and shared-header inline COMDATs
  (iterator-tag ctors, `??1?$CIntelligentPointer@VCThing@@@@UAE@XZ`, the list node `deallocate`).
- Repo root: listed before / after (work/root_before.txt); only `.git`'s mtime moved (not this agent; no git run).

## Open (APPLIED.tsv gives each line's census and blocker)
Large: L37 (0x00535470, 26 referencers / 6 spellings), L129 (0x01436EA8, ~90 / 12+). Units needing ports: L4 / L5 + 0x0074FCE0
(whole local model; pair<int> -> pair<long> self signature, two sources needed), L28 (0x0063E480 onto the 0x0049BE90 pattern), L57
(0x00899DB0, whose own leaf is a stand-in too), L111 (0x00A427A0 needs an add-only CLipSyncDataBank.h opt-in for virtual Update),
L114 (2-row unit), L120 (binder 0x00B620A0). One-row self ports with binders already right (census only, sources not read): L71, L72, L74, L83, L86, L89, L92,
L100, L104, L115; the ctor rows DEF pointer-parameter ctors with row-local vtables, and any parameter-type change beyond pointer ->
reference (L72) is a self-signature change needing two sources. ??_G rows (L29, L32, L50, L95, L96) are free
__fastcall stand-ins: each needs its class's virtual-dtor emitter. L118: check its vtable emitters first. L119: catalog-only + binder.
E2 0x0049D800 and E3 0x01296F14: census in APPLIED; E3 is a 7-row unit (4 vtable binders, 3 slot rows): Ego_r S_PUB32
`??_7CCreatureAction_ScreamerIntoAggressive@?A0x4c815337@@6B@` (FableWin's ctors carry the same hash) puts the class in the anonymous
namespace, so the 3 binders that spell the global-namespace vtable must move into it (the hash itself is stripped, EXTRA_RULES
2026-11-20); 0x0096DF50 already spells an anonymous-namespace one.

## Decisions needed
1. L2: the in-place respell adds `protected: virtual ~CThing();` to 0x006A9EF0's row-local empty CThing (needed so the vptr stays at
   +0 once OnCreate is virtual). Accept under the in-place rule (same class token, nothing new defined), or require a port?
2. L2 / L27: four leaf-only self renames ride along (0x006A9EF0, 0x00833A70 QAE -> MAE; 0x00499A70 QAE -> UAE). Both PDBs; no landed
   referencer (the vtable emitters model fewer slots). OK under the standing rule?
3. Catalog / file renames for the coordinator: 0x004DDAB4 (SetTargetedThing -> SetPTargetedThing), 0x005348E0 (Find_005348e0 ->
   std::__find<unsigned long*, unsigned long>), 0x00B09D50 (StdList_Clear_00b09d50 -> std::_List_base<CSpriteTrailEntry>::clear).

## Flags lines / funclets / tests / descriptive names
No row needed new flags. No funclet includes a changed source. Tests: 0x00499A70's rewritten on the shared header (the HEAD one
used fable_boot.h's model and no longer links); the other nine keep their HEAD tests (host models), all PASS. Descriptive names:
0x005348E0 / 0x00B09D50 STLport spellings are descriptive-by-library (the PDBs carry Dinkumware), as nf239 / nf241 landed.

## Rule slip
One: the L34 grade was launched although its clang precheck had just failed (missing FABLE_RETAIL_DEF_CLASS_BASE / ETCInterfaceType);
the lane cl.exe spun and was killed with tools/killmine.sh (the job ran in the background; no foreground sleep, no git, nothing
written to the repo).
