#pragma optimize("s",on)
// CTCDestroyThingOnSpecialAbilityFinish::GetClassTypeName() const -- calls CTCDestroyThingOnSpecialAbilityFinish::StatGetClassTypeName @ 0x004D63A7 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d63a7(); };
extern "C" T* __stdcall CTCDestroyThingOnSpecialAbilityFinish_GetClassTypeName(T* result) { result->Init_004d63a7(); return result; }