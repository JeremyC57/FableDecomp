#pragma optimize("s",on)
// CTCDRegionEntrance::GetClassTypeName() const -- calls CTCDRegionEntrance::StatGetClassTypeName @ 0x004D429B into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d429b(); };
extern "C" T* __stdcall CTCDRegionEntrance_GetClassTypeName(T* result) { result->Init_004d429b(); return result; }