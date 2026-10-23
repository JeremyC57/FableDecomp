#pragma optimize("s",on)
// CTCSinglePersonOccupiable::GetClassTypeName() const -- calls CTCSinglePersonOccupiable::StatGetClassTypeName @ 0x004D4E53 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d4e53(); };
extern "C" T* __stdcall CTCSinglePersonOccupiable_GetClassTypeName(T* result) { result->Init_004d4e53(); return result; }