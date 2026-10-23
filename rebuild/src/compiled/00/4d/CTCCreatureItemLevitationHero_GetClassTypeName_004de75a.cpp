#pragma optimize("s",on)
// CTCCreatureItemLevitationHero::GetClassTypeName() const -- calls CTCCreatureItemLevitationHero::StatGetClassTypeName @ 0x004D45A1 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d45a1(); };
extern "C" T* __stdcall CTCCreatureItemLevitationHero_GetClassTypeName(T* result) { result->Init_004d45a1(); return result; }