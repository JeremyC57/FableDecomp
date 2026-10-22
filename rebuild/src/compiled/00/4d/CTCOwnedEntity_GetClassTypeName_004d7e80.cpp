#pragma optimize("s",on)
// CTCOwnedEntity::GetClassTypeName() const -- calls CTCOwnedEntity::StatGetClassTypeName @ 0x004D3589 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init(); };
extern "C" T* __stdcall CTCOwnedEntity_GetClassTypeName(T* result) { result->Init(); return result; }