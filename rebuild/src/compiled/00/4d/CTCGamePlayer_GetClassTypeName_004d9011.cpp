#pragma optimize("s",on)
// CTCGamePlayer::GetClassTypeName() const -- calls CTCGamePlayer::StatGetClassTypeName @ 0x004D5ADD into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d5add(); };
extern "C" T* __stdcall CTCGamePlayer_GetClassTypeName(T* result) { result->Init_004d5add(); return result; }