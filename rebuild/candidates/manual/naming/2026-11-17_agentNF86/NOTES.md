# NF86 notes (round 7, queue q11098..q11120)

Counts come from STATUS.tsv / APPLIED.tsv / RENAMES.tsv in this directory. 83 rows (80 locked + 3 extra rows claimed:
0x00812280, 0x0059B0C3, 0x0059AFD7): 36 ported, 27 not-a-binder, 20 blocked, 0 deferred. 23 entries: 15 done, 1 done-already
(q11098), 7 blocked (q11101, q11109, q11110, q11111, q11114, q11117, q11119). recheck_final.log: all 36 ported rows
RELOCATION_MATCH + PASS in one vland run. RENAMES: 21 section-1 lines, 51 section-2 lines. REJECTED.tsv: no claim rejected.

## Descriptive names (no PDB spelling exists)
- **Descriptive by library (STLport 4.6; both PDBs are Dinkumware):** 0x00451A85 `~_Rb_tree`, 0x00451240 `clear`,
  0x004DA252 `_M_erase` (map<EHeroExperienceStatCategory,float>), 0x0059B4D8 `_M_reserve_map_at_back`, 0x0059B0C3 `_M_reallocate_map`,
  0x0059B49B `__uninitialized_copy`, 0x0059B2CC `__copy_aux`, 0x0059AFD7 `__copy`, 0x0053CCD0 `__uninitialized_fill_n`. Each is
  the STLport 4.6 source name and signature, and each is bound by a landed caller under exactly that spelling (second source).
- **0x00981350** `CAIStateGroupBase::AIAction_SpeakAISpeechToTarget`, seven arguments: in no PDB (TLC-only overload); spelt as NF84's
  CAIStateGroupBase.h declaration (brief: it stays descriptive).
- Every other renamed spelling is a PDB spelling (Ego_r.pdb and/or FableWin.pdb, cited per line in RENAMES section 1).

