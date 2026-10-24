// Grade: DIFFER(13v17) -- IMPOSSIBLE (CRT hand-written asm): __SEH_epilog is CRT sehprolg.asm: it restores fs:[0] from [ebp-10h], pops the return address into ecx, pops edi/esi/ebx of the CALLER's frame, `leave`s the caller's frame and returns with `push ecx ; ret`. No compiled C++ function pops its own return address or tears down its caller's frame, and VC7.1 has no fs-segment intrinsic (__readfsdword/__writefsdword arrive in VC8); this model only unlinks a record. One grade.
// __SEH_epilog -- CRT sehprolg.asm: unlink the frame's SEH record, restore edi/esi/ebx, leave, return to the caller's caller
// Leaf: SehEpilogModel
struct SEHRecord { SEHRecord* Next; void* Handler; };
extern SEHRecord* g_threadSehChainModel;    // models fs:[0]; VC7.1 has no fs-segment intrinsic (__writefsdword is VC8)
extern "C" void __cdecl SehEpilogModel(SEHRecord* frameRecord)
{
    g_threadSehChainModel = frameRecord->Next;
}
