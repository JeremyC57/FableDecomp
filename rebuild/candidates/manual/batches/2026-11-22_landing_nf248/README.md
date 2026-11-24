# Landing nf248 (Account A, 2026-11-22): seven of NF241's open CODEXP27 lines (10 rows)

Agent NF248 (hand-back early, context). Hand-back: `pending/2026-11-22_nf248_handback/` (APPLIED.tsv = per-line census and blocker for
the 29 lines still open, plus E2 0x0049D800 and E3 0x01296F14).
- **L2 0x00666310** (CThingCreatureBase::OnCreate, protected virtual `MAE`): binders 0x006A9EF0 / 0x00833A70 respelled in place in their
  row-local models (0x006A9EF0 also gains `protected: virtual ~CThing();`, `??1CThing@@MAE@XZ` in both PDBs, keeping the vptr at +0);
  2 -> 1; their own leaves QAE -> MAE (both PDBs; unreferenced). Table line replaced.
- **L23 / L24 0x00446B40 / 0x00446B90:** self rows onto CGamePlayerInterface.h `FABLE_RETAIL_CDRAWABILITYBUTTONS_WEAPONABILITIES_PASS27`
  (a file-static accessor keeps retail's re-read of end()); 2 -> 1 each.
- **L27 0x00497B60:** binder 0x00499A70 off the fable_boot.h model onto engine/CProgressDisplay.h, now calling the private `AAE` spelling
  (2 -> 1); its own leaf QAE -> UAE (both PDBs, vtable slot 5, unbound); test rewritten on the shared header. Table line deleted.
- **L34 0x004DDAB4:** self row onto CTCTargetingBase.h + CIntelligentPointer.h; catalog SetTargetedThing -> `SetPTargetedThing` (both PDBs);
  2 -> 1.
- **L36 0x005348E0:** made-up `Find_005348e0` -> explicit instantiation of STLport `std::__find<unsigned long*, unsigned long>` (MATCH;
  element type from CTable.h (Ego_r) and the landed binders); binders 0x005518E0 / 0x00551BC0 respelled in place; 3 -> 1; table line
  deleted. pk.py: their two HEAD-NEQ `Find` copies are now `__find` copies EQ 0x005348E0.
- **L117 0x00B09D50:** stand-in -> `std::_List_base<CEnginePrimitiveSpriteTrail::CSpriteTrailEntry>::clear` (shared); 2 -> 1.
- Files / Modules respelled for 0x004DDAB4 / 0x005348E0 / 0x00B09D50 (renamer). Link of the 10 objects: one LNK4006
  (RetailUI::ChildrenVector::Grow, 0x005518E0 v 0x00551BC0), both define it at HEAD: existing debt.
- **Recheck** (`logs/recheck.log`, landed tree): the 10 changed rows + the table witnesses / self rows 0x00666310, 0x00497B60,
  0x0049A0B0, 0x00531090 (14 rows): 1 MATCH + 13 RELOCATION_MATCH, all PASS; 0 RELOC_CONFLICT.

## Coordinator rulings on NF248's questions
1. 0x006A9EF0's added `protected: virtual ~CThing();` in its row-local CThing: accepted as an in-place respell (same class token, a
   PDB-spelled declaration, nothing new defined; the standing in-place ruling).
2. The leaf-only self renames 0x006A9EF0 / 0x00833A70 QAE -> MAE and 0x00499A70 QAE -> UAE: accepted (both PDBs; no landed referencer).
3. Catalog / file renames for 0x004DDAB4, 0x005348E0, 0x00B09D50: applied.
Rule slip (an L34 grade launched after a failed clang precheck; killed with killmine.sh): noted.
The 29 open lines + E2 / E3 pass to the next naming agent (NF250).
