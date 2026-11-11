# Placeholder-name families (2026-11-11)

The user, 2026-11-11: "separate the renames by family to make it quicker, then launch naming agents 1 family each time".
`FAMILIES.tsv` counts the landed catalog rows whose name still carries a stand-in (an address suffix, `vfunc_N`, `Unproven`, `sub_` /
`FUN_`, `thunk_`), grouped by family: 11,248 rows in 1,703 families, 59 families of 20+ rows covering 8,340. Compiler funclets
(`Unwind@`, atexit thunks) are excluded: retail has no names for them. A pattern count -- some address-suffixed names are honest
descriptive names, and some wrong names look normal.
One naming agent works one family at a time, largest first: FAM01 = `_global::CCountedPointer_*` (795, CCountedPointer<T>::ReleaseData
and friends), then CVtable_<addr> classes (765), sub_ / FUN_ (718), CIVCountedPointer (686), template instances with CUnproven* type
arguments (514), std:: with stand-in arguments (470), `_static_init` (398), StdList (389) ...
