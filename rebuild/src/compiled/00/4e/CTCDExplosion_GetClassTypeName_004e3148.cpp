#pragma optimize("s",on)
// CTCDExplosion::GetClassTypeName() const -- calls CTCDExplosion::StatGetClassTypeName @ 0x004D5CE6 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d5ce6(); };
extern "C" T* __stdcall CTCDExplosion_GetClassTypeName(T* result) { result->Init_004d5ce6(); return result; }