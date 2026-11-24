# NF344 helpers (2026-11-24)
- pre.sh <src>: clang MSVC-target syntax precheck (NOWCHAR=1 for VC7.1 wchar_t). zs.py / zsrun.sh: VC7.1 cl /Zs fallback (D1), own timeout 900 per file.
- flagrun.py <addr> <variant> <CAT|NOD|"flags"> <out.jsonl>: compile a landed row as recheck/vland do under a /d2inl variant at one fixed path;
  pk_all.check (+ NF340 flagged-run addresses as source o) and pickany_vs_retail.py; keeps the object in work/objs/.
- gatecmp.py <jsonl>...: NF340 gate per variant against the catalog build (BROKE / REPAIR / DEBT / other, both sides, leafref or reach).
- objfn.py <head.obj> <new.obj>: per-function bytes + relocation diff of two objects (no-op flag test).
- findcopy.py <obj> <symbol-substring>: masked search of Fable.exe code for a copy's body (finds NOADDR copies' retail form; prints VAs only).
- mkflagtsv.py: FLAG_CHANGES.tsv from flagrun results.
- census_leaf.py <out.tsv> [summary]: item 2, landed Leaf (or self_symbols graded symbol) bound in reloc_bindings.tsv to another address;
  ?A0x-stripped, ??_E->??_G, prefix leaves only when unique; capstone relocation-masked body compare of the two addresses.
