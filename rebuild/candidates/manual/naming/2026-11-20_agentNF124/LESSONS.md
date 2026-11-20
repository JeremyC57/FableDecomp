# NF124 lessons (one line each)
- A row's local CModeBase moves onto the shared nested CModeBase in one or two grades when the row's other local models do not clash with fable_string.h; ParentThing becomes CThing& (`.` not `->`), and the derived override must be declared `virtual` to get the PDB UAE leaf.
- Each port respells every callee the local model declared: run pdiff.sh and check every new pair against reloc_bindings + the callee's self Leaf (tools/coll.py) before calling a row done.
- A port that inlines a header accessor (GFRandom, GetControlPointCount) can reorder statements the landed row had pinned: keep the landed statement order (seed pointer before the range) first.
- The CArray<C3DVector>::operator= binders hand-declare operator= on a local CArray: they are debt, not an nf104 fold; on CCardinalSpline.h the implicit copy binds vector<C3DVector>::operator= directly.
- A helper script that treats `-` as "read stdin" hangs a Bash call when `-` is meant as an empty field: use an explicit marker.
