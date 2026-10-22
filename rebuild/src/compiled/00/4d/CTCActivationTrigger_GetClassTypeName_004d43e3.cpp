#pragma optimize("s",on)
// CTCActivationTrigger::GetClassTypeName() const -- calls CTCActivationTrigger::StatGetClassTypeName @ 0x004D43A6 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init(); };
extern "C" T* __stdcall CTCActivationTrigger_GetClassTypeName(T* result) { result->Init(); return result; }