#pragma optimize("s",on)
// Leaf: DeleteData_0050ceb0
// DeleteData_0050ceb0 @ 0050ceb0 (catalog name CCountedPointer::DeleteData)
// push ecx ; call 0x00BFE9BC ; pop ecx ; ret -- the pointer in ecx is handed to the scalar operator delete
// (0x00BFE9BC is the MSVCR71 ??3@YAXPAX@Z import thunk; the row was first landed as a copy of 0x004C73B0's
// allocator stand-in `operator_new` calling an invented `helper_bfeb1c`).
// The catalog names the row CCountedPointer::DeleteData; the member form (class, `this` convention) is unproven,
// so the function keeps a descriptive free-function name.

void __fastcall DeleteData_0050ceb0(void* p)
{
    ::operator delete(p);
}
