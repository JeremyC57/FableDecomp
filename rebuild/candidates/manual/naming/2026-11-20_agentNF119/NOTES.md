# NF119 notes (round 10, PRIORITY slice 5 remainder; early hand-back)

Counts (from STATUS.tsv / APPLIED.tsv): 52 rows listed: 5 ported, 32 not-a-binder, 14 blocked (port), 1 deferred (0x007940F0, held by NF120).
19 entries: 1 done (q4497), 8 blocked (q3885, q4491, q4495, q4742, q6158, q6371, q6406, q6407), 10 open (q3864, the CTCCarrying family
q4469..q4473 + q4984, q6231, q6232, q6393). Final recheck: recheck_final.log, one vland run over the 5 ported rows: 5 RELOCATION_MATCH, all PASS.
No overlay headers (hdr/README.md). No new made-up names (the scaffolding helpers are anonymous-namespace, TU-internal, descriptive).

## Ported
- 0x0065F6A6 count_if<list<CIntelligentPointer<CThingAICreature> >::iterator, HasGuardProperty> (q4497): self leaf YGJ local model ->
  STLport YIH (the spelling its only caller 0x00659800 binds); binds 0x0065F152 and the 0x0050F410 copy ctor (both already bound).
  Explicit instantiation hits C2785 (NF115's 3 forms + explicit template arguments here), so the instance is emitted by the nf86/nf88
  address-taking scaffolding (`// Fidelity: scaffolding`). vland prints a non-blocking WARN STANDIN_LEAF (catalog name in text form).
- 0x008B7B10 CCreatureAction_SheatheWeapons::PerformAction (q6158): local CThing model -> shared headers; MakeNoise and
  SheatheAllWeapons (0x006647F0) now bind the CThingCreatureBase spellings (both already bound elsewhere). Leaf line added.
- 0x0078EBF0 CTCDivineWrath::PlaySoundW (q6158, claimed): all-local -> engine/CTCDivineWrath.h; self leaf QAEKKK -> the PDB private
  ?PlaySoundW@CTCDivineWrath@@AAEKABVCCharString@@_N@Z (no landed referencer, exe scan); 0x0066BD00 now bound as
  CTCSoundPlayer::PlayAttachedCriteria (was the CTCSound::PlaySound stand-in). Identity of 0x0078EBF0 vs its byte twin 0x007940F0: block
  position and the twin's CTCUnholyPower binding (RENAMES evidence); 0x007940F0's catalog name is wrong (NAME_FIXES).
- 0x0075F980 CCardinalSpline::AppendControlPoint (q6407, claimed): local CArray __true_type model -> the shared CCardinalSpline.h body
  (APPENDCONTROLPOINT opt-in) emitted through the scaffolding, overflow declaration only (VECTOR_OVERFLOW_DECL_ONLY, form 2; form 1 with
  OVERFLOW_OUTOFLINE also matched but emitted a _M_insert_overflow body COMDAT); binds 0x0066E370 under its self row's IAE __false_type spelling.
- 0x0066E370 vector<C3DVector>::_M_insert_overflow (q6407): truncated `// Leaf:` line completed (object unchanged).

## Flags lines
- 0x0065F6A6: `// Flags: /Gr` (its build_candidates entry has no CompilerFlags: add `CompilerFlags = '/Gr'` at landing, GOTCHAS catalog-flags line).

## Blocked rows and the classes that must move first (0 grades each unless STATUS says otherwise)
- Carried from NF115: 0x0090C1A0, 0x006CDA00, 0x0093DB10, 0x006D50C0, 0x007BB660, 0x00692960, 0x00856A20, 0x008C0FB0 (STATUS has NF115's lists).
- 0x007CC4D0 CTCMultiArrow::Init: local CThing / CTCMultiArrow / CTCInventoryAbilities / CTCSoundPlayer / CMainGameComponent /
  CDefStringTable / CVectorMap specialization / CIntelligentPointer; it is a q6231/q6232 REFS row: port with that family.
- 0x00858130 / 0x00858400 / 0x00858590 (CCombatActionBase::ProcessPossible*StrikeEvent) and 0x00951B20 (JackDragon FlyOutOfGround):
  CVector<C3DVector> / CVector3DList stand-ins inside 13-17-declaration local models; renaming the stand-in to std::vector would be a new
  local model. 0x00858130 / 0x00858400 are also q3864 binders (HasPerformedAction stand-ins): port them once for both entries.
- 0x00A887B0 CSBCCorridor::CreateLineList: local std::vector that DEFINES a noinline stand-in body under
  ?_M_insert_overflow@...@QAE...ABV__true_type@@ (not retail's spelling, so it cannot clash at link, but it is debt); needs a CSBCCorridor
  class opt-in (engine/CSBCCorridor.h is a generated flat struct; RTTI CSBConstraint / CSBInput / CSB), CWall, list<CWall>.
- Deferred: 0x007940F0 CTCUnholyPower::PlaySoundW (held by NF120) binds ?MakeNoise@CThing@@: the 0x0078EBF0 port is its recipe.

## Splits
- None created. Reduced: 0x0065F152 (the $$CBVCThing spelling gone: its two REFS rows 0065f6a6 / 0065f9ab now both bind the claim),
  0x0050F410 (0065f6a6 no longer binds the converting-ctor spelling; the other exe-scan referencers 0090d030 / 0090e5d0 not probed),
  0x0065F6A6 (YGJ gone; exe scan: its only referencer is 0x00659800, which binds YIH),
  0x00662110 (4 -> 2 rows on ?MakeNoise@CThing@@), 0x006647F0 (one CThing::SheatheAllWeapons binder fewer), 0x0066BD00 (one binder of the
  CTCSound::PlaySound stand-in fewer; 0x007940F0, held by NF120, still binds it), 0x0066E370 (the one CArray __true_type binder gone).
- Seen, not touched: data 0x0129D25C carries three spellings (?g_InvalidSoundHandle@@3KA from 0x0079CE80 / 0x0078EBF0,
  ?g_invalidSampleID@@3KB from 0x006CF040, ?g_InvalidSoundHandle_0129D25C@@3JA from 0x008DF680); all descriptive, no PDB data symbol checked.

## Open entries (not started; triage done)
- CTCCarrying family (q4469..q4473, q4984): 52 REFS rows (work/carry_refs.txt), 25 of them local models, 7 with no Leaf line; none in
  UNMATCHED_ROWS as Account B's. engine/CTCCarrying.h already declares the claimed spellings (REMOVETHING / CARRYSLOT / CTCCARRYING opt-ins)
  except IsCarriedThingInPrimarySlot, and each claim is already bound by a landed row, so per-row respells only shrink splits; the six
  self rows are local CTCCarrying models whose own leaves need the PDB spellings.
- q3864 (0x006925A0, 8 stand-in spellings), q6393 (0x00662A00, 6 spellings over 24 rows), q6231 / q6232 (93 shared rows): not started.

## Rule slips
- Many Bash calls started with `D=...` / `cd` instead of the lane line (`export S=...; source $S/wine/env.sh; ...`); every compile call did
  source the lane.
- One `mkdir $S/w22/nflocks/0075f980` ran before S was exported (it failed silently: /w22 does not exist; the claim was then made correctly).
- The first vl.sh would have passed `none` from a `// Flags: none` line as a compiler flag (no row hit it; fixed in work/vl.sh).
- One compile was re-run to read a truncated error (0x008B7B10 C2039), counted in grades_used (5).
- First drafts of NOTES / STATUS over-stated two split reductions (0x0066BD00, 0x0050F410) and named 0x007940F0's class unverified;
  corrected after review (now from reloc_bindings + exe scan).
