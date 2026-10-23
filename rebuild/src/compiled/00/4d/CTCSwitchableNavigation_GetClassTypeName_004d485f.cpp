#pragma optimize("s",on)
// CTCSwitchableNavigation::GetClassTypeName() const -- calls CTCSwitchableNavigation::StatGetClassTypeName @ 0x004D482C into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d482c(); };
extern "C" T* __stdcall CTCSwitchableNavigation_GetClassTypeName(T* result) { result->Init_004d482c(); return result; }