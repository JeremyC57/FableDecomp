#pragma optimize("s",on)
// CTCActionUseSwitch::GetClassTypeName() const -- calls CTCActionUseSwitch::StatGetClassTypeName @ 0x004D2D85 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2d85(); };
extern "C" T* __stdcall CTCActionUseSwitch_GetClassTypeName(T* result) { result->Init_004d2d85(); return result; }