#pragma optimize("s",on)
// CTCKraken::GetClassTypeName() const -- calls CTCKraken::StatGetClassTypeName @ 0x004D5DC2 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d5dc2(); };
extern "C" T* __stdcall CTCKraken_GetClassTypeName(T* result) { result->Init_004d5dc2(); return result; }