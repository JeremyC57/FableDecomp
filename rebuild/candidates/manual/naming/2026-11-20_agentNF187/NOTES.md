# NF187 notes (round 10: game / creature / UI / input member names)

Partial hand-back at an entry boundary (context). Counts (APPLIED.tsv / STATUS.tsv): 67 entries: 8 done, 16 done-already, 8 blocked,
1 deferred, 34 open (q6533 and every entry after it in queue order, not started). Rows: 16 changed (2 MATCH + 14 RELOCATION_MATCH,
every test PASS in recheck_final.log, one vland run), 20 not-a-binder, 7 blocked, 1 deferred. selfcheck: 0 problems. Header opt-ins: 1
(FABLE_RETAIL_STLP_LIST_DTOR_DECL_ONLY, hdr/README.md).

## Per entry
- q4154 0x01228E44: self row re-authored as the compiler's `_$E2` from the definition of NUISystem::CList::GPreviousSelected on
  engine/NUISystem_CList.h (FABLE_RETAIL_CLIST) under `#pragma optimize("s")`, like the nf89 / nf164 static pairs. New overlay opt-in
  FABLE_RETAIL_STLP_LIST_DTOR_DECL_ONLY (stlport/fable_containers.h): with the default `~list() { }` the TU emits a pick-any
  ~list<CList*> that is a 5-byte `jmp ~_List_base` (graded against 0x0053C9BC: DIFFER 5v23), so ~list is declared only. The other
  table-owned copy, `_List_base<CList*>::clear` (0x004855A3), is byte-identical to the owner row's object (cmpbody: 39 = 39 bytes,
  relocations equal). The row defines GPreviousSelected (2V, the header's default branch, same symbol as the landed initializer
  0x0121A6AC): the known LNK2005 of a static's initializer / atexit-destructor pair (EXTRA_RULES 2026-11-20). DEBT for
  NAME_FIXES_QUEUE. The initializer 0x0121A6AC grades RELOCATION_MATCH (`_$E1`) from the same source, so it is ported too (lock
  claimed; its local NUISystem::CList / STLport model removed; test rewritten on the header): the pair lands from one form in this
  hand-back and defines one GPreviousSelected symbol.
  - Callee 0x0053C9BC: the stand-in ?Invoke_0053c9bc@AutoTinyThunkTarget@@QAEXXZ is replaced by the FableWin.pdb spelling
    ??1?$list@PAVCList@NUISystem@@V?$allocator@PAVCList@NUISystem@@@std@@@std@@QAE@XZ (FableWin 0x021A8566; not in the Ego_r name set). The
    address keeps two spellings (its own self leaf is the descriptive placeholder ??1list_T_0053c9bc@std@@, the only referencer is
    0x01228E44): count unchanged. A self port of 0x0053C9BC onto the header (DTOR_IN_LIST + explicit instantiation) would close it;
    not attempted (one PDB only, outside the entries; NF185's STL slice).
  - Data 0x013B8A98: the stand-in g_AutoTinyThunkObject_013b8a98 is replaced by the header's default 2V spelling (16 rows bind it).
    The PDB access is protected (1V, NF156 / NF179: header claim queued; NF156 wrote "never onto 2V"). With 0x0121A6AC now on the
    header too, flipping the header's default branch moves both pair rows and the header-based 2V binders together (see Decisions).
- q4420 0x0072A090: claim holds (camera-point slot 28 bodies), but the type-7 class binds no symbol (FABLE_FORCEINLINE template on
  a local model); the source fix needs a whole-row port (17 local models with shared headers). Blocked: port, 0 forms.
- q5009 0x00A475C0: in-place respell of the local CNavigatorBase declaration in 0x00A1AB40 / 0x00A1BCA0 / 0x00A1BEE0 (protected
  const, const pointers); node locals made const (the row-local casts of GetNodeAt's const result dropped in 0x00A1BCA0; 0x00A1BEE0's
  local LastNavigableNodeVisited made const, as engine/CNavigatorAStar.h has it). All 5 REFS rows bind one spelling.
- q5031 0x009F5690: self already right; binder 0x009F5D70 respelled in place (CAFile* -> CAFile&).
- q5081 0x004A6AB0: self already right (queue note stale); ??_G 0x004AE8D0 respelled in place (NF182 q4021 recipe). Its self leaf
  stays a stand-in (WARN STANDIN_LEAF, pre-existing).
- q5203 0x006159A0: self already right; binder 0x00615920 ported onto engine/CPersistContext_TransferableVector.h + CPersistTraits.h
  + CTCInventoryMap.h (self rename, RENAMES s1). The traits reader is declared as a member explicit specialisation
  (`template <> void CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferIn(...);`), matching the owner row 0x006159A0,
  which defines it through a row-local explicit CLASS specialisation `template<> class CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>`
  (same mangled member). Test rewritten on the shared headers. Pick-any: the object defines the header-inline
  ??0CUIMiniMapMarker@CTCInventoryMap@@QAE@XZ, whose owner is 0x005FA530 (Leaf `??0CUIMiniMapMarker`, a global-class spelling): graded
  against 0x005FA530 it is RELOCATION_MATCH (byte-identical); the other new COMDATs have no owner (reloc_bindings + Leaf-line grep).

- q6244 0x00A14040: done-already (all 33 REFS rows call RotateXY on a global C3DVector; reloc_bindings has one spelling).
- q6247 0x00ABA370: done-already (self fixed 2026-11-12; 0x00ACDB80 is locked by NF188, read only: it uses the header accessor).
- q6294 / q6299 0x00A04110: done-already (NF160). q6295 / q6300 0x00A04030: self renamed in place SetAsJoystickPosition1 ->
  SetAsJoystickPosition (PDB twin order; the only caller 0x00AB6E40 already binds it; narrows a self-vs-caller split); test respelled.
  q6301 0x00A04060: already SetAsJoystickPosition2; Leaf line added; label fix in NAME_FIXES.
- q6317 0x0079AA90: done-already.
- q6323 0x0138306C: claim REJECTED. Ego_r.publics / FableWin.publics (/tmp/claude-0/egod_pdb/*.publics.txt) carry
  `?CurrentCreatureGlobalDefNum@CTCSummonSpell@@0JA`, and Ego_r types list it as a private LF_STMEMBER of CTCSummonSpell. 0x0079A6E0 /
  0x0079B650 / 0x0079ACC0 respelled in place (made-up stand-in -> PDB spelling; the address keeps 2 spellings, count unchanged; the
  0x0079A6E0 test's own model respelled too). 0x0072AB40 (CTCSpecialAbilities vtable 0x0126683C slot 1, persist key
  "CurrentCreatureGlobalDefNum") still binds ?CurrentCreatureGlobalDefNum@CTCSpecialAbilities@@2JA: see Decisions.
- q6321 0x007F8860: CTCParticleAttacher has NO shared header (grep `class|struct CTCParticleAttacher` over rebuild/include, _quarantine
  included: none). The landed 0x007DE710 / 0x00772BB0 model it as `class CTCParticleAttacher : public CTCBase` on engine/CTCBase.h.
  0x008CCD70: stand-in struct replaced by that sibling shape (+ engine/CTCBase.h include) - one form, RELOCATION_MATCH; its only
  defined-symbol change is the local CThing::GetInterface<T> COMDAT's T. 0x007DE5D0: the local CTCBase model replaced by
  engine/CTCBase.h, then the same sibling shape - one form; a single C-style downcast `(CTCParticleAttacher*)&GetTC(...)` remains
  (HEAD had a double cast to the stand-in): cast debt (nf105: one side is a header-less local model); the typed GetTC form was not
  tried. 0x006412B0: blocked/port (stand-in on a local CTCBase, no engine includes; 0 forms). End state: a shared
  engine/CTCParticleAttacher.h (coordinated header item), after which these rows include it.

- q6329 / q6337 0x006CEAE0: done-already (NF14 re-identified the self row; the only caller 0x0092FAB0 binds it).
- q6372 / q6409 0x00E80D60: blocked: port. Both callers already bind the PDB PlayCutscene; the self row is all stand-ins
  (Row00e80d60::Script::Run); its rename is a re-author on engine/NScript_CV_FishingCompetitionScript.h and the NF178 cast-map family.
- q6396 0x0073A690: deferred: the only binder 0x00861A80 is locked by NF185 (self rename and binder must move together).
- q6423..q6432 (spell container virtuals): done-already (self rows already on the shared headers; vtable-only).
- q6438 / q6441 0x00662000, q6443 0x007A9300 (NF107): done-already.
- q6452 / q6461 0x0062E900: blocked: port; NF181's census of the 471-row family stands (rows left listed in APPLIED).
- q6456 0x00662D80: 0x008EA9F0 respelled in place (the inline local CHoverAnim ctor COMDAT now takes const CAnimationEntry*; no owner,
  no header for CHoverAnim). Left: 0x0093C6D0, which hands the entry to the local CCreatureAction_PlayAnimation ctor 0x00842570
  declared (CThing*, CAnimationEntry*, ...) while the PDB has (CThingCreatureBase&, const CAnimationEntry&, ...): it moves with that ctor.

## Split census (no new splits)
- 0x0053C9BC 2 -> 2 (stand-in -> FableWin spelling), 0x013B8A98 unchanged set (stand-in -> existing 2V), 0x00A475C0 3 -> 1,
  0x009F5690 2 -> 1, 0x004A6AB0 2 -> 1, 0x006159A0 2 -> 1, 0x00A04030 2 -> 1 (self rename onto the caller's spelling),
  0x0138306C 2 -> 2 (made-up -> PDB), 0x007F8860 3 -> 2, 0x00662D80 3 -> 2. 0x00615920 / 0x01228E44 self renames: no landed referencer
  binds the old leaf (0x01228E44's only referencer binds `_$E2`).

## New local models (three-condition rule)
- 0x008CCD70 and 0x007DE5D0: `class CTCParticleAttacher : public CTCBase` (no header anywhere under rebuild/include; landed siblings
  0x007DE710 / 0x00772BB0 use the same shape; base from engine/CTCBase.h).

## Descriptive names
- None added.

## Flags lines
- None added.

## Decisions for the coordinator
- q6323 / 0x0072AB40: the PDB static is PRIVATE to CTCSummonSpell, but the CTCSpecialAbilities persist hook reads it. FableWin has no
  accessor; the llvm-pdbutil type dump prints no LF_FRIENDCLS records at all (0 in 3,800 classes), so friendship cannot be read off.
  Moving the row needs engine/CTCSummonSpell.h with `private: static long CurrentCreatureGlobalDefNum;` plus `friend class
  CTCSpecialAbilities;` (a friend in a shared header: a header claim), or a decision to declare it otherwise.
- q4154 pair: 0x01228E44 (`_$E2`) and 0x0121A6AC (`_$E1`) land together from one source and both define GPreviousSelected: the
  known LNK2005 of the 2026-11-20 pair rule (DEBT for NAME_FIXES_QUEUE; the TU-emitter rule fixes it).
- q4154 access: the pair defines the header's default PUBLIC 2V spelling, against NF156's "never onto 2V". Alternatives:
  (a) land as is (2V, consistent with the 16 header-based 2V binders), and flip the header default to protected 1V + the FableWin
  GetPreviousSelected accessor as the queued header claim (moves the pair with it); (b) define the pair under the 1V branch now
  (FABLE_RETAIL_PASS22_SLIDER_PROCESS_EVENT, a generic macro that also changes C2DVector / CAction / CSlider declarations; not graded),
  which leaves the 2V binders on a symbol nothing defines until the header claim lands. I chose (a).

## Pick-any (every newly defined COMDAT)
- reloc_bindings owners + a grep of every `// Leaf:` line (long and short prefixes, by hand for ??0C2DVector / ??A?$vector@ /
  ??0?$list@ / ??1?$_List_base@): 0x01228E44 / 0x0121A6AC define _List_base<CList*>::clear (owner 0x004855A3: cmpbody byte-identical);
  0x00615920 defines ??0CUIMiniMapMarker@CTCInventoryMap@@QAE@XZ (owner 0x005FA530 under a global spelling: graded against it,
  RELOCATION_MATCH). No other new COMDAT has an owner (the bare `??0?$list@` / `??0?$_List_base@` leaves are other element types).

## Funclets
- Include-by-filename scan of all 16 changed rows over rebuild/src/compiled and rebuild/tests: no funclet row includes any of them
  (none for the self-renamed parents 0x00615920, 0x00A04030, 0x01228E44 either).

## Locks met
- NF185: 0x00861A80 (q6396 binder; entry deferred). NF188 (not in the brief's running list): 0x00ACDB80 (q6247; read only, not graded).
- acctB: 0x00A1E5C0 (q5009 REFS; not touched; its source already declares the PDB spelling).
- Claimed: 0x00A1AB40, 0x009F5D70, 0x004AE8D0, 0x0079A6E0, 0x008CCD70 (was `released`), 0x0121A6AC; 0x0053C9BC claimed then
  released (not changed).

## Rule slips
- My first Bash call exited 1 (`wc` over directories).
- q6323 was recorded before q6321 reached its terminal state (q6321 was read first, q6323 settled, then q6321 finished): queue order.
- I graded 0x004855A3's HEAD source (read-only pick-any comparison) without checking its lock first; it had no lock.
- I graded 0x00A1ABF0 / 0x009F5D70 HEAD sources before claiming 0x009F5D70 (0x00A1ABF0: no lock; probe only).
- grades_used in STATUS.tsv was reconciled from work/grades.log at hand-back (g.sh runs plus logged q.py probes).
- 0x007DE5D0's test still uses the stand-in names (CTCComponent82 / ClearEntries) in its own standalone model; it PASSes and binds
  nothing, but it was not respelled.
- The REFS output of q6452 was printed in full (very large), costing context.
