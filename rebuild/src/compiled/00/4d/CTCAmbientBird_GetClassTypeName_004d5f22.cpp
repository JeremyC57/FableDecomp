#pragma optimize("s",on)
// CTCAmbientBird::GetClassTypeName() const -- calls CTCAmbientBird::StatGetClassTypeName @ 0x004D5EE8 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d5ee8(); };
extern "C" T* __stdcall CTCAmbientBird_GetClassTypeName(T* result) { result->Init_004d5ee8(); return result; }