# NF145 notes (2026-11-20, round 10: follow-ups owed by the nf137 / nf140 landings)

## Counts (STATUS.tsv / APPLIED.tsv)
- Entries (6): done 5 (q11780, q11784, q11808, q11809, q11810); done-already 0; rejected 0; blocked 0; deferred 0; open 1 (q11785,
  partial: 0x00956960 left, decision 1).
- Rows: 21 changed, all RELOCATION_MATCH + test PASS (recheck_final.log): ported onto shared headers 6 (0x0122A9A0, 0x009BFE70,
  0x009BFF10, 0x009BFF40, 0x009C0020, 0x009566C0); inline control only 13 (0x009D52D0 + the 12 Deallocate users, already on the header);
  in-place respells of HEAD local models 2 (0x00B24690, 0x008E8830). 6 + 13 + 2 = 21.
  Not-a-binder 3 (0x008E8C50 already defines the InvulnerableTeleport name; 0x004189C2 already binds the PDB 0x009BFF10 spelling, probed;
  0x009551A0 already on the opt-in). Blocked 1 (0x00956960, not attempted: q11785 follow-up, decision 1).
- Header opt-ins: engine/CMemoryAllocator.h (forces dropped, ALLOCATE_FORCEINLINE removed, new _DEALLOCATOR), engine/CDisplayManager.h
  (new _RESETDEVICE). hdr/README.md.

## q11810: inline control (EXTRA_RULES 2026-11-08 preference 2 replaces the nf140 FABLE_FORCEINLINE)
Header made plain `inline` in a sweep copy (work/sw); each row graded flagless and under /d2inlT<N> / /d2inlS<N> (logs work/sweep_*.log).
- 0x009D52D0 (Allocate): flagless DIFFER 254v310. T74..T77 DIFFER, T78..T800 match (T78, 79, 80, 87, 93, 100, 114, 150, 200, 400, 800
  tried). S15..S39 DIFFER, S40, S60, S100 match. Smallest: /d2inlT78 (chosen; T is the threshold knob, S40 is a 4x change of the
  always-inline size). The ALLOCATE_FORCEINLINE opt-in had no other user, so it is removed from the header; the row no longer defines it.
- 0x00BD9B90 (Deallocate + Destroy): flagless DIFFER 68v97. T74..T113 DIFFER (68v97), T114..T2000 match. S12..S30 DIFFER 68v97 (Destroy
  called), S40..S84 DIFFER 16v97 (Deallocate called), S85..S100 match. Smallest: /d2inlT114.
- The other 11 Deallocate users, every T74..T114: 0x009D61F0, 0x009A4350, 0x009A1660, 0x009C6020, 0x00A02220, 0x00A02190 DIFFER at
  T74..T113 and match at T114 (same threshold step as 0x00BD9B90: the Deallocate site cost); 0x009C6D60, 0x00A02570, 0x00A02370,
  0x00BDA150, 0x00BD9CE0 match at every T including the default (confirmed flagless), so they need no flag.
- Result: the flag works for every user of both opt-ins, so both forces are dropped (coordinator rule: no FABLE_FORCEINLINE in a shared
  header opt-in unless a flag sweep failed). 7 rows carry `// Flags: /d2inlT114` + an `// Inline-budget:` line, 0x009D52D0 carries
  `// Flags: /d2inlT78`; the 5 flagless rows have a short `// Inline:` note instead of their old Inline-budget line.
- vland's flags come from the payload: mkpay.py reads only the catalog CompilerFlags, so work/pay.sh adds each row's `// Flags:` line.

## Flags lines (the catalog CompilerFlags must get them; GOTCHAS 2026-11-19 catalog-flags line)
- /d2inlT78: 0x009D52D0.
- /d2inlT114: 0x009D61F0, 0x009A4350, 0x009A1660, 0x009C6020, 0x00A02220, 0x00A02190, 0x00BD9B90.

