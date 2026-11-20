# 0x00AD9980 CVertexGroupBlockOptimiser::ReorderInput (was catalogued CVertexGroupBlockList::FilterBySubset)

**Grade:** RELOCATION_MATCH (416/416). No flag, no pragma. Regenerated on alt_v7 (STLP_TREE_CONSTFIND_NOINLINE), per main's ruling.
- **Strict vland:** passes. LANDVERIFY_STRICT_HEADERS=1, empty overlay `hdr/`, fresh LANDVERIFY_WORK (`vland.log`), no warnings.
- **Payload:** module CVertexGroupBlockOptimiser, name ReorderInput.
- **Behaviour test:** passes on the real object. 6 runs check the exact output order and contents.
- **Mutants:** 11/11 KILLED (`mutants/00ad9980.out`).
  - One mutant is behaviour-neutral by design and SURVIVES: the order of the two local declarations (`.dead.out`). Its bytes differ; see
    the table below.
- **Fidelity copy:** `fidcheck` reports 1 copy checked, 0 differ. `$S/fidcheck.py`'s logic was run on this file through
  `tools/fidcheck_file.py`: the IsSubsetOf copy is the landed 0x00AD8C30 body verbatim.
- **COMDAT:** the row object's `?IsSubsetOf@CVertexGroupBlock@@QBE_NABV1@@Z` is 108 B and byte-equal to retail 0x00AD8C30, relocations
  only (cmpcomdat.py).
- **Header changes:** none. `hdr/` and `hdr.diff` are empty, and newly_flagged_rows.tsv has the header line only.
- **Oracle length:** 416, not the draft's "414 + padding". The function ends with `add esp,2Ch ; ret 4`.

## Name (NAME_FIXES.tsv)
`?ReorderInput@CVertexGroupBlockOptimiser@@QAEXABVCVertexGroupBlockList@@@Z`.
- **Ego_r.pdb:** 0x00C5D290.
- **FableWin.pdb:** lib_vertex_group_block.obj #25 (462 B), in the same object as IsSubsetOf (#9) and RemoveAllSubsets (#18 = landed
  0x00AD9770).
- **Ego_d.pdb:** the proc has the locals solution, input_size, remaining_input, new_input_list, c_order and c_input.
- **FilterBySubset:** an autoname, found in no PDB.
- **Layout:** InputList is the optimiser's +0 member. Retail has no direct caller.

## What changed against the draft (pending/2026-11-11_linux_handQ8/differ/00ad9980.cpp, DIFFER(415v416))
- **Headers:** the shared headers replace the local models (engine/CArray.h, stlport/fable_map.h, engine/CVertexGroupBlock.h, with
  existing opt-ins only).
- **Class models:** CVertexGroupBlockList is local, with its Ego_d inline members, as in the landed RemoveAllSubsets.
  CVertexGroupBlockOptimiser has the landed AddBlocks 0x00AD9F10 member list.
- **IsSubsetOf:** a same-TU body (landed 0x00AD8C30, verbatim) under the `// Fidelity:` line.
- **Locals:** new_input_list is declared first, marked `// Inferred:` and citing 0x00AD99B6-BE before the ctor call at 0x00AD99C7, and the
  bool array freed at 0x00AD9AEE/EF before 0x00AD9B04 / 0x00AD9B0E.

## Ablation (each item removed singly from the final source; `evidence/ablations/results.txt` also gives the IsSubsetOf COMDAT result per variant)
| change | row grade | IsSubsetOf COMDAT vs retail 0x00AD8C30 |
|---|---|---|
| final source | RELOCATION_MATCH | 108 B, SAME (relocations only) |
| no STLP_TREE_CONSTFIND_NOINLINE (find opaque) | DIFFER(410v416) | 130 B, DIFF |
| plain STLP_TREE_CONSTFIND in its place (`evidence/alt_plain_constfind_v6.cpp`) | RELOCATION_MATCH | 160 B, DIFF: find is expanded, while retail calls 0x006699C0 inside 0x00AD8C30 |
| no STLP_MAP_COPY_OUTOFLINE (map copy expanded instead of the call at 0x00AD9A4F) | DIFFER(416v416) | SAME |
| no STLP_DESTROY_AUX_FASTCALL_DECL (the range destroy loop expanded instead of the call at 0x00AD9B04) | DIFFER(422v416) | SAME |
| no STLP_VECTOR_C (no vector::operator=: the assignment at 0x00AD9AE1 becomes a word copy) | DIFFER(416v416) | SAME |
| IsSubsetOf declaration-only (no same-TU body) | DIFFER(410v416) | none |
| Ego_d local order (remaining_input first) | DIFFER(416v416) | SAME |

- **Line 2:** names the four opt-ins with these grades.
- **Dropped:** three byte-neutral defines are gone from the source: FABLE_RETAIL_CARRAY, STLP_LIST and STLP_VECTOR_COPY_OUTOFLINE (each
  was RELOCATION_MATCH when removed).

## Side check (main's non-blocking ask)
I regraded a scratch copy of landed 0x00AD9770 RemoveAllSubsets with STLP_TREE_CONSTFIND swapped for _NOINLINE (`$W/v/00ad9770/`).
- **Row:** still RELOCATION_MATCH.
- **IsProperSubsetOf COMDAT:** goes from 160 B DIFF to 108 B, byte-equal to retail 0x00AD8CA0 (relocations only). The landed define
  gives the 160 B DIFF.
