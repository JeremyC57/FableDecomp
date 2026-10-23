#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionDragonFireball::GetClassTypeName() const -- calls NThingShot::CTCShotDefinitionDragonFireball::StatGetClassTypeName @ 0x004D5562 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d5562(); };
extern "C" T* __stdcall CTCShotDefinitionDragonFireball_GetClassTypeName(T* result) { result->Init_004d5562(); return result; }