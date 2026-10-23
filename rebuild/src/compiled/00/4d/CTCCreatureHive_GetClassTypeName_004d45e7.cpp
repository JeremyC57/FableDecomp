#pragma optimize("s",on)
// CTCCreatureHive::GetClassTypeName() const -- calls CTCCreatureHive::StatGetClassTypeName @ 0x004D45B4 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d45b4(); };
extern "C" T* __stdcall CTCCreatureHive_GetClassTypeName(T* result) { result->Init_004d45b4(); return result; }