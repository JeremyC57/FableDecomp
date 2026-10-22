#pragma optimize("s",on)
// CTCGeneratedCreature::GetClassTypeName() const -- calls CTCGeneratedCreature::StatGetClassTypeName @ 0x004D3780 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init(); };
extern "C" T* __stdcall CTCGeneratedCreature_GetClassTypeName(T* result) { result->Init(); return result; }