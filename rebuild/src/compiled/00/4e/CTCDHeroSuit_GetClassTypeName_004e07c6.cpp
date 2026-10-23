#pragma optimize("s",on)
// CTCDHeroSuit::GetClassTypeName() const -- calls CTCDHeroSuit::StatGetClassTypeName @ 0x004D4232 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d4232(); };
extern "C" T* __stdcall CTCDHeroSuit_GetClassTypeName(T* result) { result->Init_004d4232(); return result; }