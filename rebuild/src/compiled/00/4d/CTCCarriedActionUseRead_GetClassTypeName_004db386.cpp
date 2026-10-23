#pragma optimize("s",on)
// CTCCarriedActionUseRead::GetClassTypeName() const -- calls CTCCarriedActionUseRead::StatGetClassTypeName @ 0x004D5EB7 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d5eb7(); };
extern "C" T* __stdcall CTCCarriedActionUseRead_GetClassTypeName(T* result) { result->Init_004d5eb7(); return result; }