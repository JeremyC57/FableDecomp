# asm1 hand-back -- row 0x009D8650 ?MicroThreadSleep@MicroThread@@YIXXZ under docs/pipeline/ASM_RULES.md (owner task 2026-11-18)

Owner ask: "Fix the naked asm row to follow our current rules." Nothing here is landed; the repo was read-only.

**Status in one line:** under the repo's current rules and tools the row is `naked_asm` (purity.py) / NOT_GENUINE (vland): the bytes
(RELOCATION_MATCH) and behaviour test (PASS) are done, every other ASM_RULES item is met, but grade `retail_asm` exists only under the
unapplied purity proposal and an owner exception for a whole-function asm helper (decision 1).

## Files
| file | what |
|---|---|
| `src/009d8650.cpp` | the row: `#include "engine/MicroThreadSleep.h"` + `FABLE_MICROTHREAD_SLEEP_DEFINITION` inside `namespace MicroThread`; no `__asm` / `naked` / `_emit` in its text. RELOCATION_MATCH (36/36). |
| `test/009d8650.cpp` | plain-C++ behaviour test (MSVC SEH `__try` / `__except`, no assembler), links the row object: PASS. Three mutant helpers (no chain-head swap / no ebp save / global not updated) all FAIL. |
| `hdr/engine/MicroThreadSleep.h` | NEW shared helper header (overlay path = proposed `rebuild/include/engine/MicroThreadSleep.h`): declarations for callers, the macro-wrapped asm definition (retail path), the `FABLE_PORTABLE` twin (`fable_portable::MicroThreadSleep`), `// Retail-asm-helper:` / `// Retail-asm:` / `// Retail-asm-function:` lines. |
| `portable/test_MicroThreadSleep.cpp` | dual-path test for `rebuild/tests/portable/` (+ a `fable_test_mode.h` copy for the local run, `run_dual.py`, `results.tsv`). |
| `proposals/purity_whole_function_helper.diff` (+ `proposals/crawl/purity.py`, `with_proposed_purity.py`, `compare_purity.py`) | proposed, UNAPPLIED purity.py change (owner decision 1) and the checks run with it. |
| `probe/att/` | the rule-4 attempt sources `c01..c32`, `i01..i12`, `index.tsv`, `results.tsv`, raw q.py output `*.out` (`probe/gen_attempts.py`, `probe/grade_attempts.py`). |
| `probe/nn1..nn4.cpp`, `probe/iuse.cpp`, `probe/ie_*.cpp`, `probe/mut_M1..M3/` | prologue proof, intrinsic existence probes, behaviour-test mutants. |
| `payload.json`, `oracle.tsv`, `NOTES.md`, `STATUS.tsv`, `NAME_FIXES.tsv`, `LABELS.tsv`, `LESSONS.md`, `hdr/README.md` | lane hand-back files. |

## Rule-4 attempt table (graded against retail 0x009D8650 with `$S/tools/q.py`, VC7.1 RTM 13.10.3077, base flags /O2 /Oy /W3)
Retail (36 bytes): `push ebx ; push ebp ; push esi ; push edi ; mov eax,fs:[0] ; push eax ; mov eax,esp ; mov esp,[0x013CA898] ;
mov [0x013CA898],eax ; pop eax ; mov fs:[0],eax ; pop edi ; pop esi ; pop ebp ; pop ebx ; ret`. What no attempt produced: the stack
switch 0x009D865D `mov esp, dword ptr [0x013CA898]` (every attempt lacks it), the chain head carried through the switched stacks
(0x009D865A `push eax` / 0x009D8668 `pop eax`), and retail's push order with ebp second (0x009D8651).

