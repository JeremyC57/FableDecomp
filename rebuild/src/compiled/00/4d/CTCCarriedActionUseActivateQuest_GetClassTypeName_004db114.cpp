#pragma optimize("s",on)
// CTCCarriedActionUseActivateQuest::GetClassTypeName() const -- calls CTCCarriedActionUseActivateQuest::StatGetClassTypeName @ 0x004D5043 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d5043(); };
extern "C" T* __stdcall CTCCarriedActionUseActivateQuest_GetClassTypeName(T* result) { result->Init_004d5043(); return result; }