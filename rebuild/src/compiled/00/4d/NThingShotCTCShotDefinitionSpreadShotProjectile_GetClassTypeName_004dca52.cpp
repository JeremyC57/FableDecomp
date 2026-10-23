#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionSpreadShotProjectile::GetClassTypeName() const -- calls NThingShot::CTCShotDefinitionSpreadShotProjectile::StatGetClassTypeName @ 0x004D53EE into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d53ee(); };
extern "C" T* __stdcall CTCShotDefinitionSpreadShotProjectile_GetClassTypeName(T* result) { result->Init_004d53ee(); return result; }