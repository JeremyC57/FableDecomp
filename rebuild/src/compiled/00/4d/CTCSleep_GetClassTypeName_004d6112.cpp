#pragma optimize("s",on)
// CTCSleep::GetClassTypeName() const -- calls CTCSleep::StatGetClassTypeName @ 0x004D60D2 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d60d2(); };
extern "C" T* __stdcall CTCSleep_GetClassTypeName(T* result) { result->Init_004d60d2(); return result; }