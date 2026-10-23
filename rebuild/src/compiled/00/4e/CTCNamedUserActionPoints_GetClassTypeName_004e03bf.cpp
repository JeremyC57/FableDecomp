#pragma optimize("s",on)
// CTCNamedUserActionPoints::GetClassTypeName() const -- calls CTCNamedUserActionPoints::StatGetClassTypeName @ 0x004D34ED into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d34ed(); };
extern "C" T* __stdcall CTCNamedUserActionPoints_GetClassTypeName(T* result) { result->Init_004d34ed(); return result; }