# asm1 NOTES -- row 0x009D8650 ?MicroThreadSleep@MicroThread@@YIXXZ (owner task 2026-11-18: fix the naked asm row to ASM_RULES)

## Findings so far
- Global 0x013CA898: Ego_d.pdb lib_microthread.obj S_LDATA32 `PGlobalStackPointer` (type 0x420, Ego_d sec4 off 1109616);
  Anniversary Xbox360 PDB S_LDATA32 `PGlobalStackPointer` too (second source). Ego_d order of the TU statics (sec4 offsets):
  PActiveScript 1109612 / PGlobalStackPointer +4 / MicroThread::ThreadStackSize +8 (GDATA32, int) / PScriptStackTop +12 /
  PScriptStackTopWithChecksum +16 == retail 0x013CA894 / 898 / 89C / 8A0 / 8A4 (same order, same spacing).
  Landed descriptive spellings: SavedStackPointer (0x898), ActiveThread / g_pActiveMicroThread (0x894), g_stackSize (0x89C),
  StackTop / g_pStackTop (0x8A0), g_pStackEnd (0x8A4). LDATA32 records omit the namespace (DummyExceptionBlock is LDATA32 too
  and lives in namespace MicroThread), so namespace membership of PGlobalStackPointer is unproven (assumed MicroThread).
- Non-naked probes (probe/nn1..nn4.cpp, base flags /O2 /Oy /W3):
  nn1 asm core only: 28 bytes, no register saves (`mov eax,fs:[0] ... mov fs:[0],eax ; ret`).
  nn2 asm core + its own push/pop of ebx ebp esi edi: 42 bytes, compiler adds push ebx/esi/edi around it.
  nn3 core + xor ebx/esi/edi: 40 bytes, compiler saves ebx esi edi (order ebx esi edi), never ebp.
  nn4 core + xor ebx/ebp/esi/edi: 42 bytes, compiler STILL does not save ebp.
  => VC7.1 never saves ebp for an inline asm block; retail's `push ebx ; push ebp ; push esi ; push edi` order with ebp second
  can only come from hand asm in a naked function. With Ego_d /Od length 36 == retail 36, a naked definition is the only way.