## For the coordinator to rule on: TU-internal scaffolding that emits a header inline body
0x00860150 (CCreatureAction_GetUpFromSitting ctor), 0x00B9C110, 0x00B9C130 and 0x00BCA060 (`??_G` scalar deleting destructors) get
their bytes from a shared header's inline body. VC7.1 emits an inline body only when the TU uses it. Each row therefore holds one
anonymous-namespace function that references it (calls the ctor, or constructs the class so the vtable and its `??_G` are emitted).
VC7.1 names that function with a hashed `?A0x...` name, so it never collides at link and it is not a retail symbol. The graded
symbol is the pick-any COMDAT (selection 2, `symsel.py`), as the brief asks for q11102 / q11120 ("never a second strong
definition"). The 0x0085FF10 row (NF84) used the same pick-any COMDAT through its in-row inline ctor; that body now lives in the
header opt-in, with the same tokens. If the coordinator rejects scaffolding, these four rows go back to blocked: authoring
together with the 13 other `??_G` rows.

## Why the other `??_G` rows are blocked (0x004585A1 and 12 q11117 rows)
VC7.1 emits `??_G` only with the class vtable, i.e. in the TU that defines the constructor. For 0x004585A1 (CArmourDef) and the 12
sub-primitive classes, the constructor is a separately landed row with a strong (nodup) definition. A scaffolding construction
would need a second definition of that constructor, which gives LNK2005 at the whole-image link (GOTCHAS nf80). The three q11117
rows that were ported have header-inline (implicit) constructors. Each blocked row's STATUS note names its constructor. They
unblock when the `??_G` row and its constructor row land as one TU.

## Placement judgement (q11105..q11107, CPersistTraits pairs)
FableWin.pdb puts each traits ctor directly before its traits dtor in the tc_hero_experience TU: 0x01F00A40 / 0x01F00A60 for
CComboMultiplierData and 0x01F03B60 / 0x01F03B80 for CHeroStatIncreaseData. The Ego_d tc_hero_experience.obj contributions are
adjacent too. Retail has the same order: 0x006DBFE0 / 0x006DC020 and 0x006DC1B0 / 0x006DC1F0. With NF84's 0x006DC020 decision
(~CPersistTraits<CComboMultiplierData>), this makes 0x006DBFE0 its constructor. The name rests on TU adjacency plus the body
(empty traits ctor, only the vptr store). That is the best evidence available; no other function fits the slot.

## Blocked (ported rows elsewhere in the entry are listed in APPLIED)
- q11101 0x004585A1: `??_GCArmourDef@@UAEPAXI@Z` (Ego_d PUB32; vtable 0x01234AEC slot 0). Authoring: its ctor 0x00458573 is a
  separate strong row.
- q11109..q11111 (0x005BEAD5, 0x005BF30D, 0x005BE8D3, 0x005BE8B1, 0x005D8E70): names verified from two or three sources and
  recorded in NAME_FIXES.tsv. A port needs the PASS22 CCategory / NInventory_CTCInventoryBase.h chain, which is broken: form B gave
  SRC_FAIL, and CDefPointer.h needs DEF_CLASS_BASE. Drafts are in work/hdr_drafts/.
- q11114 0x0074ACE0: a 612-byte all-local model with about eight wrong bindings. NF84's shared-header form failed C2259.
- q11117: 12 `??_G` rows (above).
- q11119 0x005959AB: the port onto fable_deque.h (draft work/hdr_drafts/fable_deque_copyscan.h) binds all four callees correctly,
  but three forms grade DIFFER 87 vs 88 bytes. Retail keeps `_M_cur` in esi across the `back()` call.

## Other judgements
- 0x0059B0C3: access-only respell (`protected:` label) inside the row's pre-existing local deque model (in-place precedent). A full
  fable_deque.h port needs a `_M_reallocate_map` body opt-in (open).
- 0x008D6D40: CModeSleepBed has no shared header; the row keeps its local model on shared base types.
- 0x0096CFC0 / 0x009498B0: in-place respell of an in-row free-function declaration of NCreatureMode::GetUpFromSitting.
- 0x00812110: `??_GCHitParameters@@QAEPAXI@Z` (QAE, no virtual dtor). VC7.1 emits it for the delete expression in
  CCountedPointer<CHitParameters>::DeleteData, so 0x00812150 / 0x00812110 / 0x00812280 share one explicit-instantiation recipe.
- vland SHARED_HEADER_AVAILABLE warnings on 0x0059686D, 0x00597B20, 0x006644F0, 0x00928E20, 0x009498B0, 0x0094DC40, 0x009621F0,
  0x0096CFC0, 0x0096DDB0, 0x0097D8B0, 0x0097DA40, 0x0097DB70: each warned class was already modelled locally in the landed row
  (checked: no new local class definitions).
- NAME_FIXES.tsv holds 12 proven spellings for the coordinator's caller respell or label pass. Some are rows of this slice that
  stay blocked or not-a-binder (q11109..q11111 callees); they are listed so the name is fixed when those rows move.

## vland warnings in recheck_final.log (triaged)
- RELOC_NAME: each binding either matches the old row's probe (`work/xo_<addr>.txt`) at that offset and is inherited, or targets an
  address whose spelling this hand-back proves. Inherited: `??0CBrainOutput` 0x00835620 (7 rows), `HasMode` 0x0073A8A0,
  `IsSeatTaken` 0x007B8570, `TakeOver` 0x00669280, `__copy_trivial` 0x00405BA0, `_M_increment` 0x0059AAF9. Proven here:
  0x004508D6 and 0x00664670 (NAME_FIXES), and 0x004DA252 / 0x0059AFD7 / 0x0059B2CC / 0x00812150 (RENAMES section 1).
  0x00664670 is the CCountedPointer overload of SetCurrentAction (Ego_r 0x005429A0 / FableWin 0x01D94200; it tail-jumps to the
  reference overload 0x006644F0). It is a different address from 0x006644F0, so the two spellings do not conflict.
- STANDIN_LEAF on 0x004DA252, 0x0059B2CC, 0x0059B49B, 0x006DBFE0, 0x006DC1B0 and 0x006DC1F0 compares against the OLD catalog names,
  which RENAMES section 1 replaces. On 0x00B9C110 / 0x00B9C130 / 0x00BCA060 it is a false positive in standin_leaf.py: `cat_mc` only
  recognises "scalar deleting" with spaces, and these catalog names use underscores. The scaffolding does not cause it.
- Section 2 was checked line by line against the old probes: each `old_symbol` is bound at `callee_address` in the old row. The
  relocations of 0x0085FF10 (no section-2 line) are identical before and after.