### Plain C++ (32 distinct spellings, all compiled, all DIFFER)
| # | family | idea | flags | grade | VC7.1 emitted (relocations shown as 0) |
|---|---|---|---|---|---|
| c01 | empty | empty body (the function as a no-op call) | - | DIFFER(1v36) | `ret` |
| c02 | global swap | swap the global with the address of a local | - | DIFFER(11v36) | `push ecx ; lea eax, [esp] ; mov dword ptr [0], eax ; pop ecx ; ret` |
| c03 | global swap | swap through a temporary whose own address is published | - | DIFFER(11v36) | `push ecx ; lea eax, [esp] ; mov dword ptr [0], eax ; pop ecx ; ret` |
| c04 | call other side | call the parked pointer as a function | - | DIFFER(6v36) | `jmp dword ptr [0]` |
| c05 | call other side | call the parked frame's return address (tail position) | - | DIFFER(8v36) | `mov eax, dword ptr [0] ; jmp dword ptr [eax + 0x14]` |
| c06 | setjmp/longjmp | symmetric setjmp/longjmp: park own jmp_buf, longjmp to the other side | - | DIFFER(55v36) | `push ebp ; mov ebp, esp ; sub esp, 0x44 ; mov eax, dword ptr [0] ; lea ecx, [ebp - 0x44] ; mov edx, ecx ; push 0 ; push edx ; mov dword ptr [ebp - 4], eax ; mov dword ptr [0], ecx ; call 0x9d8671 ; add esp, 8 ; test eax, eax ; jne 0x9d8683 ; ...` |
| c07 | setjmp/longjmp | longjmp only, into the jmp_buf the global points at | - | DIFFER(14v36) | `mov eax, dword ptr [0] ; push 1 ; push eax ; call 0x9d865d ; int3` |
| c08 | frame copy | copy the other side's saved frame into a local | - | DIFFER(16v36) | `lea eax, [esp - 0x18] ; sub esp, 0x18 ; mov dword ptr [0], eax ; add esp, 0x18 ; ret` |
| c09 | frame build | build the saved frame as a local struct and publish it | - | DIFFER(16v36) | `lea eax, [esp - 0x18] ; sub esp, 0x18 ; mov dword ptr [0], eax ; add esp, 0x18 ; ret` |
| c10 | SEH frame | __try/__finally around the swap (the compiler's own fs:[0] registration) | - | DIFFER(89v36) | `push ebp ; mov ebp, esp ; push -1 ; push 0 ; push 0 ; mov eax, dword ptr fs:[0] ; push eax ; mov dword ptr fs:[0], esp ; sub esp, 0xc ; push ebx ; push esi ; push edi ; mov dword ptr [ebp - 4], 0 ; mov eax, dword ptr [0] ; ...` |
| c11 | C++ EH frame | try/catch(...) around the swap, /EHsc (C++ EH registration writes fs:[0]) | /EHsc | DIFFER(66v36) | `push ebp ; mov ebp, esp ; push -1 ; push 0 ; mov eax, dword ptr fs:[0] ; push eax ; mov dword ptr fs:[0], esp ; push ecx ; push ebx ; push esi ; push edi ; mov dword ptr [ebp - 0x10], esp ; mov dword ptr [ebp - 4], 0 ; call 0x9d867b ; ...` |
| c12 | C++ EH frame | a local with a destructor, /EHsc (unwind registration) | /EHsc | DIFFER(66v36) | `push -1 ; push 0 ; mov eax, dword ptr fs:[0] ; push eax ; mov dword ptr fs:[0], esp ; push ecx ; mov dword ptr [esp + 0xc], 0 ; call 0x9d8673 ; lea ecx, [esp] ; mov dword ptr [esp + 0xc], 0xffffffff ; call 0x9d8683 ; mov ecx, dword ptr [esp + 4] ; mov dword ptr fs:[0], ecx ; add esp, 0x10 ; ...` |
| c13 | register pressure | six live values (forces the callee-saved pushes in FPO order) | - | DIFFER(68v36) | `mov eax, dword ptr [0] ; mov edx, dword ptr [eax + 0x10] ; mov ecx, dword ptr [eax + 8] ; push ebx ; mov ebx, dword ptr [eax + 0xc] ; push ebp ; mov ebp, dword ptr [eax + 0x14] ; push esi ; mov esi, dword ptr [eax] ; push edi ; mov edi, dword ptr [eax + 4] ; mov dword ptr [eax + 4], edx ; imul edx, ebx ; mov dword ptr [eax + 0xc], ecx ; ...` |
| c14 | external swap | the draft: an extern "C" SwapStackPointer(&global) stand-in | - | DIFFER(12v36) | `push 0 ; call 0x9d865a ; pop ecx ; ret` |
| c15 | reference swap | swap through a reference to the global | - | DIFFER(1v36) | `ret` |
| c16 | member function | an inline member of a stack-switch struct | - | DIFFER(11v36) | `push ecx ; lea eax, [esp] ; mov dword ptr [0], eax ; pop ecx ; ret` |
| c17 | static local | park through a function-static | - | DIFFER(21v36) | `mov eax, dword ptr [0] ; mov dword ptr [0], eax ; mov dword ptr [0], 0 ; ret` |
| c18 | unoptimised TU | swap under #pragma optimize("", off) (the microthread caller TU is /Od) | - | DIFFER(25v36) | `push ebp ; mov ebp, esp ; push ecx ; mov eax, dword ptr [0] ; mov dword ptr [ebp - 4], eax ; lea ecx, [ebp - 4] ; mov dword ptr [0], ecx ; mov esp, ebp ; pop ebp ; ret` |
| c19 | OS fibre | SwitchToFiber(parked) -- the OS performs the switch | - | DIFFER(13v36) | `mov eax, dword ptr [0] ; push eax ; call dword ptr [0] ; ret` |
| c20 | union pun | swap through a pointer/integer union | - | DIFFER(11v36) | `push ecx ; lea eax, [esp] ; mov dword ptr [0], eax ; pop ecx ; ret` |
| c21 | stackless coroutine | Duff/protothread state switch (no stack switch at all) | - | DIFFER(38v36) | `mov eax, dword ptr [0] ; xor ecx, ecx ; sub eax, ecx ; je 0x9d866b ; dec eax ; jne 0x9d8675 ; mov dword ptr [0], ecx ; mov dword ptr [0], ecx ; ret ; mov dword ptr [0], 1 ; ret` |
| c22 | return into frame | fetch the parked frame's return address and jump there through a noreturn pointer | - | DIFFER(25v36) | `push ecx ; mov eax, dword ptr [0] ; mov eax, dword ptr [eax + 0x14] ; lea ecx, [esp] ; mov dword ptr [esp], eax ; mov dword ptr [0], ecx ; call eax ; pop ecx ; ret` |
| c23 | volatile | volatile swap (forces every load and store) | - | DIFFER(27v36) | `push ecx ; mov dword ptr [esp], 0 ; mov eax, dword ptr [0] ; lea ecx, [esp] ; mov dword ptr [esp], eax ; mov dword ptr [0], ecx ; pop ecx ; ret` |
| c24 | frame pointer | the swap with a frame pointer (/Oy-) | /O2 /Oy- /W3 | DIFFER(16v36) | `push ebp ; mov ebp, esp ; push ecx ; lea eax, [ebp - 4] ; mov dword ptr [0], eax ; mov esp, ebp ; pop ebp ; ret` |
| c25 | SEH frame | __try/__except(EXCEPTION_EXECUTE_HANDLER) around the swap | - | DIFFER(85v36) | `push ebp ; mov ebp, esp ; push -1 ; push 0 ; push 0 ; mov eax, dword ptr fs:[0] ; push eax ; mov dword ptr fs:[0], esp ; sub esp, 8 ; push ebx ; push esi ; push edi ; mov dword ptr [ebp - 0x18], esp ; mov dword ptr [ebp - 4], 0 ; ...` |
| c26 | noreturn call | __declspec(noreturn) resume of the other side | - | DIFFER(11v36) | `mov ecx, dword ptr [0] ; jmp 0x9d865b` |
| c27 | OS interlock | kernel32 InterlockedExchange (API call) of the global with a local's address | - | DIFFER(18v36) | `push ecx ; lea eax, [esp] ; push eax ; push 0 ; call dword ptr [0] ; pop ecx ; ret` |
| c28 | forced-inline helper | a __forceinline swap helper expanded in place (the GFPackFloat helper shape) | - | DIFFER(11v36) | `push ecx ; lea eax, [esp] ; mov dword ptr [0], eax ; pop ecx ; ret` |
| c29 | address arithmetic | compute "esp" from a local's address and store it (no write to esp) | - | DIFFER(14v36) | `push ecx ; lea eax, [esp] ; sub eax, 0x14 ; mov dword ptr [0], eax ; pop ecx ; ret` |
| c30 | recursion | call itself through a pointer when the frame is not ours (forces a real frame) | - | DIFFER(30v36) | `push ecx ; mov eax, dword ptr [0] ; test eax, eax ; lea ecx, [esp] ; mov dword ptr [esp], eax ; mov dword ptr [0], ecx ; je 0x9d866c ; call dword ptr [0] ; pop ecx ; ret` |
| c31 | tail call | tail call to a resume function that would do the switch | - | DIFFER(5v36) | `jmp 0x9d8655` |
| c32 | struct return | return the parked frame by value into the caller (copies, no switch) | - | DIFFER(69v36) | `mov eax, dword ptr [0] ; mov ecx, dword ptr [eax] ; mov dword ptr [0], ecx ; mov edx, dword ptr [eax + 4] ; mov dword ptr [4], edx ; mov ecx, dword ptr [eax + 8] ; mov dword ptr [8], ecx ; mov edx, dword ptr [eax + 0xc] ; mov dword ptr [0xc], edx ; mov ecx, dword ptr [eax + 0x10] ; mov dword ptr [0x10], ecx ; mov edx, dword ptr [eax + 0x14] ; mov dword ptr [0x14], edx ; mov dword ptr [0], 0 ; ...` |

c28 was first `NtCurrentTeb()->ExceptionList` through `<windows.h>`: SRC_FAIL (no Platform SDK headers in the lane, `C1083 windows.h`),
replaced by the forced-inline helper spelling graded above. Closest families: c10 / c11 / c12 / c25 (the compiler's own SEH / C++ EH
registration writes `fs:[0]` from esp but never loads esp from memory), c13 (the compiler's FPO save order push ebx ... ebp ... esi ...
edi, interleaved with loads, only under real register pressure), c18 / c24 / i05 / i06 / i12 (the only esp writes VC7.1 emits are its
own `mov esp,ebp` / `lea esp,[ebp-4]` epilogues and `_chkstk` probes; i11 has an ebp frame but no esp write at all). 32 distinct spellings,
fewer distinct outputs: c02 / c03 / c16 / c20 / c28 emit identical code (`push ecx ; lea eax,[esp] ; mov [g],eax ; pop ecx ; ret`), as do
c01 / c15 (`ret`).

### Intrinsics (12 spellings, all compiled, all DIFFER)
Existence (compile-tested, `probe/iuse.cpp` + `probe/ie_*.cpp`): VC7.1 RTM emits inline `_AddressOfReturnAddress` (`lea eax,[esp]`),
`_ReturnAddress` (`mov eax,[esp]`), **`__readfsdword` (`64 A1` = `mov eax,fs:[0]`) and `__writefsdword` (`64 A3` = `mov fs:[0],eax`, with
warning C4733)**, `_InterlockedExchange` (`xchg`), `_alloca` (`_chkstk` in an ebp frame), `_disable` / `_enable` (cli / sti),
`__debugbreak` (int3). Not intrinsics in VC7.1 (C4163): `_InterlockedExchangePointer`, `__getcallerseflags`, `__noop` (pragma form),
`__ud2`, `__writeeflags`. So the brief's "`__readfsdword` does NOT exist in VC7.1" is wrong for RTM 3077; still, no intrinsic writes esp,
pushes or pops, and the chain head here travels through `push eax` / `pop eax` on the two stacks.
| # | family | idea | flags | grade | VC7.1 emitted (relocations shown as 0) |
|---|---|---|---|---|---|
| i01 | __readfsdword/__writefsdword | save fs:[0] with __readfsdword, write it back with __writefsdword, swap the global | - | DIFFER(23v36) | `push ecx ; mov eax, dword ptr [0] ; lea ecx, [esp] ; mov dword ptr [0], ecx ; mov dword ptr fs:[0], eax ; pop ecx ; ret` |
| i02 | __readfsdword/__writefsdword | publish a frame whose first word is fs:[0], restore the other side's chain from its frame | - | DIFFER(39v36) | `sub esp, 0x18 ; mov eax, dword ptr fs:[0] ; mov dword ptr [esp], eax ; mov eax, dword ptr [0] ; lea ecx, [esp] ; mov dword ptr [0], ecx ; mov edx, dword ptr [eax] ; mov dword ptr fs:[0], edx ; add esp, 0x18 ; ret` |
| i03 | _AddressOfReturnAddress | redirect our own return address to the parked frame's return address | - | DIFFER(20v36) | `mov ecx, dword ptr [0] ; lea eax, [esp] ; mov dword ptr [0], eax ; mov ecx, dword ptr [ecx + 0x14] ; mov dword ptr [eax], ecx ; ret` |
| i04 | _ReturnAddress | publish our return address in the global | - | DIFFER(9v36) | `mov eax, dword ptr [esp] ; mov dword ptr [0], eax ; ret` |
| i05 | _alloca | _alloca(sizeof frame) to push a frame image | - | DIFFER(70v36) | `push ebp ; mov ebp, esp ; mov eax, 0x18 ; call 0x9d865d ; mov eax, esp ; mov dword ptr [eax], 0 ; mov ecx, dword ptr [0] ; mov dword ptr [0], eax ; mov edx, dword ptr [ecx] ; mov dword ptr [eax], edx ; mov edx, dword ptr [ecx + 4] ; mov dword ptr [eax + 4], edx ; mov edx, dword ptr [ecx + 8] ; mov dword ptr [eax + 8], edx ; ...` |
| i06 | _alloca | _alloca(distance to the parked stack) -- move esp down to it | - | DIFFER(41v36) | `push ebp ; mov ebp, esp ; push ecx ; mov ecx, dword ptr [0] ; lea eax, [ebp - 1] ; sub eax, ecx ; add eax, 3 ; and eax, 0xfffffffc ; call 0x9d866a ; lea eax, [ebp - 1] ; mov dword ptr [0], eax ; lea esp, [ebp - 4] ; mov esp, ebp ; pop ebp ; ...` |
| i07 | _InterlockedExchange | atomic swap of the global with a local's address | - | DIFFER(13v36) | `push ecx ; lea eax, [esp] ; mov ecx, 0 ; xchg dword ptr [ecx], eax ; pop ecx ; ret` |
| i08 | _InterlockedExchange + fs | atomic global swap plus the fs:[0] save/restore intrinsics | - | DIFFER(22v36) | `push ecx ; lea eax, [esp] ; mov ecx, 0 ; xchg dword ptr [ecx], eax ; mov edx, dword ptr [eax] ; mov dword ptr fs:[0], edx ; pop ecx ; ret` |
| i09 | _AddressOfReturnAddress + fs | redirect the return address and swap the chain head | - | DIFFER(44v36) | `mov ecx, dword ptr [0] ; lea eax, [esp] ; push esi ; mov esi, dword ptr fs:[0] ; lea edx, [eax - 4] ; mov dword ptr [edx], esi ; mov dword ptr [0], edx ; mov edx, dword ptr [ecx] ; mov ecx, dword ptr [ecx + 0x14] ; mov dword ptr fs:[0], edx ; mov dword ptr [eax], ecx ; pop esi ; ret` |
| i10 | _AddressOfReturnAddress + memcpy | copy the parked frame over our own frame with the memcpy intrinsic | - | DIFFER(52v36) | `mov ecx, dword ptr [0] ; lea eax, [esp] ; add eax, -0x14 ; mov dword ptr [0], eax ; mov edx, dword ptr [ecx] ; mov dword ptr [eax], edx ; mov edx, dword ptr [ecx + 4] ; mov dword ptr [eax + 4], edx ; mov edx, dword ptr [ecx + 8] ; mov dword ptr [eax + 8], edx ; mov edx, dword ptr [ecx + 0xc] ; mov dword ptr [eax + 0xc], edx ; mov edx, dword ptr [ecx + 0x10] ; mov dword ptr [eax + 0x10], edx ; ...` |
| i11 | _AddressOfReturnAddress /Oy- | ebp frame (/Oy-): "esp" = _AddressOfReturnAddress()+4 published, chain restored from the parked frame | /O2 /Oy- /W3 | DIFFER(31v36) | `push ebp ; mov ebp, esp ; mov eax, dword ptr [0] ; lea ecx, [ebp + 4] ; add ecx, 4 ; mov dword ptr [0], ecx ; mov edx, dword ptr [eax] ; mov dword ptr fs:[0], edx ; pop ebp ; ret` |
| i12 | _alloca + _AddressOfReturnAddress | _alloca the 5-word image then redirect the return address (closest to push+ret) | - | DIFFER(56v36) | `push ebp ; mov ebp, esp ; mov eax, 0x14 ; call 0x9d865d ; mov ecx, dword ptr fs:[0] ; mov eax, esp ; mov dword ptr [eax], ecx ; mov ecx, dword ptr [0] ; mov dword ptr [0], eax ; mov edx, dword ptr [ecx] ; mov ecx, dword ptr [ecx + 0x14] ; lea eax, [ebp + 4] ; mov dword ptr fs:[0], edx ; mov dword ptr [eax], ecx ; ...` |

### Asm spellings that keep a compiler prologue (why the definition has none)
| probe | body | VC7.1 emitted |
|---|---|---|
| nn1 | the 7 core instructions (`mov eax,fs:[0]` ... `mov fs:[0],eax`) in a normal function | 28 bytes: the core + `ret`; no register saved |
| nn2 | all 15 retail instructions (own push / pop of ebx ebp esi edi) in a normal function | 42 bytes: compiler `push ebx ; push esi ; push edi` around the block |
| nn3 | core + `xor ebx/esi/edi` | 40 bytes: compiler saves ebx esi edi (in that order), never ebp |
| nn4 | core + `xor ebx/ebp/esi/edi` | 42 bytes: ebp still not saved |
| e14 probe / this row | the whole function in assembler, no compiler prologue (macro in the shared header) | 36 bytes, RELOCATION_MATCH |

VC7.1 never saves ebp around an inline-asm block and puts its own pushes first, so retail's `push ebx ; push ebp ; push esi ; push edi`
exists only as hand asm with no compiler prologue. Ego_d.pdb's /Od body is 36 bytes as well (a debug C++ body would carry an ebp frame and
/RTC fill): the original source was written that way.

