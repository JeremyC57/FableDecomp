#pragma optimize("s",on)
// CTCCarriedActionUseGive::GetClassTypeName() const -- calls CTCCarriedActionUseGive::StatGetClassTypeName @ 0x004D2CDA into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2cda(); };
extern "C" T* __stdcall CTCCarriedActionUseGive_GetClassTypeName(T* result) { result->Init_004d2cda(); return result; }