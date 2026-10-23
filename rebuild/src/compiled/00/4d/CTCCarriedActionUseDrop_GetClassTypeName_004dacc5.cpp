#pragma optimize("s",on)
// CTCCarriedActionUseDrop::GetClassTypeName() const -- calls CTCCarriedActionUseDrop::StatGetClassTypeName @ 0x004D2CB4 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2cb4(); };
extern "C" T* __stdcall CTCCarriedActionUseDrop_GetClassTypeName(T* result) { result->Init_004d2cb4(); return result; }