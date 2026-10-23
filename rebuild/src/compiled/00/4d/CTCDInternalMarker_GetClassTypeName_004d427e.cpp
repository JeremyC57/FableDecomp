#pragma optimize("s",on)
// CTCDInternalMarker::GetClassTypeName() const -- calls CTCDInternalMarker::StatGetClassTypeName @ 0x004D4249 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d4249(); };
extern "C" T* __stdcall CTCDInternalMarker_GetClassTypeName(T* result) { result->Init_004d4249(); return result; }