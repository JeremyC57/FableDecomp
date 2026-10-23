#pragma optimize("s",on)
// CTCTavernJug::GetClassTypeName() const -- calls CTCTavernJug::StatGetClassTypeName @ 0x004D51B5 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d51b5(); };
extern "C" T* __stdcall CTCTavernJug_GetClassTypeName(T* result) { result->Init_004d51b5(); return result; }