#pragma optimize("s",on)
// Leaf: DeleteArray_004c73b0
// DeleteArray_004c73b0 @ 004c73b0 (catalog name _global::DeleteArray_004c73b0)
// push ecx ; call 0x00BFEB1C ; pop ecx ; ret -- the pointer in ecx is handed to operator delete[]
// (0x00BFEB1C is the MSVCR71 ??_V@YAXPAX@Z import thunk; the row was first landed as an allocator stand-in
// `operator_new` calling an invented `helper_bfeb1c_00bfeb1c`).

void __cdecl operator delete[](void* p);           // 0x00BFEB1C MSVCR71 ??_V@YAXPAX@Z (import thunk); VC7.1 predeclares only scalar new/delete

void __fastcall DeleteArray_004c73b0(void* p)
{
    ::operator delete[](p);
}