#pragma optimize("s",on)
// CTCPreCalculatedNavigationRoute::GetClassTypeName() const -- calls CTCPreCalculatedNavigationRoute::StatGetClassTypeName @ 0x004D4D4B into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d4d4b(); };
extern "C" T* __stdcall CTCPreCalculatedNavigationRoute_GetClassTypeName(T* result) { result->Init_004d4d4b(); return result; }