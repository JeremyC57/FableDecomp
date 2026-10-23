#pragma optimize("s",on)
// CTCCarriedActionUseShow::GetClassTypeName() const -- calls CTCCarriedActionUseShow::StatGetClassTypeName @ 0x004D2D13 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2d13(); };
extern "C" T* __stdcall CTCCarriedActionUseShow_GetClassTypeName(T* result) { result->Init_004d2d13(); return result; }