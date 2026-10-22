#pragma optimize("s",on)
// CTCSearchableContainer::GetClassTypeName() const -- calls CTCSearchableContainer::StatGetClassTypeName @ 0x004D38CD into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init(); };
extern "C" T* __stdcall CTCSearchableContainer_GetClassTypeName(T* result) { result->Init(); return result; }