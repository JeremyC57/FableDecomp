# Landing nf327 (Account A, 2026-11-24): 10 rows unblocked by the NF321-NF325 rulings (11 headers, 7 of them new)

Hand-back: `pending/2026-11-24_nf327_handback/`; helpers under `naming_batch/agent_tools/nf327/`.
- **Entries:** 14. **Outcome:** 10 done, 4 skipped-held.
- Landed together with nf325, because 0x00A86030 needs nf325's CPhysicsFacesSegmentBranch.h.

## What landed
- **Six Clone rows on six new headers (NF323 Q1):** 0x007EBB30, 0x007ED140, 0x0084C010, 0x0084CA30, 0x0084CA60, 0x00933D50.
  - Each header is gen_class_headers.py's flat output as the default branch, with NF323's model under
    `FABLE_RETAIL_CCREATUREACTION_<X>_VIRTUAL_MODEL`.
  - Stripping the opt-in gives the generator's text back byte for byte.
  - cloneproof passes for all six: the emitted `??_7` matches retail at all 70 slots, and the `??_G` body equals retail's slot 0.
- **0x0088E520 EndCutFade (NF321 Q1):** NF321's CGameScriptInterface.h opt-in, with the repo header unchanged since NF321's copy.
  - Under the three-name map, every section, relocation, the DEF set and `.drectve` are equal. The only UND difference is `__fltused`.
  - Already-landed objects carry `__fltused`: 0x00A86030 MATCH and 0x006E71F0.
  - Rest's second source: the landed self row 0x006E71F0 plus both PDB name sets.
  - 2 table replaces (helperA -> EndFadeScreen, helperB -> Rest). ref_scan on both old spellings: NONE.
- **0x00902CA7 / 0x00902CDC ServeFood GetBuilding (NF325 Q2):**
  - a new CAIStateGroup_ServeFood.h in the opt-in form, with slots 14-16 `= 0`;
  - `_VIRTUAL_MODEL` opt-ins on ServeFoodAtHome.h and ServeFoodAtTavern.h;
  - the probe shows `__purecall` at slots 14-16, as in retail, and the derived tables match.
- **0x00A86030 GetPossibleCollisionSet (NF325 Q1):** an add-only `friend class CPhysicsFacesSegmentBranch;` under
  `FABLE_RETAIL_CIPHYSICSFACESSEGMENTBASE_FRIEND_BRANCH`. HEAD's helper class keeps its one inline accessor, so the COMDAT set is unchanged
  (NF327 Q3: acceptable).

## Proof
- **Per row:** precheck; vland HEAD and new under one work path; objsec under the renames; a /FORCE link with 0 duplicates; no other
  landed definer; no table line on the addresses.
- **equiv_check** over the include closure of the changed headers (1,709 rows): 0 differing. The planted control flags every includer
  that reads an edited region. NF325's full 57,146-row run at this landing confirms that the closure method finds everything the full
  run finds; the full run's only extra lines were path-only `__FILE__` differences.
- **At landing:** all 32 orig/ copies equal HEAD; 41 of 41 PASS (nf325 + nf327); every row grades its RENAMES spelling.

## Rulings on NF327's questions (coordinator)
- **Q1 (PickUpJug 0x009680D0):** held. The TU adds `??1` destructors and an out-of-line base destructor call that retail inlines (NF320
  Q2). It waits for a port of the destructor shape.
- **Q2 (0x00860290 with binders 0x0085FF10 / 0x00860150):** held as one unit. The binders are not ALL_EQ under the one rename, because
  the opt-in moves 5 slots of their own emitted vftable. Landing the Clone alone would strand the binders' references to the old spelling.
- **Q3:** keeping HEAD's existing helper class unchanged is acceptable; it is not new scaffolding.

## Queued
- 4 T3 lines `nf327`.
- NAME_FIXES `nf327` (0x0084C9A0).

## Recheck
`logs/recheck.log` (landed tree, 41 rows of nf325 + nf327): 41 PASS.
