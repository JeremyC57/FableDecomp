# How VC7.1 c2 orders commutative operands (Account B, hand3c3 author, 2026-11-20)

Measured inside c2.dll 13.10.3077 while closing 0x00B075D0 CPSCSingleSprite::AddToBoundingSphere (one swapped x87 pair). Extract of the
batch README; the kit in tools_il/ runs under rebuild/candidates/manual/pending/2026-11-19_authoring_handoff/hand3c/tools/sched/host.py
(set W to a workspace holding tools/sched and tools/il).


Read out of c2.dll 13.10.3077 with the hand3c c2dbg harness (tools/il/, see "Tools"):
1. The order is fixed in c2's per-statement tree optimiser, before lowering and register allocation. Its node canonicaliser
   (0x1075CC68) gives every operand a key (0x1075D388: `depth << 24 | size << 16 | hash16`) and merge-sorts the operand list of each
   add / mul node by key, descending (0x1076A2F3 with the comparator 0x1076A394).
2. hash16 (0x1075D54E) of a frame symbol is its symbol number << 5 (temporaries << 6); of a node, the sum of its operands' low key
   words, the i-th shifted left by i, plus the opcode. So `E * v` loads the higher-numbered symbol first, and a sum of products
   `E * v` (E > v) is ordered by E + 2v.
3. Symbol numbers (0x1075A0C5) come from per-kind pools with free lists; each pool takes fresh numbers 32 at a time (0x107603DB,
   counter 0x108A79D0). Locals, the locals / parameters of every inlined callee and the field symbols of frame aggregates
   (pt.Y, mat.E32, ...) share one pool. The field symbols are made by the tree optimiser in IL order, one transient with each.
4. Retail's loop order needs E22 - E32 > 2 * (pt.Z - pt.Y) in symbol numbers: E32 must be the last number of a 32-block and E22 the
   first of the pool's next block. Our text had E32 = #378; five more pool symbols before it were needed (mod 32).

Those five came from writing three expanded helpers the way the debug builds (and STLport) have them -- none was chosen for the count:
- `std::list::size()` in STLport's own spelling, `size_type __result = distance(begin(), end()); return __result;`
  (existing opt-in FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE; the default header body is a hand-written loop): +3.
- CGraphicDataBank::PeekGraphic as Ego_d's line table has it (lines 552..559: test 19 bytes, `return &GraphicList[index];` 20,
  `return NULL;` 2) instead of the one-return conditional: +1 (the second return's result symbol).
- CParticleUpdateParamDesc::GetUpdateParam as Ego_d has it (static const MaxRecursion; line table 126..133: `if (nested <
  MaxRecursion)` 11 bytes, the forwarding return 55 bytes = with `nested + 1`, `return false` 2) instead of a flat forwarder; the
  row then passes nested = 0 and the folded body pushes 1, as retail does at 0x00B0771F: +1. The limit 10 and the `+ 1` are also
  what the landed out-of-line caller 0x00AF3080 shows (`depth < 10`, `depth + 1`).
With all three, E32 = #383 and E22 = #480 (tools/il/sym.sh prints `480*370 383*372` for the loop's Y row) and the row matches.
Each alone or any two leave the swapped pair (+3, +1, +1 are additive; measured). The Ego_d-shaped const CVectorMap::Find
(`found = LowerBound(val); if (found != end() && !Compare(val, found->first)) return found; return end();`, line table 64..71) was
measured too: it changes nothing here (the row matches with or without it), so the shared header is left alone.

Consequence for landing: this row's match depends on the number of locals / parameters / return symbols of every helper it expands.
A later change to one of those shared helper texts can flip 0x00B07676 without touching the row -- recheck this row when
fable_containers.h (list), CMatrix3x4.h (TransformPoint), CParticleSystem.h, CPSCBase.h, CVectorMap.h, CArray.h, CGraphicInfo.h,

## Kit
literal nested = 0 folds away (not in the row's code).

## Tools left for the next author (W/tools/il, run under tools/sched/host.py; nothing in the toolchain is modified)
cdis.py (c2.dll disassembly with a call-target function index), dump2.py + pp2.py (the pre-lowering IL of one function with symbol
names), fm.py / sym.sh / sv.sh (symbol numbers and sort hashes of the float products for a source variant, 15 s, no grade),
symlog.py + lpool.py (every symbol allocation with its name and number), wp1.py (hardware write watchpoints inside c2).

## NAME_FIXES / LABELS
NAME_FIXES.tsv repeats one earlier record that vland still warns about: 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (labelled
?g_NormaliseEpsilon@@3MB; the other vland WARN, __CIfmod -> the MSVCR71 import thunk, is the expected alias). Nothing new (0x00AF2250 GetSystemTransform, 0x00ADDD80 AddToBounds, 0x00AEA9F0 GetTickRate, 0x00AEAAA0 GetGraphicBank,
0x00AF2C50 LowerBound, 0x00AF8F00 ScaleParticleDimensions are bound under their PDB names).
