#pragma optimize("s",on)
// CTCBlacksmiths::GetClassTypeName() const -- calls CTCBlacksmiths::StatGetClassTypeName @ 0x004D40AC into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d40ac(); };
extern "C" T* __stdcall CTCBlacksmiths_GetClassTypeName(T* result) { result->Init_004d40ac(); return result; }