## Grade
- Bytes: RELOCATION_MATCH (fg.py / q.py, and vland's own parity), behaviour test PASS (vt.py, and vland).
- `purity.classify(src/009d8650.cpp)` with the repo's purity.py = **`naked_asm`**; read-only vland = **NOT_GENUINE** (purity gate): the
  helper header fails `_retail_asm` because it contains `naked` and an asm `ret`. With the proposed purity change: classify = `retail_asm`,
  the test = `genuine`, vland = `RELOCATION_MATCH PASS` (WINS 1, no relocation-identity warning). The proposal changes no other
  classification (56,612 rebuild/src sources and 31 helper headers compared: 0 differences). The purity unit test
  (tools/decomp_pipeline/tests/test_purity_helpers.py) has the same 3 failures with and without it: all three call `classify()` on header /
  row text that spells `__asm` and expect `retail_asm`, which the 2026-11-05 rule-5 change made `naked_asm` (stale tests, unrelated to
  naked); all 31 repo helper headers pass the helper evidence rule (`_helper_header` ok) today.
- Precedent: none. `grep -rln naked rebuild/include` finds no header; every existing helper keeps a compiler prologue (a `__forceinline`
  function or bare `__asm` statements).

## Dual-path results (portable_twins_check.py imported unmodified by `portable/run_dual.py`, --no-rows, TESTS / INC repointed)
| step | result |
|---|---|
| vc71-hdr-asm / vc71-hdr-portable | OK / OK |
| vc71-asm | PASS: asm path, scenario S (separate stacks) and C (shared stack + RestoreStack / PreserveStack copy, CreateMicroThread's first frame) |
| vc71-dual | PASS: asm path S + C and the Win32 fibre twin S, each against the same expected 24-point trace, sums and handler checks |
| vc71-portable | PASS: Win32 fibre twin S (C n/a: fibres own their stacks) |
| gxx-hdr | OK |
| gxx-portable | PASS: POSIX ucontext twin S + C |
| gxx-m32-compile | SKIPPED (no 32-bit glibc headers on this host) |

Extra: g++ and clang++ at -O0 / -O1 / -O2 / -O3 / -Os, S + C PASS (clang -O1+ first FAILED C until the POSIX twin was made noinline, see
decision 2). The row also compiles under VC7.1 /DFABLE_PORTABLE and g++ -DFABLE_PORTABLE.

## Owner decisions
1. **A rule exception is needed: a whole-function (`naked`) asm helper.** The current rules name `naked` as oracle-only: ASM_RULES rule 5
   ("`naked` / `_emit` byte bakes (`naked_asm`, `emit_bake`, `asm_bake`) are oracles, never reconstruction, and never counted"), CLAUDE.md
   Byte purity ("`__asm` / `naked` / `_emit` is grade `asm_bake`: an oracle, never reconstruction"), and purity.py's evidence rule ("it has
   no `naked` and no `_emit` (the function's prologue and epilogue stay compiler-emitted)"; "no `__asm` statement returns"), which it applies
   to helper headers too. This row meets ASM_RULES 2 and 3.2 otherwise (retail bytes no C++ / intrinsic emits, isolated macro-wrapped asm in
   a shared header under the PDB name, FABLE_PORTABLE twin, evidence lines, dual-path test, no asm in row or test), but its asm cannot keep a
   compiler prologue: nn1-nn4 show VC7.1 never saves ebp around inline asm and adds its own pushes, and Ego_d.pdb's /Od length (36 = retail)
   shows the original source had none either. So the request: allow a shared helper header to define the whole routine without a compiler
   prologue when the original did (proof: retail address + debug-build length), e.g. by applying
   `proposals/purity_whole_function_helper.diff` (a `// Retail-asm-function:` line citing a retail address unlocks `naked` + `ret` in a
   helper header only; rows still may not spell them) and amending rule 5 to match. Otherwise the row stays parked (its bytes are an
   oracle). Nothing is disguised: the header spells `__declspec(naked)` and `__asm ret`.
2. **"Identical semantics" for the FABLE_PORTABLE twin cannot be met.** ISO C++ cannot switch stacks. The twin keeps the protocol (park this
   side's context on its own stack, publish its token in PGlobalStackPointer, resume the parked side; each side keeps its callee-saved
   registers, locals and its own exception handlers), proven by the dual-path test, but NOT the representation: the token is a
   `ucontext_t*` (POSIX) or a fibre handle (Win32), not an esp pointing at the 24-byte frame (chain, edi, esi, ebp, ebx, return).
   Consequences: CreateMicroThread 0x009D8710 (which fabricates that frame) needs a portable branch before a portable build can start a
   micro-thread. On POSIX, SwitchToMicroThread's stack-image copying keeps working unchanged (tested, scenario C) because the twin is never
   inlined and its `ucontext_t` is the lowest live object of its frame -- true for x86-64 GCC / Clang (tested), not guaranteed by C++ nor on
   ABIs that keep the frame record below the locals (e.g. AArch64). On Win32 the image copying does not apply (fibre stacks), so
   SwitchToMicroThread would need a portable branch too, and the twin needs one variable retail does not have
   (`fable_portable::MicroThreadRunningFiber()`, set by whoever calls ConvertThreadToFiber). FPU state: the asm path touches none (x87
   control word / SSE state are not saved); ucontext saves the FP environment (a superset); fibres save what the OS saves.
3. **Name of 0x013CA898.** Proven `PGlobalStackPointer` (Ego_d.pdb + Anniversary PDB, both S_LDATA32); reloc_bindings and the landed
   SwitchToMicroThread 0x009D87F0 say `SavedStackPointer` (descriptive). vland does not warn either way. NAME_FIXES.tsv lists it with the
   other TU statics found on the way (the coordinator respells the landed users). Namespace membership is unproven (LDATA32 records carry
   none); assumed MicroThread like DummyExceptionBlock.
4. **Header placement.** New file `rebuild/include/engine/MicroThreadSleep.h` (no MicroThread header existed in rebuild/include; named after
   the routine like engine/GFGetCycleCount.h, so it does not pre-empt a future `engine/MicroThread*.h` class header). The landed callers
   (0x009D87F0, 0x00A44690, 0x00A446A0) declare MicroThreadSleep themselves; including this header gives them the same declaration and
   does not change their purity grade (they never spell the macro). Include order with rebuild_abi.h checked both ways under VC7.1 /W4
   (no C4005: the header defines FABLE_FASTCALL only when absent, with identical tokens). Landing note: the header comment points at "the
   asm1 hand-back README.md" for the attempt table -- repoint it at the batch README the table lands in. Side note: the repo's
   engine/CSuspendableProcessMicrothread.h forward-declares `struct MicroThreadInfo;` at global scope, while the PDB class is
   `MicroThread::MicroThreadInfo` (not touched here).
