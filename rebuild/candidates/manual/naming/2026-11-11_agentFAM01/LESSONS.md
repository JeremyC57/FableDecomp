# agentFAM01 lessons (one line each)
- VC7.1 emits a COMDAT for every referenced inline even when every call is inlined, and retail links /OPT:NOREF: 692 of 714 release bodies have no reference at all, so position, not callers, is all that is left for them.
- With `~CCountedPointer() { ReleaseData(); }` a speed TU emits ??1<T> and ReleaseData<T> as identical bodies; byte twins of one T are not proof of two Ts.
- The called member depends on the CALLER's mode: /O1 scope ends and scalar deleting dtors call ReleaseData (dtor = jmp thunk), /O2 ones call ??1 (ReleaseData inlined into it); ShareData / Reset always call ReleaseData.
- Size-TU code is unpadded (function starts not 16-aligned, no 0xCC before) and uses `pop ecx` / `and [m],0`; speed-TU code is CC-padded with `add esp,4` / `mov [m],0`: enough to classify a caller (work/classify.py).
- A CCountedPointer's T is cheapest from the caller's member offset in Ego_r (scalar deleting dtor `lea ecx,[esi+N]`) or the singleton static it touches (`mov ecx,0x013Bxxxx` + the PDB-named Get that reads it).
- ShareData rows are all labelled `<NUISystem::CAction>` by propagation; the ReleaseData each one calls gives its real T.
- A 5-byte `jmp` right after a run of size-TU release bodies is the destructor of the matching T (same order).
- Tests that include rebuild headers cannot build on 64-bit g++ (ptr32 assert): mirror the header members under `#ifndef _MSC_VER`, and null-check `operator new` by hand (g++ C++11 assumes new never returns 0).
- In a test that replaces global operator new/delete, arm the hooks only around the checks or libstdc++'s own allocations crash it.
