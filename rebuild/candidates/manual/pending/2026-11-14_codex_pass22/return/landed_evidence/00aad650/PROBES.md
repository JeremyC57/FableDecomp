# Pass22 CWaveLoader::LoadWaveFile attempts

Read both earlier drafts/tests, chunk856 notes and lessons, all6 neighbours, original shared headers, all native instructions and original debug donor960 bytes.116 historical grades retained in package notes; no replay of the old local/scope sweeps. Current package baseline1575/1575 has exactly two mov/add vs mov/lea pairs. Original module1189 names/locals reveal split samples/size, unsigned bits and the actual CWaveFile::Initialise call instead of manual stores. Current extent1575 includes emitted terminal INT3; no guessed cut to1574.

Baseline normalization: restored all proven original local names, portable FABLE_FASTCALL, removed obsolete prior commentary; no source-shape attempt counted for names alone.

1. Restore the original visible inline CWaveFile::Initialise member and call, proven by original donor033FF070/033FF3FC, instead of repeated manual receiver dereferences. Original parameter names/types restored.

1 result:1575/1575, same two pairs.
2. Restore original positive PCM if/else scope plus original samples/size split and unsigned bits (module1189 + donor033FF29A..033FF2DE). Earlier sources omitted one actual named computation.

2 result:1575/1575,same2pairs.
3. Original VC6-compatible loop scope guard: both donor loops have xor/test constant-false guard before the actual for-init, absent from our ordinary for. Restore if(false){}else for, with explicit donor evidence; no artificial unused locals.

3 result:same1575/1575 two pairs.
4. Remove inherited force-inline controls from the original reader APIs, using ordinary inline in an isolated shared-header branch. This tests the plain-C++ path before any device; no new inline controls to alter allocation. Pin original assert names.

4 result:804/1575; ordinary reader wrappers retain calls. Rejected, best original-idiom strategy3 restored. The header experiment remains in immutable history and is removed from selected overlay. Compiler trace is diagnostic-only and independently byte-identical to plain RTM.

5. Restore original message-taking CWaveFileException constructors and all four actual donor literals. Shared opt-in delegates to the original const-char CExceptionBase signature; its retail release message path is empty (each native throw directly copies the one-byte object, no message evaluation call). No invented state/callee. This restores source identity independent of any codegen gain.

CPU-target diagnostic only, not source strategies: /G5,/G7,/GB compiled from strategy5 to exclude different instruction-cost models. Full results in diag_cpu_* history. Current compiler defaults are /G6.

5 result:same1575/1575 two pairs.
6. Express 8-bit sample bias/shift with unsigned arithmetic (0x80u), preserving low16bits while avoiding negative signed left shift. This ordinary conversion is inferred source spelling; donor instructions alone cannot distinguish signed/unsigned before the short store. Regrade checks both local conversion and whole-function allocation.

6 result:same1575/1575 two pairs.
7. Restore visible original STLport sized-vector constructor instead of the inherited declaration-only N_OUTOFLINE path. The actual CArray<short> caller and original STLport source determine the template identity/body. Plain out-of-class specialization; no inline device. Independently inspect its emitted body if retained.

7 result:same1575/1575 two pairs.
8. Expose original complete STLport vector assignment via existing VECTOR_C shared opt-in (replace declaration-only VECTOR_ASSIGN). Constructor stays opaque for this isolated experiment; real operator body, not a write-only stand-in.

8 result:same1575/1575 two pairs. Constructor strategy7 independently matches97/97 at00AADD80; assignment strategy8 independently matches236/236 at00AADE80.
9. Combine both genuine visible template callees: either remaining opaque address-taking call can keep the whole local vector escaped, so only simultaneous visibility tests that capture boundary. Both bodies exact independently; no stand-ins or artificial tags.

9 result:1575/1575, all527 instructions and masked bytes equal. Both genuine vector bodies must be visible together; either alone leaves both pairs. Main plain-RTM regrade also exact. Behavior and binding review pending.

Validation: main RTM recompile exact; all six emitted callable helpers exact; recursive146-byte exception metadata exact. Full copied-body fixture2304 normal combinations+empty RIFF passes VC/GCC, four exception cases GCC. Independent all-relocation audit and header default check recorded separately.