## q11809: 0x0122A9A0 onto engine/CMemoryAllocator.h
- Opt-ins _CTOR (storage: NoAllocations +0x40C), _DEALLOCATE (DestroyWhenEmpty, ~CMemoryAllocator, operator delete, inline Destroy) and the
  new _DEALLOCATOR (nested CDeallocator, static Deallocator, operator delete's body for this TU: VirtualFree MEM_RELEASE).
- Data names: the PDB name sets hold no data records for 0x013CAA90 / 0x013CAA94, so neither spelling has two sources. Took the header's
  ?PMemoryAllocator@CMemoryAllocator@@1PAV1@A / ?DestroyWhenEmpty@CMemoryAllocator@@1_NA (every other CMemoryAllocator binder uses them; the
  row's PAllocator / ShuttingDown had no other binder). 0x013CAA94: 2 spellings -> 1; 0x013CAA90 loses one stand-in spelling.
- The +0x40C read: Ego_r.pdb storage names it NoAllocations (GetNoAllocations 0x009F1F10 reads the same word). The old row's
  PeekNoAllocators (FableWin only, `QBEJXZ`) would read NoAllocators +0x410 by that layout, so it is not this accessor; the field is read
  directly (no accessor claimed).
- First form matched with no flag (plain-inline Destroy expands here). Defined symbols vs HEAD: ??_GCMemoryAllocator QAE -> IAE (the
  header's, as the Deallocate users), PeekNoAllocators COMDAT gone; own leaf `_$E2` (static) and the strong ~CDeallocator as at HEAD.
- Test rewritten on the header (VirtualAlloc'd storage, VirtualQuery checks the release; host path stubs VirtualFree).

## q11808: CDisplayManager family onto engine/CDisplayManager.h
- 0x009C0020 and 0x009BFF40 port with unchanged leaves. Their CCreateDeviceParameters copies are now the header's implicit copy
  constructor (pick-any COMDAT, the same symbol the owner 0x009C11A0 emits), so the HEAD local user copy constructors are gone.
- 0x009BFE70: the header nests CPresentationIntervalDesc, so the port gives the row its PDB leaf (`...ABVCPresentationIntervalDesc@1@@Z`,
  both PDBs; NF140 NAME_FIXES). Exe scan: its only referencer is 0x009BFF10, which moves with it.
- 0x009BFF10: HEAD's Leaf line named ?ChangePresentationInterval@CDisplayManager@@QAEX_NJ0@Z but the body defined
  ?SetPresentParameters@CDisplayManager@@QAEX_NJ0@Z (the leaf matched no symbol; q.py graded the largest function). Ported: the body now
  defines the PDB name. Referencers 0x004189C2 (already PDB) and 0x00B24690 (three-bool SetPresentParameters stand-in, respelled in place).
  Spellings: 0x009BFE70 2 -> 1, 0x009BFF10 3 -> 1. The source file name should follow (SetPresentParameters -> ChangePresentationInterval).
- No operator== exists for CPresentationIntervalDesc in either PDB, so 0x009BFE70 writes the two-field test out; no constructor either, so
  0x009BFF10 aggregate-initialises the local. Both matched on the first form.
- Tests of the four ported rows rewritten on the header (zeroed storage stands in for the manager; ResetDevice / ChangePresentationInterval /
  GetColourDepth are recording stand-ins), PassPatterns kept.

## q11780 / q11785 / q11784
- 0x009566C0 on the _CLASS opt-in; the scaffolding is the nf86 placement-new under `#pragma inline_depth(0)` (as 0x004371A0, 0x0041BEB0).
  Its vtable section now equals 0x009551A0's byte for byte and relocation for relocation (work/coffsec.py on both objects).
- 0x008E8830: CCreatureAction_BriarRoseInvulnerableTeleport has no header (grep `class|struct` over rebuild/include: none; nor for
  ForcePush). The respell renames the row's HEAD-local class token in place; landed sibling 0x008E8C50 models the same shape. Judgment
  call disclosed: the local class sits on the row's HEAD-local CCombatActionBase (as ForcePush did at HEAD and as 0x008E8C50 does), not
  on the shared header, so the third condition of the 2026-11-20 local-class rule holds only as it did at HEAD. Its object's only
  changed symbol is the implicit destructor COMDAT, pick-any like the landed owner 0x008E8CB0's.

## Decisions for the coordinator
1. q11785 stays open (partial). One vtable, still two contents: landed 0x00956960 (CCreatureAction_ThrowShotObject::Clone, REFS row of 0x012944D4)
   emits ??_7CCreatureAction_ThrowShotObject@@6B@ from a local model with a placeholder v00. Moreover, the shared _CLASS opt-in class has no
   overrides, so its vtable binds CCombatActionBase::Clone at slot 0x2C and CCreatureActionBase::Initialise at 0x30, where retail 0x012944D4
   holds 0x00956960 (Clone; both PDBs ?Clone@CCreatureAction_ThrowShotObject@@UBEPAVCCreatureActionBase@@XZ) and 0x008B66D0 (catalog
   CCreatureAction_ThrowShotObject::Initialise; Ego_r ?Initialise@CCreatureAction_ThrowShotObject@@UAEXXZ). Fix (not done, context): a
   member-specific opt-in declaring those overrides, defined by 0x009551A0, 0x009566C0 and a header port of 0x00956960 (its own leaf
   becomes the PDB UBE Clone; no code referencer). Not claimed. HEAD 0x00956960 probed: its object emits a 4-byte vtable whose one slot is
   ?v00@CCreatureAction_ThrowShotObject@@UAEXXZ (work/vt_00956960_head.txt). Evidence strength: slot 0x2C (Clone) is solid (retail vtable
   read + both PDBs); slot 0x30 (Initialise) is weaker (catalog label + an Ego_r name at a donor address, no FableWin cross-check made).
   Resume: one member-specific opt-in (e.g. _THROWSHOTOBJECT_OVERRIDES) defined by 0x009551A0, 0x009566C0 and 0x00956960 in the SAME
   hand-back; port 0x00956960 onto it (Clone body `return new CCreatureAction_ThrowShotObject(*this);` over the header's
   CCombatActionBase copy constructor 0x0062F3B0); compare the three vtable sections with work/coffsec.py and against the retail slots.
2. 0x008E8830 local-class rule exception: the row now defines CCreatureAction_BriarRoseInvulnerableTeleport (header-less) where HEAD defined
   CCreatureAction_BriarRoseForcePush, on its HEAD-local CCombatActionBase model. Conditions 1 (no header) and 2 (sibling 0x008E8C50 has the
   same shape) hold; condition 3 (base from a shared header) does not, exactly as at HEAD. Accept as an in-place token respell, or treat
   q11784 as `blocked: port` (0x008E8830 onto engine/CCombatActionBase.h and the other headers it models locally) and drop the row.
3. Removal of the ALLOCATE_FORCEINLINE opt-in from engine/CMemoryAllocator.h (its only user, 0x009D52D0, is in this hand-back).

## Tests kept (not rewritten on headers)
- 0x009566C0: its 11-line test models generic Base / D classes (no shared-header class name); a header-based test would have to define
  every one of the 70 vtable slots to link. Kept unchanged; passes.
- 0x00B24690 and the 13 q11810 rows: unchanged standalone tests, all pass (their rows changed only flags / a member spelling).

## Needs header claim
- None. No existing shared-header member was renamed (operator delete keeps its declaration unless _DEALLOCATOR is defined).

## Local classes
- No row gained a class definition of a header-backed class. 0x008E8830: header-less token respelled in place (above).

## Defined-symbol diffs (objdump -t, landing vs HEAD)
- 0x0122A9A0, 0x008E8830: listed above. q11810 rows: only header inlines (Allocate / Deallocate / Destroy / ??_GCMemoryAllocator, pick-any).
  CDisplayManager rows: own leaf + ??0CCreateDeviceParameters@CDisplayManager@@QAE@ABU01@@Z and ??0C2DExtentsI@@QAE@ABV0@@Z (header
  inlines, pick-any). 0x009566C0: own leaf (pick-any) + header inlines + the anonymous-namespace scaffolding function.

## Descriptive names
- ReferenceThrowShotObjectCtor (scaffolding, anonymous namespace; `// Fidelity: scaffolding, not retail`).
- None else new. DestroyWhenEmpty / PMemoryAllocator are existing header spellings (no PDB data record).

## EH funclets
- No Unwind funclet includes any changed row (grep of rebuild/src/compiled for each row file name).

## Locks
- Claimed (released -> NF145): 0x009D61F0 0x009A4350 0x009A1660 0x009C6D60 0x009C6020 0x00A02220 0x00A02190 0x00A02570 0x00A02370
  0x00BDA150 0x00BD9CE0 0x00B24690; new: 0x009BFF10. None is in UNMATCHED_ROWS as handed off / reserved.

## Rule slips
- None known. All sweeps ran one compile at a time inside timeout.
