#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionThrow::GetClassTypeName() const -- calls NThingShot::CTCShotDefinitionThrow::StatGetClassTypeName @ 0x004D5447 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init(); };
extern "C" T* __stdcall CTCShotDefinitionThrow_GetClassTypeName(T* result) { result->Init(); return result; }