#pragma optimize("s",on)
// CTCDecapitation::GetClassTypeName() const -- calls CTCDecapitation::StatGetClassTypeName @ 0x004D5AF0 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d5af0(); };
extern "C" T* __stdcall CTCDecapitation_GetClassTypeName(T* result) { result->Init_004d5af0(); return result; }