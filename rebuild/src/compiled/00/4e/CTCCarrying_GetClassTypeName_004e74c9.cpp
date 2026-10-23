#pragma optimize("s",on)
// CTCCarrying::GetClassTypeName() const -- calls CTCCarrying::StatGetClassTypeName @ 0x004D3D56 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d3d56(); };
extern "C" T* __stdcall CTCCarrying_GetClassTypeName(T* result) { result->Init_004d3d56(); return result; }