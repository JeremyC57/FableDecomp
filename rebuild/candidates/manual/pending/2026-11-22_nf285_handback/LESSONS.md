# NF285 lessons (2026-11-22)

- Most T1 standin_alias entries past the CCountedPointer block are NOT in-place respells. The stand-in is usually a CLASS token, such as
  `_Retail` / `Methods` / `CBase_<addr>` / `CElementNN_<addr>` / `N_<addr>::`. A scope compare of each stand-in against `pdb_spelling`
  (tools/cls2.py) finds the few that are a real class token with a stand-in member name. Those go through in one batch: 12 binders,
  12 ALL_EQ.
- A `Methods` struct derived from a flat (data-only) header view can bind the real class's members once the flat struct DECLARES them
  under an add-only opt-in. A member's mangled name has no class key, so the flat `struct` still gives the PDB `QAE` spelling. This
  fails for private (AAE) and virtual (MAE / UAE) PDB members: a derived struct cannot call a private member, and a flat view cannot
  declare a virtual.
- Moving a callee from a pointer to a reference parameter, from non-const to const, or into private / protected access as the PDB says
  left every object byte-identical (objsec ALL_EQ). So did moving a float-argument callee from `__stdcall` to `__fastcall`.
- clang precheck v VC7.1: rows with `typedef unsigned short wchar` plus `L""` need `-Xclang -fno-wchar` (VC7.1's default
  `/Zc:wchar_t-`). Some landed tests cannot pass any clang precheck: VC7.1's own `<ostream>`, or an explicit specialization ordering.
  Precheck each entry's binder tests during triage and keep a "recheck withheld" list.
- The `reloc_bindings` witness hides second binders of a stand-in (0x00947840 for IsCookTime_00658bb9). A source grep of the stand-in
  identifier across src / tests / include finds them, and also finds declaration-only non-binders (0x004B4C50).
- equiv_check writes stub headers under `$S/stubinc_equiv`. Run it with `S` pointing inside your own folder. Its `addr-file` argument
  limits the run to the header's includers (seconds instead of 25 minutes). Build the "new" tree as a symlink farm with only the
  overlaid file copied, and never copy onto a symlink.
