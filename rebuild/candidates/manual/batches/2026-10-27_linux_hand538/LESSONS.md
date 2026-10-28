# Chunk 538 retry lessons

- **Rename before reshaping.** In 004a6550 the PDB-correct signatures closed a register swap that about 20 shape forms
  could not move. The signatures were references instead of pointers, `Get3DEngine()` returning `CIEngine&`, and the
  namespaced fastcall callees. The decisive change was writing the quest-manager allocation as one plain
  `new T(args)` inside the `Reset(...)` call, not as `operator new` + placement new + a null check. Spelled-out
  new-expressions are oracles, not sources.
- **Same-TU noinline bodies of the real out-of-line STL callees fix register and frame choices.** This worked for
  `_M_find`, `_M_insert_overflow`, `resize` and `GetAllThings`, each under its real name with
  `#pragma auto_inline(off)` + `__declspec(noinline)`. VC7.1 then sees which arguments are only read.
- **An inline function that returns a class by value builds it in the caller's return slot** (`lea`, not the ctor's
  eax). Use it for `GetLoadAllSectionName()` and for `CDefString` -> `CCharString`.
- **The order of the opening stores is a free variable.** Permute the first 4-6 independent statements, compile-only,
  before spending grades.
- **Conditional temporaries share one flag variable** (bit 1, bit 2 ...). Whether VC7.1 keeps it in a register depends
  on what else wants ebx. In 00502620 a `CMapInfo&` local for the first statement moved the flag back into ebx (561v559).
- **Correct names can cost bytes.** In 00502620 the PDB-exact forms (`*LevelWADBank` via `CCountedPointer::operator*`,
  the `_K`-returning static) cost 7 bytes against the parked misnamed source. Re-sweep after renaming, and never revert
  a name to win bytes.
- **Audit class keys.** `struct CCharString` mangles as `ABUCCharString`, but the PDB has `ABVCCharString`. Every
  PDB class in this chunk is V (class), so convert `struct X {` to `class X { public:` and give base classes an
  explicit `public`.
- **x64 test gotcha:** `long` is 8 bytes under g++. A test that `memcpy`s 4 bytes into a `long` reads garbage. Use
  `int` for 32-bit stream fields.

## Review at landing
- 004a6550 `g_pQuestManager` 0x013B89FC: here a `CCountedPointer<CQuestManager>` (retail thiscall Reset 0x004A9A10 on it);
  landed models declare `CQuestManager*`. Two mangled data names for one address -- pick one.
- STLport helper spellings in NAME_FIXES.tsv come from our objects (the PDBs carry Dinkumware names).
- `??4CCharString` / `??4CWideString` return `const X&` in both PDBs; most landed models spell a non-const return.
