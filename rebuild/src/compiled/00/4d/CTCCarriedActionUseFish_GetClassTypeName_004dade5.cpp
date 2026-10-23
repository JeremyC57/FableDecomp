#pragma optimize("s",on)
// CTCCarriedActionUseFish::GetClassTypeName() const -- calls CTCCarriedActionUseFish::StatGetClassTypeName @ 0x004D2D39 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2d39(); };
extern "C" T* __stdcall CTCCarriedActionUseFish_GetClassTypeName(T* result) { result->Init_004d2d39(); return result; }