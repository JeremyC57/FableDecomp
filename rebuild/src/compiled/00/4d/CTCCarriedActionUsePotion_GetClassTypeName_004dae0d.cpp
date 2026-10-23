#pragma optimize("s",on)
// CTCCarriedActionUsePotion::GetClassTypeName() const -- calls CTCCarriedActionUsePotion::StatGetClassTypeName @ 0x004D2D4C into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2d4c(); };
extern "C" T* __stdcall CTCCarriedActionUsePotion_GetClassTypeName(T* result) { result->Init_004d2d4c(); return result; }