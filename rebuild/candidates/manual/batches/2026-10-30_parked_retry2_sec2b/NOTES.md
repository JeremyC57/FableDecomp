# Retry wave 2, section 2b -- notes (resumed agent)

The first sec2b agent finished 57 rows (STATUS.tsv lines 1-57) before a container restart. This file covers the resumed run.

## Wins (resumed run)
- 00717710 NTCScriptedControl::CActionTalkBase::OnActivate: RELOCATION_MATCH. Lever: PDB inline accessor
  (FableWin ?GetParentCreature@CActionBase@CTCScriptedControl@@IAEAAVCThingCreatureBase@@XZ) for the CModeConversation
  argument instead of *PTCScript->ParentThing. Names fixed from the parked source: 0x0085F310 is NCreatureMode::IsSitting
  (landed row + Ego_r), 0x007338A0 is CTCCombat::UnsetBlocking (catalog + landed binding in 0x00733F70), CThing is a class.
- 007197c0 GetAllThingsInRoughArea<CThingCreatureBase, vector<CThingCreatureBase*>, CThingFilter_None>: RELOCATION_MATCH;
  006f37b0 (<CThingBuilding, vector<const CThing*>>) and 006f3570 (<CThingObject, vector<const CThing*>>) also
  RELOCATION_MATCH but in name_review/ (PDB also lists const-T instances that would fold to the same bytes). Lever: the
  "lane gap" SIB operand order ([ecx+eax+4]) is reproduced by caller-hoisted byte offsets level*sizeof(...) computed inside a
  guarded y-loop and passed to GetCellPtr(cellsOffset, offset, x, y) -- the shape the ForEachInRoughArea twin 0x006CE800
  already used. Script: mapcell.py.

## Final state at hand-back (02:25 UTC, 8h lane limit)
STATUS.tsv covers all 337 WORKLIST rows (+2 extra rows 00698a30 / 006bb390):
12 matched in src/ (dry vland WINS 12, no RELOC_IDENTITY), 4 byte-matching rows in name_review/, 2 PENDING_HEADER,
53 IMPROVED (differ/), 183 HELD, 85 NOT_ATTEMPTED_TIME (the lane stopped at the time limit; residue in their STATUS lines).

### Matches (src/ + test/, dry vland MATCH/RELOCATION_MATCH + PASS)
006e2e60 00717710 007197c0 0072abd0 00736760 00751da0 00752300 0075fe30 006d7080 006aff40 006b0020 0077fde0

### Per-lever win tally (match = M, improvement = I)
- Stepwise float sums in permuted order (`r = X*v.X; r += Z*v.Z; r += Y*v.Y;`): 1 M (0075fe30), 4 I (006e8ce0 00726160 00753630 00760020)
- Named float result local for a difference (`float result = a - b; return result;`, keeps the 2nd operand on x87, fsubr join): 2 M (006aff40 006b0020)
- Named const references to the operands of a subtraction: 1 M (0075fe30, with the stepwise dot)
- reverse_iterator post-increment returning a copy (`attack = *attackIt++;`): 1 M (006d7080)
- Over-span re-grade over the true extent (BOUNDS_FIX.tsv + oracle_extra.tsv): 1 M (0077fde0)
- PDB inline accessor instead of a hand-walked member chain: 1 M (00717710)
- Caller-hoisted byte offsets for the map-cell SIB order (mapcell.py): 1 M + 2 name_review (007197c0; 006f37b0 006f3570)
- IS_THING_ALIVE template / `const CWorld& world = PeekWorld()` / Z,Y,X stepwise squares (earlier run): 00751da0 00752300 006e2e60 0072abd0 00736760 (M)
- __forceinline on a small inline the lane emits out of line (HasTC, a counted-release dtor): 2 I (006ddd30, 0065a41c)
- Block-scoping an out-parameter iterator so its frame word is freed: 1 I (00733cf0)
- Decrement the count before the swap copy (`--count; v[i] = v[count];`): 1 I (0074a4e0)
- Named locals for a debug line's end point / colour before a primitive temporary: 1 I (006992a0)
- `X* p = 0; if (HasTC) p = ...;` instead of a ternary: 1 I (00663990)
- Unnamed tag temporary as a call argument (`__find(.., random_access_iterator_tag())`): 1 I (006e3580)
- Virtual returning a non-POD CDefString by value (hidden pointer, no dtor) read inline as `.m_index`: 1 I (006ea0c0)
- By-value member operator+ called as (*b + *a): 1 I (007622c0)
- Named `unsigned char& flags = Flags178; flags &= ~1;` (RMW and byte ptr): 1 I (006a9690)
- Named mesh-bank pointer for a call inside an `&&` branch: 2 I (00680050 00670d50); `c_str()` as `m_pData == 0 ? "" : m_pData->m_str`: 1 I (00680050)
- `""` literal passed to a const CCharString& parameter: 1 I (0074ec50)
- Named `CWorld& world = *GetWorld();` for a vcall: 1 I (00722740)
- Bool local for a flag byte: 1 I (006ab0e0); joined `||` early rejects: 1 I (006c8e90); `place[i]` off a begin() base: 1 I (00774c60)
- Hoisting a member into a local before a loop: 1 I (0067e290); const reciprocal + named result: 1 I (006d63f0)
- Float-cast fabs delta local: 1 I (00754d80); channel copy reused: 1 I (0077ba40); removing an inner block scope: 1 I (00650c1d)
- `const __int64&` key reference: 1 I (006ecea0); side local before a call: 1 I (006f2c00)
- CCharString temporary as an argument + in-place division: 1 I (0065a8ee); named CIntelligentPointer& to Targets[i]: 1 I (0064c869)
- Earlier-run differ rows (0070bf50 .. 00744b50 in STATUS order) carry their own levers in STATUS.tsv.

### Descriptive (not PDB-proven) names in landed/differ sources
- CTCPhysicsBase vslot 73 `GetOldRHSet` (006aff40, 006b0020): slot 72 is Ego_r ?GetLastRHSet@CTCPhysicsBase@@UBEABVCRightHandedSet@@XZ, slot 73 has no PDB spelling found.
- 006ea0c0 slot 0x28 `GetDefName` on the info+0x64 object (differ only).
