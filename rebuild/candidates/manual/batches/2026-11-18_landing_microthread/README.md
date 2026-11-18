# 2026-11-18 landing microthread: 0x009D8650 MicroThread::MicroThreadSleep (36 bytes, retail_asm) with a portable twin that stands on its own

Owner: "As long as it does not affect matching or portability, it should be fine" (the whole-function asm helper rule merged from
FullDecomp-B), then "fix it as you see fit based on my rules" (the row's portable twin was not portable on its own).

## The portability fix (engine/MicroThreadSleep.h, new)
The Ego_d lane's twin kept retail's design, one shared stack with each thread's image copied under its top, and so needed a stack
copied to the same address (frame-layout dependent: x86-64 SysV only) and had no portable CreateMicroThread. Now the portable build
gives every micro-thread its own context and stack, held in the thread's `Stack` vector (MicroThreadDetails +8):
- `MicroThreadInit` (CreateMicroThread's portable branch) makes a POSIX ucontext on a 256 KB stack inside the image, or a Win32 fibre;
  its start routine calls entry(argument) then MicroThreadReturn, as retail's fabricated frame does.
- SwitchToMicroThread's portable RestoreStack / PreserveStack move the resume token (`MicroThreadResumeToken` / `MicroThreadParkToken`)
  instead of copying an image; `MicroThreadSleep`'s twin (swapcontext / SwitchToFiber) is the protocol of the asm path.
- ~MicroThreadDetails's portable branch releases a Win32 fibre (`MicroThreadRelease`).
No stack is copied and nothing depends on frame layout or one ABI. Hosts: POSIX ucontext (any ABI) and Win32 fibres.

## Checks
- Callers' matching bytes unchanged: `vland --recheck-landed`, strict headers, 0x009D8710 / 0x009D87F0 / 0x009D88D0 RELOCATION_MATCH
  PASS (`logs/recheck_callers.log`).
- 0x009D8650: dry run and landing RELOCATION_MATCH + PASS, strict headers (`logs/vland_dry.log`, `logs/vland_land.log`); purity
  `retail_asm` (crawl/purity.py's whole-function helper rule).
- Dual-path test `rebuild/tests/portable/test_MicroThreadSleep.cpp`: asm path S + C, Win32 twin S + P (P = the portable module
  protocol through the header API, as the three callers use it), POSIX twin S + P; `portable_twins_check.py`: vc71-asm / dual /
  portable PASS, gxx-portable PASS, the three callers vc71-row-portable OK (`logs/portable_twins.tsv`); also clang++ -O0 / -O2 / -O3
  PASS. (The run's one failure, the 0x00B98E10 ClipSubSectionsSIMD row-dual driver, includes none of these files.)

## Name fix
0x013CA898 was `SavedStackPointer` in SwitchToMicroThread and `PGlobalStackPointer` in the new header; it is `PGlobalStackPointer`
(Ego_d.pdb S_LDATA32 in lib_microthread.obj + the Anniversary PDB), respelled in the row, its test and reloc_bindings.tsv.
