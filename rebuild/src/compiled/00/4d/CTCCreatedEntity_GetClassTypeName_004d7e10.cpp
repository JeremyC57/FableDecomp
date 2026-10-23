#pragma optimize("s",on)
// CTCCreatedEntity::GetClassTypeName() const -- calls CTCCreatedEntity::StatGetClassTypeName @ 0x004D3576 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d3576(); };
extern "C" T* __stdcall CTCCreatedEntity_GetClassTypeName(T* result) { result->Init_004d3576(); return result; }