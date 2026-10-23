#pragma optimize("s",on)
// CTCAnimatingObject::GetClassTypeName() const -- calls CTCAnimatingObject::StatGetClassTypeName @ 0x004D4A56 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d4a56(); };
extern "C" T* __stdcall CTCAnimatingObject_GetClassTypeName(T* result) { result->Init_004d4a56(); return result; }