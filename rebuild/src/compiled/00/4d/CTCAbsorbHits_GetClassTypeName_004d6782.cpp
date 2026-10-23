#pragma optimize("s",on)
// CTCAbsorbHits::GetClassTypeName() const -- calls CTCAbsorbHits::StatGetClassTypeName @ 0x004D674D into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d674d(); };
extern "C" T* __stdcall CTCAbsorbHits_GetClassTypeName(T* result) { result->Init_004d674d(); return result; }