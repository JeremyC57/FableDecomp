#pragma optimize("s",on)
// CTCActionUsePickUp::GetClassTypeName() const -- calls CTCActionUsePickUp::StatGetClassTypeName @ 0x004D2BA0 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2ba0(); };
extern "C" T* __stdcall CTCActionUsePickUp_GetClassTypeName(T* result) { result->Init_004d2ba0(); return result; }