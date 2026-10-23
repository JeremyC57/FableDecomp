#pragma optimize("s",on)
// CTCCameraPointTrack::GetClassTypeName() const -- calls CTCCameraPointTrack::StatGetClassTypeName @ 0x004D3AE1 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d3ae1(); };
extern "C" T* __stdcall CTCCameraPointTrack_GetClassTypeName(T* result) { result->Init_004d3ae1(); return result; }