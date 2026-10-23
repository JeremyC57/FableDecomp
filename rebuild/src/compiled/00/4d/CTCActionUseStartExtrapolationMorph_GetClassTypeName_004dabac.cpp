#pragma optimize("s",on)
// CTCActionUseStartExtrapolationMorph::GetClassTypeName() const -- calls CTCActionUseStartExtrapolationMorph::StatGetClassTypeName @ 0x004D2C68 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2c68(); };
extern "C" T* __stdcall CTCActionUseStartExtrapolationMorph_GetClassTypeName(T* result) { result->Init_004d2c68(); return result; }