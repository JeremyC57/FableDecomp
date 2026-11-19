# `native/` — FullDecomp's byte-matched C++ as native code in FableRecomp

The FullDecomp branch holds C++ for about 98% of Fable.exe's functions that VC7.1 compiles to the
retail bytes. This directory turns that source into host code (x86-64 or aarch64 Linux) that runs
inside FableRecomp **next to the lifted code**, sharing the same guest memory and objects. A lowered
function replaces its lifted body; everything it calls that is not lowered runs lifted.

## How

1. **Front end.** clang compiles each matched file for `i686-pc-windows-msvc` in VC7.1 mode
   (`-fms-compatibility-version=13.10`) to LLVM IR. That IR has the retail record and vtable
   layouts, 32-bit pointers and the x86 calling conventions. `shim/` holds the few C headers
   the files include (the MSVC target has no CRT headers here). About 99% of the files compile.
2. **`fable_lower`** (this directory, an LLVM 18 tool) makes the IR 64-bit safe. It relies on
   FableRecomp's identity-mapped guest memory (`RECOMP_IDENTITY_MEMORY`): a guest pointer
   zero-extended is a host pointer.
   - GEPs become byte offsets computed with the 32-bit layout.
   - Pointer loads and stores become 32-bit words.
   - Allocas move to the guest stack (`Ctx::esp`), so their addresses are guest addresses.
   - Globals, vtables and string literals bind to their retail addresses through FullDecomp's
     `rebuild/oracles/reloc_bindings.tsv` (COFF symbol to address).
   - Calls to functions that are not lowered, and every indirect call (vtables hold guest
     addresses), become `nd_call`. `nd_call` puts the x86 arguments on the guest stack and runs
     the lifted code or the host trap.
   - Each lowered function gets a guest-ABI entry, `nd_entry_<ADDR>(Ctx*)`, and a wrap,
     `nd_wrap_<ADDR>`. The wrap runs the entry, or `F_<ADDR>_orig` when the function is
     switched off.

   It leaves out what it cannot lower: inline asm, `inalloca`, varargs definitions, dynamic
   allocas, byval parameters, and symbols without a binding. Those stay lifted;
   `report.tsv` says which and why.
3. **Runtime.** `../win/native_rt.cpp` has the frame, call-bridge and entry helpers.
4. **Build.** Lift with the `--wrap` list the script writes, then configure with
   `-DFABLE_NATIVE_OBJS=<out>/nd_<arch>.o`.

```sh
rebuild/recomp/native/build_native.sh addresses.txt out/native     # nd_x64.o, nd_a64.o, report.tsv, wraps.txt
fable_recomp Fable.exe rebuild/manifest/functions.tsv gen_win/ <HANDOFF.md hooks and wraps> $(cat out/native/wraps.txt)
cmake ... -DFABLE_NATIVE_OBJS=$PWD/out/native/nd_a64.o && ninja
```

## Switches

- `FABLE_NATIVE_DECOMP=0`: every lowered function off (the lifted bodies run).
- `FABLE_ND_OFF=<hex>[,<hex>-<hex>...]`: single functions or address ranges off, for bisecting a crash.

## Differences from retail

The front end runs with `-fno-strict-aliasing -fwrapv -fno-delete-null-pointer-checks`, and `fable_lower` drops
every alignment, dereferenceable and nonnull assumption: VC7.1 exploited none of them and the game
relies on that (an object of this kind sits at an odd address in retail).

Float arithmetic is clang's, in source order, at IEEE single and double precision. The game runs
the x87 at 24-bit precision, and VC7.1 reordered some sums, so results can differ from retail in
the last bits; double-precision code differs more. Math library calls (`sqrt`, `sin`, ...) use the
host libc.
