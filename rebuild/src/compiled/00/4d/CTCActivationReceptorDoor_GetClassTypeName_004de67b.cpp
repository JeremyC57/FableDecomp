#pragma optimize("s",on)
// CTCActivationReceptorDoor::GetClassTypeName() const -- calls CTCActivationReceptorDoor::StatGetClassTypeName @ 0x004D4380 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init(); };
extern "C" T* __stdcall CTCActivationReceptorDoor_GetClassTypeName(T* result) { result->Init(); return result; }