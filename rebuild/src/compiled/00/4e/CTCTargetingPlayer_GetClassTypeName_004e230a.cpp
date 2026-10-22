#pragma optimize("s",on)
// CTCTargetingPlayer::GetClassTypeName() const -- calls CTCTargetingPlayer::StatGetClassTypeName @ 0x0049D450 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init(); };
extern "C" T* __stdcall CTCTargetingPlayer_GetClassTypeName(T* result) { result->Init(); return result; }