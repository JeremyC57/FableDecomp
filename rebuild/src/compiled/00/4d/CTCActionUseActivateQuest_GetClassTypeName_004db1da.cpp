#pragma optimize("s",on)
// CTCActionUseActivateQuest::GetClassTypeName() const -- calls CTCActionUseActivateQuest::StatGetClassTypeName @ 0x004D57FA into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init(); };
extern "C" T* __stdcall CTCActionUseActivateQuest_GetClassTypeName(T* result) { result->Init(); return result; }