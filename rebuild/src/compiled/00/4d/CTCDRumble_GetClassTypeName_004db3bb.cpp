#pragma optimize("s",on)
// CTCDRumble::GetClassTypeName() const -- calls CTCDRumble::StatGetClassTypeName @ 0x004D5FF2 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d5ff2(); };
extern "C" T* __stdcall CTCDRumble_GetClassTypeName(T* result) { result->Init_004d5ff2(); return result; }