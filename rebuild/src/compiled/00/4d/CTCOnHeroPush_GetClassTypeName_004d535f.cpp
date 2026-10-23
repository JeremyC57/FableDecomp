#pragma optimize("s",on)
// CTCOnHeroPush::GetClassTypeName() const -- calls CTCOnHeroPush::StatGetClassTypeName @ 0x004D532E into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d532e(); };
extern "C" T* __stdcall CTCOnHeroPush_GetClassTypeName(T* result) { result->Init_004d532e(); return result; }