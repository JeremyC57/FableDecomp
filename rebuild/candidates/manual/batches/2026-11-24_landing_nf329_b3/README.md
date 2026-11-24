# Landing nf329 b3 (Account A, 2026-11-24): 60 flat-header combat rows onto their PDB members on landed class models

Agent NF329 took NF324 Q1, the 396 flat-header ports. The advisor ruled the route wanted as its own pass (owner delegated). Hand-back:
`pending/2026-11-24_nf329_handback/` (NOTES.md first); tools in `naming_batch/agent_tools/nf329/` (flatten_pilot v5).
- **This batch:** 60 rows through the switch route on models that are already landed, with no header change:
  - 57 CCombatSequence rows on NF325's models;
  - 2 ServeFoodAt* rows on NF327's models;
  - 1 CPhysicsFacesSegmentBranch row.
- **Each row:** the flattened `__fastcall` self `Class_Method` becomes the RTTI-placed PDB member `Class::Method`.

## Proof
- **Agent, per row:**
  - HEAD v new graded at one work path;
  - objcheck ALL_EQ under the one rename, with `.drectve` equal or HEAD plus exactly `/DEFAULTLIB:"libcp"` (NF318 Q1);
  - MATCH / RELOCATION_MATCH with the unchanged landed test passing.
- **Agent, across all 301 rows:**
  - one link HEAD v new: 0 duplicates, equal unresolved sets;
  - new-name census: no other landed source defines a new name;
  - no reloc_bindings line targets these rows.
- **Coordinator:**
  - every `orig/` copy (300 rows and 118 headers) equals the repo;
  - `land_scripted.py` checked staleness, rechecked all 60 on the landed tree (`logs/recheck.log`, all good), and each graded symbol is
    the PDB symbol.

## Rulings on NF329's decisions (advisor decision, owner delegated)
- **D-a (accepted):** the 15 rows whose member sits on a new slot pass on the vftable probe. Each retail vftable slot holds the row's own
  address, and the header declares it at that slot; the coordinator re-read all 15 from Fable.exe.
- **D-b:** `land_scripted.py --release-t3 nf324` lifts the hold only for an address whose sole T3 line is its nf324 one. The lines of
  landed rows are closed: removed from the queue and kept in `T3_CLOSED.tsv`.
- **D-c (accepted):** the macro is `FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL`, the NF323 / NF325 form.
- **D-d (accepted for b1 / b2):** `FABLE_RETAIL_KRAKEN_ROAR` gates only CCombatActionBase.h's class model, and it has landed definers.
  The forward declarations use `class`, the key of their shared headers. Queued cleanup: a neutral `FABLE_RETAIL_CCOMBATACTIONBASE` guard
  widening (NF322 Q1 form).
- **D-e:** six of the CA headers also sit in parked pending overlays from Nov 3-13 (hand732, handH40i, `2026-11-13_acctB_parked`). Those
  overlays must rebase onto the landed headers.
- **D-f:** at b2, 0x008CA950's HEAD entry is rebuilt from the repo. Its diff is only the rename, the header move and the dropped
  byte-dump comment.
- **D-g (accepted):** the models keep the generator's `#pragma pack(push, 1)`. All 1,247 generated headers use it, with explicit pads and
  every offset and sizeof asserted, so the layout is unchanged. It is not authored packing.
