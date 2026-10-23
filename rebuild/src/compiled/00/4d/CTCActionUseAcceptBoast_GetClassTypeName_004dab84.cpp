#pragma optimize("s",on)
// CTCActionUseAcceptBoast::GetClassTypeName() const -- calls CTCActionUseAcceptBoast::StatGetClassTypeName @ 0x004D2C55 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2c55(); };
extern "C" T* __stdcall CTCActionUseAcceptBoast_GetClassTypeName(T* result) { result->Init_004d2c55(); return result; }