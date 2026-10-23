#pragma optimize("s",on)
// CTCActionUseBlowHorn::GetClassTypeName() const -- calls CTCActionUseBlowHorn::StatGetClassTypeName @ 0x004D2BFF into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2bff(); };
extern "C" T* __stdcall CTCActionUseBlowHorn_GetClassTypeName(T* result) { result->Init_004d2bff(); return result; }