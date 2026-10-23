#pragma optimize("s",on)
// CTCActionUseSign::GetClassTypeName() const -- calls CTCActionUseSign::StatGetClassTypeName @ 0x004D2B7A into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2b7a(); };
extern "C" T* __stdcall CTCActionUseSign_GetClassTypeName(T* result) { result->Init_004d2b7a(); return result; }