- e14 naked probe (probe/e14hdr + probe/e14_f1.cpp) regraded today: RELOCATION_MATCH.
- purity.py `_retail_asm` rejects a helper header containing "naked" or an asm `ret` -> a naked helper makes the row grade
  naked_asm under the current tool (owner decision #1; do not disguise the keyword).
- INTRINSICS (compile-tested, probe/iuse.cpp, VC7.1 RTM 13.10.3077): `#pragma intrinsic` + declaration accepted and emitted inline for
  _AddressOfReturnAddress (`lea eax,[esp]`), _ReturnAddress (`mov eax,[esp]`), __readfsdword (`mov eax,fs:[0]` = 64 A1, the retail
  encoding), __writefsdword (`mov fs:[0],eax` = 64 A3, plus warning C4733 "Inline asm assigning to 'FS:0': handler not registered as
  safe handler"), _InterlockedExchange (`xchg [ecx],eax`), _alloca (`mov eax,n ; call __chkstk` in an ebp frame), _disable/_enable
  (cli/sti), __debugbreak (int3). C4163 "not available as an intrinsic": _InterlockedExchangePointer, __getcallerseflags, __noop
  (pragma form), __ud2, __writeeflags. => the BRIEF's "__readfsdword does NOT exist in VC7.1" is wrong: it exists in RTM 3077
  (GOTCHA). No intrinsic writes esp, pushes, or pops.

## Row status (2026-11-18)
- src/009d8650.cpp (#include "engine/MicroThreadSleep.h" + FABLE_MICROTHREAD_SLEEP_DEFINITION inside namespace MicroThread): RELOCATION_MATCH.
- test/009d8650.cpp: PASS under vt.py linked with the row object (scenario 1 separate stacks, scenario 2 retail shared stack +
  RestoreStack/PreserveStack image copy with CreateMicroThread's first frame; SEH __try/__except per side, parked-frame checks).
  Mutants (probe/mut_M1..M3 header copies): M1 no fs:[0] save/restore -> FAIL; M2 no push/pop ebp -> FAIL; M3 global not
  updated with the old esp -> FAIL.
- Rule-4 table: probe/att/results.tsv (32 C++ c01..c32 + 12 intrinsic i01..i12, all compiled, all DIFFER) -- README.md.
- purity.classify(row) = naked_asm (helper header rejected: contains `naked` and an asm `ret`). Owner decision #1.
- Dual-path test portable/test_MicroThreadSleep.cpp (run_dual.py = portable_twins_check.py imported unmodified, TESTS/INC repointed):
  vc71-hdr-asm OK, vc71-hdr-portable OK, vc71-asm PASS (asm S + C), vc71-dual PASS (asm S + C, Win32 fibre twin S), vc71-portable
  PASS (fibre twin S), gxx-hdr OK, gxx-portable PASS (POSIX ucontext twin S + C), gxx-m32-compile SKIPPED (no 32-bit glibc headers).
  Extra: g++ and clang++ at -O0/-O1/-O2/-O3/-Os all PASS S + C. FIRST clang -O1+ FAILED scenario C: the inline twin was inlined
  (g_sleep devirtualised) into UcEntry/Body and Body's argument spill sat BELOW `self` -> not copied by PreserveStack -> thread 0 resumed
  with thread 1's id. Fixed by __attribute__((noinline)) on the POSIX twin (GNU-guarded). The copy contract relies on `self` being the
  lowest live object of the parked side: true for x86-64 SysV GCC/Clang (tested), NOT guaranteed by C++ or on ABIs whose frame record
  lies below the locals (e.g. AArch64) -> part of owner decision #2.
- Row also compiles with VC7.1 /DFABLE_PORTABLE and g++ -std=c++03 -DFABLE_PORTABLE -c (64-bit).

## Final state (2026-11-18)
- Grades: RELOCATION_MATCH (fg/q + vland parity), behaviour test PASS (vt + vland). purity: repo = naked_asm (vland NOT_GENUINE);
  proposed purity (proposals/purity_whole_function_helper.diff, unapplied) = retail_asm (vland RELOCATION_MATCH PASS, WINS 1, no reloc
  warning). Proposal is inert elsewhere: 56,612 rebuild/src sources + 31 helper headers, 0 classification differences; purity unit test
  3 failures pre-existing (same with/without).
- Dual path: 7 steps OK/PASS, gxx-m32-compile SKIPPED (portable/results.tsv).
- Owner decisions: (1) whole-function asm helper vs purity's no-naked/no-ret rule; (2) twin keeps the protocol, not identical semantics
  (token representation, CreateMicroThread first frame, Win32 image copying, AArch64 caveat); (3) PGlobalStackPointer respelling;
  (4) new header engine/MicroThreadSleep.h. Details: README.md.
- Grades used on the row: ~70 (44 attempt grades + 4 nn probes + mutants + regrades).

## Rule-4 attempts (compact; full table with ideas in README.md, sources probe/att/)
id | family | grade | emitted (first 8 instr)
c01 | empty | DIFFER(1v36) | ret
c02 | global swap | DIFFER(11v36) | push ecx ; lea eax, [esp] ; mov dword ptr [0], eax ; pop ecx ; ret
c03 | global swap | DIFFER(11v36) | push ecx ; lea eax, [esp] ; mov dword ptr [0], eax ; pop ecx ; ret
c04 | call other side | DIFFER(6v36) | jmp dword ptr [0]
c05 | call other side | DIFFER(8v36) | mov eax, dword ptr [0] ; jmp dword ptr [eax + 0x14]
c06 | setjmp/longjmp | DIFFER(55v36) | push ebp ; mov ebp, esp ; sub esp, 0x44 ; mov eax, dword ptr [0] ; lea ecx, [ebp - 0x44] ; mov edx, ecx ; push 0 ; push edx
c07 | setjmp/longjmp | DIFFER(14v36) | mov eax, dword ptr [0] ; push 1 ; push eax ; call 0x9d865d ; int3
c08 | frame copy | DIFFER(16v36) | lea eax, [esp - 0x18] ; sub esp, 0x18 ; mov dword ptr [0], eax ; add esp, 0x18 ; ret
c09 | frame build | DIFFER(16v36) | lea eax, [esp - 0x18] ; sub esp, 0x18 ; mov dword ptr [0], eax ; add esp, 0x18 ; ret
c10 | SEH frame | DIFFER(89v36) | push ebp ; mov ebp, esp ; push -1 ; push 0 ; push 0 ; mov eax, dword ptr fs:[0] ; push eax ; mov dword ptr fs:[0], esp
c11 | C++ EH frame | DIFFER(66v36) | push ebp ; mov ebp, esp ; push -1 ; push 0 ; mov eax, dword ptr fs:[0] ; push eax ; mov dword ptr fs:[0], esp ; push ecx
c12 | C++ EH frame | DIFFER(66v36) | push -1 ; push 0 ; mov eax, dword ptr fs:[0] ; push eax ; mov dword ptr fs:[0], esp ; push ecx ; mov dword ptr [esp + 0xc], 0 ; call 0x9d8673
c13 | register pressure | DIFFER(68v36) | mov eax, dword ptr [0] ; mov edx, dword ptr [eax + 0x10] ; mov ecx, dword ptr [eax + 8] ; push ebx ; mov ebx, dword ptr [eax + 0xc] ; push ebp ; mov ebp, dword ptr [eax + 0x14] ; push esi
c14 | external swap | DIFFER(12v36) | push 0 ; call 0x9d865a ; pop ecx ; ret
c15 | reference swap | DIFFER(1v36) | ret
c16 | member function | DIFFER(11v36) | push ecx ; lea eax, [esp] ; mov dword ptr [0], eax ; pop ecx ; ret
c17 | static local | DIFFER(21v36) | mov eax, dword ptr [0] ; mov dword ptr [0], eax ; mov dword ptr [0], 0 ; ret
c18 | unoptimised TU | DIFFER(25v36) | push ebp ; mov ebp, esp ; push ecx ; mov eax, dword ptr [0] ; mov dword ptr [ebp - 4], eax ; lea ecx, [ebp - 4] ; mov dword ptr [0], ecx ; mov esp, ebp
c19 | OS fibre | DIFFER(13v36) | mov eax, dword ptr [0] ; push eax ; call dword ptr [0] ; ret
c20 | union pun | DIFFER(11v36) | push ecx ; lea eax, [esp] ; mov dword ptr [0], eax ; pop ecx ; ret
c21 | stackless coroutine | DIFFER(38v36) | mov eax, dword ptr [0] ; xor ecx, ecx ; sub eax, ecx ; je 0x9d866b ; dec eax ; jne 0x9d8675 ; mov dword ptr [0], ecx ; mov dword ptr [0], ecx
c22 | return into frame | DIFFER(25v36) | push ecx ; mov eax, dword ptr [0] ; mov eax, dword ptr [eax + 0x14] ; lea ecx, [esp] ; mov dword ptr [esp], eax ; mov dword ptr [0], ecx ; call eax ; pop ecx
c23 | volatile | DIFFER(27v36) | push ecx ; mov dword ptr [esp], 0 ; mov eax, dword ptr [0] ; lea ecx, [esp] ; mov dword ptr [esp], eax ; mov dword ptr [0], ecx ; pop ecx ; ret
c24 | frame pointer | DIFFER(16v36) | push ebp ; mov ebp, esp ; push ecx ; lea eax, [ebp - 4] ; mov dword ptr [0], eax ; mov esp, ebp ; pop ebp ; ret
c25 | SEH frame | DIFFER(85v36) | push ebp ; mov ebp, esp ; push -1 ; push 0 ; push 0 ; mov eax, dword ptr fs:[0] ; push eax ; mov dword ptr fs:[0], esp
c26 | noreturn call | DIFFER(11v36) | mov ecx, dword ptr [0] ; jmp 0x9d865b
c27 | OS interlock | DIFFER(18v36) | push ecx ; lea eax, [esp] ; push eax ; push 0 ; call dword ptr [0] ; pop ecx ; ret
c28 | forced-inline helper | DIFFER(11v36) | push ecx ; lea eax, [esp] ; mov dword ptr [0], eax ; pop ecx ; ret
c29 | address arithmetic | DIFFER(14v36) | push ecx ; lea eax, [esp] ; sub eax, 0x14 ; mov dword ptr [0], eax ; pop ecx ; ret
c30 | recursion | DIFFER(30v36) | push ecx ; mov eax, dword ptr [0] ; test eax, eax ; lea ecx, [esp] ; mov dword ptr [esp], eax ; mov dword ptr [0], ecx ; je 0x9d866c ; call dword ptr [0]
c31 | tail call | DIFFER(5v36) | jmp 0x9d8655
c32 | struct return | DIFFER(69v36) | mov eax, dword ptr [0] ; mov ecx, dword ptr [eax] ; mov dword ptr [0], ecx ; mov edx, dword ptr [eax + 4] ; mov dword ptr [4], edx ; mov ecx, dword ptr [eax + 8] ; mov dword ptr [8], ecx ; mov edx, dword ptr [eax + 0xc]
i01 | __readfsdword/__writefsdword | DIFFER(23v36) | push ecx ; mov eax, dword ptr [0] ; lea ecx, [esp] ; mov dword ptr [0], ecx ; mov dword ptr fs:[0], eax ; pop ecx ; ret
i02 | __readfsdword/__writefsdword | DIFFER(39v36) | sub esp, 0x18 ; mov eax, dword ptr fs:[0] ; mov dword ptr [esp], eax ; mov eax, dword ptr [0] ; lea ecx, [esp] ; mov dword ptr [0], ecx ; mov edx, dword ptr [eax] ; mov dword ptr fs:[0], edx
i03 | _AddressOfReturnAddress | DIFFER(20v36) | mov ecx, dword ptr [0] ; lea eax, [esp] ; mov dword ptr [0], eax ; mov ecx, dword ptr [ecx + 0x14] ; mov dword ptr [eax], ecx ; ret
i04 | _ReturnAddress | DIFFER(9v36) | mov eax, dword ptr [esp] ; mov dword ptr [0], eax ; ret
i05 | _alloca | DIFFER(70v36) | push ebp ; mov ebp, esp ; mov eax, 0x18 ; call 0x9d865d ; mov eax, esp ; mov dword ptr [eax], 0 ; mov ecx, dword ptr [0] ; mov dword ptr [0], eax
i06 | _alloca | DIFFER(41v36) | push ebp ; mov ebp, esp ; push ecx ; mov ecx, dword ptr [0] ; lea eax, [ebp - 1] ; sub eax, ecx ; add eax, 3 ; and eax, 0xfffffffc
i07 | _InterlockedExchange | DIFFER(13v36) | push ecx ; lea eax, [esp] ; mov ecx, 0 ; xchg dword ptr [ecx], eax ; pop ecx ; ret
i08 | _InterlockedExchange + fs | DIFFER(22v36) | push ecx ; lea eax, [esp] ; mov ecx, 0 ; xchg dword ptr [ecx], eax ; mov edx, dword ptr [eax] ; mov dword ptr fs:[0], edx ; pop ecx ; ret
i09 | _AddressOfReturnAddress + fs | DIFFER(44v36) | mov ecx, dword ptr [0] ; lea eax, [esp] ; push esi ; mov esi, dword ptr fs:[0] ; lea edx, [eax - 4] ; mov dword ptr [edx], esi ; mov dword ptr [0], edx ; mov edx, dword ptr [ecx]
i10 | _AddressOfReturnAddress + memcpy | DIFFER(52v36) | mov ecx, dword ptr [0] ; lea eax, [esp] ; add eax, -0x14 ; mov dword ptr [0], eax ; mov edx, dword ptr [ecx] ; mov dword ptr [eax], edx ; mov edx, dword ptr [ecx + 4] ; mov dword ptr [eax + 4], edx
i11 | _AddressOfReturnAddress /Oy- | DIFFER(31v36) | push ebp ; mov ebp, esp ; mov eax, dword ptr [0] ; lea ecx, [ebp + 4] ; add ecx, 4 ; mov dword ptr [0], ecx ; mov edx, dword ptr [eax] ; mov dword ptr fs:[0], edx
i12 | _alloca + _AddressOfReturnAddress | DIFFER(56v36) | push ebp ; mov ebp, esp ; mov eax, 0x14 ; call 0x9d865d ; mov ecx, dword ptr fs:[0] ; mov eax, esp ; mov dword ptr [eax], ecx ; mov ecx, dword ptr [0]
nn1 asm core, normal function | DIFFER(28v36) | core + ret, no saves
nn2 full 15 instr, normal function | DIFFER(42v36) | compiler push ebx/esi/edi + block + pops
nn3 core + xor ebx/esi/edi | DIFFER(40v36) | compiler saves ebx esi edi, not ebp
nn4 core + xor ebx/ebp/esi/edi | DIFFER(42v36) | ebp still not saved
row (whole function, header macro) | RELOCATION_MATCH | 36 bytes
