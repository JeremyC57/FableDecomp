#pragma optimize("s",on)
// CTCCarriedActionUseAbility::GetClassTypeName() const -- calls CTCCarriedActionUseAbility::StatGetClassTypeName @ 0x004D673A into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d673a(); };
extern "C" T* __stdcall CTCCarriedActionUseAbility_GetClassTypeName(T* result) { result->Init_004d673a(); return result; }