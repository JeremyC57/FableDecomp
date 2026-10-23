#pragma optimize("s",on)
// CTCCarriedActionUseFood::GetClassTypeName() const -- calls CTCCarriedActionUseFood::StatGetClassTypeName @ 0x004D2D26 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2d26(); };
extern "C" T* __stdcall CTCCarriedActionUseFood_GetClassTypeName(T* result) { result->Init_004d2d26(); return result; }