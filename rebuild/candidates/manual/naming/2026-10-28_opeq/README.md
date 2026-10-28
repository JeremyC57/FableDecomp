# Naming pass: CCharString / CWideString operator= (2026-10-28)

Retail (Ego_r.pdb, FableWin.pdb) declares every assignment operator of both classes public and returning `const T&`
(`??4CCharString@@QAEABV0@ABV0@@Z` 0x0099EFB0, `...ABV0@PBD@Z` 0x0099EFE0, `...ABV0@D@Z` 0x0099F040,
`??4CWideString@@QAEABV0@ABV0@@Z` 0x0099B7D0, `...ABV0@PB_W@Z` 0x0099B800, `...ABV0@_W@Z` 0x0099B860). Landed rows declared a
non-const `T&` (or once `T*`) return, which mangles a name retail never had.

`transform.py` (byte-neutral, idempotent) was applied to the 2,107 files in FILES.txt (1,114 sources, 992 tests,
`rebuild/include/fable_string.h`). Agent recheck (dry run, header overlay): 2,409 of 2,414 affected rows MATCH /
RELOCATION_MATCH with test PASS; the other 5 fail identically on the unchanged tree (PREEXISTING.tsv / EXCLUDED.tsv).
Coordinator recheck after applying in the repo: 84 of 84 sampled rows win (recheck_sample.log). Four stale bindings of the
old spellings removed (BINDINGS.tsv; the new spellings were already bound to the same targets).

Left for later (BLOCKED.tsv, 351 declarations): rows without `/Zc:wchar_t` (wide overloads cannot reach `_W`), local
models declared `struct` or inside a namespace, and a few test-only fakes. NAME_FIXES.tsv lists the other wrong spellings
bound to these six addresses (placeholders, the 0x0099EFB0 row's own fastcall stand-in, the AssignFromWide label).
