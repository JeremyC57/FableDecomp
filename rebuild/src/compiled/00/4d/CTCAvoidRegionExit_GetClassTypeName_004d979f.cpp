#pragma optimize("s",on)
// CTCAvoidRegionExit::GetClassTypeName() const -- calls CTCAvoidRegionExit::StatGetClassTypeName @ 0x004D65A9 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d65a9(); };
extern "C" T* __stdcall CTCAvoidRegionExit_GetClassTypeName(T* result) { result->Init_004d65a9(); return result; }