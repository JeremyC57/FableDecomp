#pragma optimize("s",on)
// CTCDExplosiveTrail::GetClassTypeName() const -- calls CTCDExplosiveTrail::StatGetClassTypeName @ 0x004D66C3 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init(); };
extern "C" T* __stdcall CTCDExplosiveTrail_GetClassTypeName(T* result) { result->Init(); return result; }