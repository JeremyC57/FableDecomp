#pragma optimize("s",on)
// CTCActionUseReadable::GetClassTypeName() const -- calls CTCActionUseReadable::StatGetClassTypeName @ 0x004D2B8D into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2b8d(); };
extern "C" T* __stdcall CTCActionUseReadable_GetClassTypeName(T* result) { result->Init_004d2b8d(); return result; }