// Grade: DIFFER(39v59) -- IMPOSSIBLE (CRT hand-written asm): __SEH_prolog is CRT sehprolg.asm: it builds the CALLER's SEH frame on the caller's stack (`push 004013D8h` _except_handler3, swaps the scope-table push with the saved ebp, `lea ebp,[esp+10h] ; sub esp,eax ; push ebx/esi/edi`, links fs:[0]) and returns into the caller with a moved esp. A compiled C++ function cannot leave esp/ebp changed for its caller, and VC7.1 has no fs-segment intrinsic; this model only links a record. One grade.
// __SEH_prolog -- CRT sehprolg.asm: build the caller's SEH frame (handler 0x004013D8, scope table from the caller's push),
// allocate its locals, save ebx/esi/edi, and link the record into fs:[0]
// Leaf: SehPrologModel
struct SEHRecord { SEHRecord* Next; void* Handler; void* ScopeTable; long TryLevel; };
extern SEHRecord* g_threadSehChainModel;    // models fs:[0]; VC7.1 has no fs-segment intrinsic (__readfsdword is VC8)
extern "C" int __cdecl _except_handler3(void*, void*, void*, void*);   // 0x004013D8
extern "C" void __cdecl SehPrologModel(SEHRecord* record, void* scopeTable)
{
    record->Next = g_threadSehChainModel;
    record->Handler = (void*)_except_handler3;
    record->ScopeTable = scopeTable;
    record->TryLevel = -1;
    g_threadSehChainModel = record;
}
