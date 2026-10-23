#pragma optimize("s",on)
// CTCUndeadSoul::GetClassTypeName() const -- calls CTCUndeadSoul::StatGetClassTypeName @ 0x004D2DC8 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2dc8(); };
extern "C" T* __stdcall CTCUndeadSoul_GetClassTypeName(T* result) { result->Init_004d2dc8(); return result; }