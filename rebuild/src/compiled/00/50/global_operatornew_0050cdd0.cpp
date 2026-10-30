#pragma optimize("s",on)
// Leaf: OperatorDelete_thunk_0050cdd0
// OperatorDelete_thunk_0050cdd0 @ 0050cdd0 (catalog name _global::OperatorDelete_thunk_0050cdd0)
// push ecx ; call 0x00BFE9BC ; pop ecx ; ret -- the pointer in ecx is handed to the scalar operator delete
// (0x00BFE9BC is the MSVCR71 ??3@YAXPAX@Z import thunk; the row was first landed as a copy of 0x004C73B0's
// allocator stand-in `operator_new` calling an invented `helper_bfeb1c`).

void __fastcall OperatorDelete_thunk_0050cdd0(void* p)
{
    ::operator delete(p);
}
