# Vendor-library landing pass: lessons

1. **Find callers from relocation pairs, not from `reloc_bindings.tsv`.** The table records a symbol once, with its first
   caller. A stand-in spelling first recorded elsewhere, or never recorded, does not show its other callers. The first
   bindings-driven pass found 75 caller rows. A `reloc_identity.check` scan over all 15,999 landed rows found 50 more:
   19 `free` stand-ins, 25 `operator delete` rows (copies of one `operator_new` template), a float `floor` misnamed
   `ceil` plus its 4 callers, and `terminate_` in `__ArrayUnwind`.
2. **Stand-in callees still RELOCATION_MATCH.** A caller that links `_my_free` where retail calls the `free` thunk
   grades RELOCATION_MATCH. Only the bindings and label checks see the wrong name, so wrong spellings build up quietly.
3. **Declare CRT callees `extern "C"` without `dllimport`.** A C++ declaration (`int __cdecl rand();`) links
   `?rand@@YAHXZ`. A `dllimport` declaration emits `call [__imp__rand]`, which is a different instruction.
4. **An invented callee right before `__ftol2` is the compiler's float-to-int cast.** `Finalize` and `sub_bfea70` were
   both this. The real callee returns `float`, so fix its return type and let the cast emit `__ftol2`.
5. **VC7.1 predeclares only the scalar `operator new` / `operator delete`.** `::operator new[]` and
   `::operator delete[]` need an explicit declaration, or the row fails to compile (SRC_FAIL, then LINK_FAIL in the
   test).
6. **Tests that replace `free` must start counting after `main` begins.** The CRT frees its own blocks during
   start-up. Use a `g_free_hook` flag that `main` sets first.
7. **Thunk catalog names for the C++ operators stay `MSVCR71.DLL::operator_new` (etc.).** The raw export spelling
   (`??2@YAPAXI@Z`) makes `reloc_identity.py`'s CRT-claim check reject every `??2` caller.
8. **The IAT settles names that the PDB does not.** Ego_r.pdb has both `?floor@@YAMM@Z` and `?ceil@@YAMM@Z`.
   0x00689370 calls the thunk whose IAT slot is `floor`, so it is the float floor, whatever its first landing said.
9. **Some library spellings cannot be compiled.** VC7.1 rejects `__thiscall` on a non-member pointer (C4234), and
   `vc71()` rewrites `__thiscall` to `__fastcall` anyway. So `?__ArrayUnwind@@YGXPAXIHP6EX0@Z@Z` and
   `??_M@YGXPAXIHP6EX0@Z@Z` stay `extern "C"` (`___ArrayUnwind@16`, `___ehvec_dtor@16`).
10. **`unlanded_bytes_estimate` sizes each unlanded function by the gap to the next known start.** Landing the 149-byte
    `__aulldvrm` at 0x00F3FD50 removed a 2.95 MB gap (next start 0x01210A00). As a result, "retail function bytes
    reproduced" jumped from 38.60% to 49.74%, although only 1,747 new bytes were matched. The estimator needs a cap,
    such as the oracle length or the end of the section.
11. **A crashed Wine test can hang `vland.py`.** The child dies but the pipe stays open. Give every vland run a
    `timeout`, and rerun a fixed subset rather than the whole batch.
12. **Payloads built by hand lack `flags`.** Use `lane_tools/recheck.py` for the final recheck so each row is graded
    under its catalog `CompilerFlags`.
