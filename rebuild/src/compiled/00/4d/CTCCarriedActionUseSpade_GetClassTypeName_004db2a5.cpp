#pragma optimize("s",on)
// CTCCarriedActionUseSpade::GetClassTypeName() const -- calls CTCCarriedActionUseSpade::StatGetClassTypeName @ 0x004D5D10 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d5d10(); };
extern "C" T* __stdcall CTCCarriedActionUseSpade_GetClassTypeName(T* result) { result->Init_004d5d10(); return result; }