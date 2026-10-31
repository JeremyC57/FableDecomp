# Chunk 621 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- PDB traits layouts are load-bearing. `CPersistTraits<std::pair<long,long> >` is 8 bytes (DefaultVal at +0, Ego_r), and retail
  transfers the map value IN the traits object: `valueTraits.DefaultVal.first = 0; ...second = 0;` (one zero register, reused
  later), then old entry / default into DefaultVal, then `valueTraits.TransferIn(this, &parser, &valueTraits.DefaultVal)`.
  `this` == &value; an empty traits stand-in lands 4 bytes away (0x00756D00, 1116v1116 -> RELOCATION_MATCH). Retail case 1 has no
  stores on its traits object; the real zeroing `pair()` is fine there, because VC7.1 drops the dead stores. No trivial stand-in needed.
- Respelling a matched stand-in to real STLport types can move register allocation. In 0x00756D00 the real `_Rb_tree_iterator`
  in the save loop was reloaded from memory across calls (1124v1116; `value_type&` 1124, key/value references 1120). A
  non-escaping copy, `iterator entry = it;` with `entry->first` / `entry->second`, keeps the node in esi and restores
  RELOCATION_MATCH.
- An STLport 4.6 `map::find` call site that pushes one argument and compares eax with the header is the private const
  `_Rb_tree::_M_find` (find is inline and wraps it). Declare `_M_find` out of line and keep `find` inline.
- x87 row sums: VC7.1 reassociates `a*b + c*d + e*f`, so the same plain 3x4 multiply compiles to different term orders in
  different functions (the 0x00A8A220 and 0x00A8A5F0 twins differ). Sequenced `float s = t1; s += t2; s += t3; out = s;`
  pins the term order exactly. It does NOT pin the fld/fmul operand order inside a product: swapping every product's
  operands compiled to identical code (0x00A8A220). Product operand order is a separate, canonicalised axis.
- Mechanical transcription helps. A symbolic x87 trace of the retail disassembly (stack simulation; reset the esp adjustment at
  calls, count pushes/pops) gives each store's exact expression tree, which a script then turns into sequenced sums. The
  scratch tools are $S/w22/agent621/x87sim.py and gen3x4.py; worth a lane_tools copy.
- A compiler-generated destructor (vptr-free, as retail's out-of-line ~CStringParser 0x004047A0) is not inlined at any of
  several early-return sites in VC7.1 (0x0088B0C0), not even when the TU first emits it through an explicit-call helper. A user
  `~T() { }` is inlined at the first site but adds the vptr store. Retail's one inline, vptr-free expansion is still unreproduced.
- Pragma/flag sweeps do not move the GetSeat-style "`mov al,1` before vs after pop edi/esi/ebp" epilogue order
  (optimize("t") identical, "s" and /Oa far worse; 0x009482A0).

Review at landing:
- 0x00756D00 is respelled: the real template member TransferMap<std::pair<long,long>, CCharString> over std::map, every callee in its
  PDB spelling, still RELOCATION_MATCH + vland PASS. NAME_FIXES.tsv has 9 callee rows (PersistError, SetAsFoundItem, CleanOutLine,
  SkipPastWholeString, std::list<CStringParserCommentBoundaries>::~list, the CCharString and pair<long,long> traits). Respell their
  landed callers 0x00463334 / 0x00475026 / 0x0047550F / 0x007AE3D0 / 0x009BBA40 and the landed own symbols of 0x009BAA90,
  0x009BAB40, 0x009B9C00, 0x0040E510, 0x0040E630 and 0x00755A30. 0x00755F40 is ICF-shared (review). The long helpers 0x009E2590 /
  0x009E2560 keep the landed __fastcall spellings (the PDB names CPersistTraits<long> members, which no member spelling reproduces).
- NAME_FIXES.tsv: the landed callers 0x0063DB30 and 0x0063D960 spell the cell tests public/struct (`QBE`, `PBUC3DVector`); the PDB
  has protected const + class (`IBE`, `PBVC3DVector`). 0x0063D4A0 is FindThemeCollisionPointWithinCell, not the catalog's
  FindGround.
- Parked sources carry symbol debt. 00972f30 uses the stand-in class `CAIStateGroupJackOfBladesStage2Methods` (retail
  `?SetupArena@CAIStateGroup_JackOfBladesStage2@@MAEXXZ`). 009289b0 declares `long Act()` public (retail
  `MAE?AW4EAIActionReturn@@XZ`). 00a8a220 / 00a8a5f0 / 00aeeba0 / 009482a0 declare non-virtual QAE/QBE members over struct
  stand-ins. 00a8a220 and 00a8a5f0 now carry their full mangled leaves (the overloads share the leaf substring).
- No row needed more than 4 grades. Five rows were retested with 1-3 new grades each. The remaining residues are x87 operand
  canonicalisation (3 rows), register coalescing (2), scheduling (2 twins), frame-slot order (1) and dtor inlining (1).
