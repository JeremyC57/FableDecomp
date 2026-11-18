# Naming evidence

- Ego_r donor 0x00C66B10 and FableWin donor 0x034C5F10 carry the exact `?Optimise@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXM@Z` leaf. Native 0x00ACDB2F calls it from the landed COptimisedMesh::Optimise. Ego_d places the primitive function in lib_3d_quadric_optimiser_internals.obj.
- Ego_d names `max_error` and the loop's `pair_itr`, `contraction_ptr`, and `contraction`. The retail head iterator lookup, ownership increments, null state and cost limit corroborate these local roles independently.
- The two same-TU bodies, `UpdateCost` and `PopFrontContraction`, retain the STLport multimap type. The first is the landed 0x00AC8A60 function. Ego_r donor 0x00C61530 and the two retail expansions prove `PopFrontContraction`, with `front` from Ego_d.
- `IsIllegal` at 0x00AC8360 is the exact Ego_r donor 0x00C61350 / FableWin donor 0x034C22C0 symbol. This retail routine calls CleanUp at its head and returns its tested bool to this row. The package manifest already includes the correction and its NAME_FIXES_QUEUE records the earlier incorrect name.
- The names of `DetectFoldOver` and `PerformContraction` come from both PDB tables and these exact landed retail neighbors. CRT and STLport relocations use the package bindings.

## Behavioral fixture

The fixture reproduces the selected method and both emitted helper bodies. Its counted-pointer wrapper refers to fixture-owned contractions, and its map uses the host STL. The checks cover stale, legal, illegal and repeatedly repriced queue entries. VC7.1 RTM /Od /W3 and GNU C++03 both passed.
