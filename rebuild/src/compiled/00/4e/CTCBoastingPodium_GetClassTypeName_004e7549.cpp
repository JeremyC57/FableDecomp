#pragma optimize("s",on)
// CTCBoastingPodium::GetClassTypeName() const -- calls CTCBoastingPodium::StatGetClassTypeName @ 0x004D46D0 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d46d0(); };
extern "C" T* __stdcall CTCBoastingPodium_GetClassTypeName(T* result) { result->Init_004d46d0(); return result; }