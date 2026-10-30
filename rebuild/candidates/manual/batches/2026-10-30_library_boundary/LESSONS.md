# libsig3 lessons (manifest boundary pass, 2026-10-30)

1. **Thunk vs. slot callers.** Only two landed rows call the 312 `FF 25` thunks (00401067 `__XcptFilter`,
   00bfee7a `?terminate@@YAXXZ`), and both already spelled them right. All stand-in names sat on the IAT
   *slot* (`call [0x0143xxxx]`), so a caller census has to walk slot relocations, not just thunk calls.
   The census (logs/slot_standin_census.tsv) found 28 rows in 24 families.
2. **Check test self-containment before respelling.** vland's behaviour_of links the test alone and falls
   back to linking the row object. A test that fakes an import through the stand-in global breaks, or calls
   the real API, once the row binds `__imp__X`. 009f26b0's test hung in a real EnterCriticalSection, and
   0099abd0's test failed. Grepping tests for `#include` is not enough: link the test alone
   (work/selfcont.py, logs/test_selfcontained_check.txt) and leave NEEDS_SOURCE rows alone until their
   test is rewritten.
3. **vland behaviour runs have no per-test timeout.** Wrap the whole run in `timeout`. A hung test exe
   keeps the batch alive until the outer timeout kills the process group.
4. **The README row label embeds the known-function count** ("all N known functions").
   update_readme_progress.py now respells that number before its exact-match replacements, so adding
   manifest rows no longer raises "README row not found".
5. **The Rich header prodid labels were off by one family.** 0x5F/0x60 are Utc1310 C/C++, 0x5D is Implib710,
   0x5A is Linker710, and 0x5E is Cvtres710 (build 3052, *not* the linker; the linker is 3077). Proven by
   compiling and linking probes with both toolsets and reading the @comp.id of on-disk libs
   (logs/rich_header_probe.txt). The "11 QFE objects" are 4 C + 1 C++ compiled objects and 6 import
   descriptors. WinDDK 3790 uuid.lib, dxguid.lib and dinput8.lib carry 0x5F/0x60 at build 4035, so the five
   4035 compiled objects may be library GUID objects rather than game code. This is still open.
6. **A library member missing from disk can still be proven.** d3dx9_25.lib is not on disk. The claim
   `d3dx9.lib(d3dx9_25.dll) _D3DXLoadVolumeFromMemory@44` verifies through lib_verify's NOT_ON_DISK path
   from the thunk shape, the IAT slot and the import name in the PE import table.
7. **One retail address, one name.** The catalog had WINMM.DLL::timeSetEvent twice. The real thunk is
   0x00F3FE0A, and the already-landed 0x00CB0C10 is CompatibleTimeSetEvent. Landed non-caller rows are
   out of scope here, so this went to NAME_FIXES.tsv.
8. **Never filter addresses with a loose prefix grep.** `grep -v "^012"` meant to drop funclets also dropped
   01224c30. Filter on an explicit address list.
