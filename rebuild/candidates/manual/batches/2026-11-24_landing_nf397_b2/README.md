# Landing nf397 b2: 0x007634E0's `__destroy_aux<CCountedPointer<CSubdivisionTriangle>*>` becomes pick-any (clears NF395's 0x00762440 blocker 1)

`engine/NShape_STLHelpers.h` defines a non-inline explicit specialization of `__destroy_aux`, so every includer emitted a NoDuplicates copy. That copy already clashed with the out-of-line owner 0x00764EE0 at HEAD. The new add-only, default-off opt-in `FABLE_RETAIL_NSHAPE_DESTROY_AUX_PICKANY` gives the same body as an `inline` (still noinline, fastcall) definition, with the original under `#else`. Only 007634e0 defines the macro (CPatchTesselation / UpdateSubdivisions, landed in Codex pass 26; that reservation is used up).
- **Section comparison (vland objects):** exactly one change, that COMDAT's selection going NoDuplicates -> Any. Bytes and relocations are equal, and the copy equals 00764ee0's. That is R1 tier 1 / R10: 00764ee0 is the shared-header owner.
- **Gates:** vland HEAD and new RELOCATION_MATCH; pk_all with 0 NEQ, the copy still EQ at 0x00764EE0.
- **Link duplicates:**

  | Link set | Before -> after |
  |---|---|
  | 007634e0 first | 2 -> 0 |
  | 007634e0 last | 1 -> 0 |
  | with NF395's 00762440 respell | 1 -> 0 |

  Unresolved is unchanged. NF397's whole-catalog equiv_check differs only in path artifacts (`../src` includes, `__FILE__`).
- **Coordinator checks:** landed 1 of 1; the self is unchanged; the header's orig copy equals the repo; baselines as in b1.
- **Follow-ups:** T3 nf397 0x00763060 (strong unreferenced copies; ruling Q1 needed) and 0x00764BC0 (the `__copy` specialization once 00764bc0 is respelt).
