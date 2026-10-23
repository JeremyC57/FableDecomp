#pragma optimize("s",on)
// CTCActionUseWallMount::GetClassTypeName() const -- calls CTCActionUseWallMount::StatGetClassTypeName @ 0x004D5CD3 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d5cd3(); };
extern "C" T* __stdcall CTCActionUseWallMount_GetClassTypeName(T* result) { result->Init_004d5cd3(); return result; }