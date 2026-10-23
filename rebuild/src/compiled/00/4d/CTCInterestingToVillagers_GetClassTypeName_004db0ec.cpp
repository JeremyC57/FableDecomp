#pragma optimize("s",on)
// CTCInterestingToVillagers::GetClassTypeName() const -- calls CTCInterestingToVillagers::StatGetClassTypeName @ 0x004D4FEA into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d4fea(); };
extern "C" T* __stdcall CTCInterestingToVillagers_GetClassTypeName(T* result) { result->Init_004d4fea(); return result; }