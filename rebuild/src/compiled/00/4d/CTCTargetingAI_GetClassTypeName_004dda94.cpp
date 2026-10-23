#pragma optimize("s",on)
// CTCTargetingAI::GetClassTypeName() const -- calls CTCTargetingAI::StatGetClassTypeName @ 0x004D2A74 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2a74(); };
extern "C" T* __stdcall CTCTargetingAI_GetClassTypeName(T* result) { result->Init_004d2a74(); return result; }