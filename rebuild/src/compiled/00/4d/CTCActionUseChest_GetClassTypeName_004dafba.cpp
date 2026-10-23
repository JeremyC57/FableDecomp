#pragma optimize("s",on)
// CTCActionUseChest::GetClassTypeName() const -- calls CTCActionUseChest::StatGetClassTypeName @ 0x004D3844 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d3844(); };
extern "C" T* __stdcall CTCActionUseChest_GetClassTypeName(T* result) { result->Init_004d3844(); return result; }