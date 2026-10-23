#pragma optimize("s",on)
// CTCDNavigationSeed::GetClassTypeName() const -- calls CTCDNavigationSeed::StatGetClassTypeName @ 0x004D41DF into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d41df(); };
extern "C" T* __stdcall CTCDNavigationSeed_GetClassTypeName(T* result) { result->Init_004d41df(); return result; }