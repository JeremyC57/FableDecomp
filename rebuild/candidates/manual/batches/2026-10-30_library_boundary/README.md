# Manifest boundary pass (2026-10-30): the 312 vendor functions outside the catalog added to the manifest and landed as `toolchain_library`

Owner (2026-10-30): "do the 312 and fix the manifest". Follows the signature pass (`../2026-10-30_library_signatures/`) and the
landing pass (`../2026-10-30_library_landing/`, whose OUTSIDE_CATALOG.tsv was the input). Brief BRIEF.md; ROWS.tsv (312 library
rows, 30 rechecked caller / funclet rows, 3 rows kept on stand-ins); MANIFEST_ADDED.tsv; BINDINGS.tsv (32 retired, 5 added,
4 kept); NAME_FIXES.tsv (12); LESSONS.md; logs/ (lib_verify and vland logs for all rows, metrics before/after, the Rich header
probe, the stand-in census).
Manifest: 312 boundary rows (307 linker import thunks, 5 CRT functions: _check_managed_app, the RTTI-selected type_info copy
ctor, ??_Etype_info, ArrayUnwindFilter, ___security_init_cookie) appended through `rebuild/manifest/boundary-starts.tsv` +
`crawl/manifest_add_boundaries.py --write` (functions.tsv 57,267 -> 57,579, insertions only, sorted; name = decorated library
symbol, module = the DLL or `msvcrt.lib(<member>)`). No collision with landed code; three unlanded host spans shortened
(00f3fbe0, 00c843f0, 00c733d0). Landed: 312 stubs, catalog entries, auto-re rows; lib_verify 312/312 MATCH (440/440 over every
toolchain_library row), vland --recheck-landed 312/312 LIBRARY_MATCH; 0x00BFEFE6 (d3dx9_25 D3DXLoadVolumeFromMemory) proves
through the import-directory path since d3dx9.lib is not on disk. Coordinator: 10 random proofs MATCH; cherry-pick conflicts in
the generated files and the line-set oracles resolved by regeneration / three-way line sets; multi-check 0.
Callers: only two landed rows call the thunks directly (both already right); 28 rows reached the imports through stand-in
globals or invented names on the IAT slots; 25 respelled to the real `__declspec(dllimport)` API (WriteFile, VirtualAlloc/Free,
the CriticalSection family, GetTickCount where the source wrongly said timeGetTime, ClientToScreen/GetClientRect/OffsetRect/
SetCursorPos, dllimport `exception` and `std::basic_string<char>` for the CImported* placeholders, Sleep, CloseHandle,
CoUninitialize, timeKillEvent, WaitForSingleObject, &SendMessageA) and rechecked with 5 EH funclets: 30/30 RELOCATION_MATCH
PASS. 3 callers stay on stand-ins because their behaviour tests fake the import through the stand-in global (0099abd0
SetFilePointer, 009f26b0 Enter/LeaveCriticalSection, 00c36150 WaitForSingleObject): test rewrites first (NAME_FIXES).
Rich header (`tools/decomp_pipeline/rich_header.py`, evidence in logs/rich_header_probe.txt): prodid 0x5F/0x60 are Utc1310 C /
C++, 0x5D Implib710, 0x5A Linker710 build 3077, 0x5E Cvtres710 build 3052 (the old table called 3052 the linker); the "11 QFE
objects" are 5 compiled C/C++ objects + 6 import descriptors, and the WinDDK uuid/dxguid/dinput8 libs carry the same 4035 IDs,
so the 5 may be GUID library objects, not game code (open). CLAUDE.md, BUILDING.md, HANDOFF.md and the pipeline handoffs
corrected accordingly.
Dashboard: toolchain_library 128 -> 440, catalog rows +312, known functions +312, landed genuine known +312; original-catalog
counts unchanged. `update_readme_progress.py` now rewrites the known-function count inside its README row label before
matching (adding manifest rows used to break it).
Open (NAME_FIXES): 0x00CB0C10 is landed as the `_timeSetEvent@20` thunk but is DirectShow's CompatibleTimeSetEvent wrapper
(the real thunk is 0x00F3FE0A, landed here); its caller 0x00CA6F90 follows. Proposed names: 009f2000
CMemoryAllocator::EnterSynchronisation, 00ca4860/00ca4870 CCritSec ctor/dtor, 00ca48c0 ~CAutoLock, 00ca4d60
CBaseRenderer::ResetEndOfStreamTimer, the 00c5e3d0/00c5e420/00c5e480 handle-holder dtors.
