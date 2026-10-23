#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionProjectilePredictive::GetClassTypeName() const -- calls NThingShot::CTCShotDefinitionProjectilePredictive::StatGetClassTypeName @ 0x004D54DA into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d54da(); };
extern "C" T* __stdcall CTCShotDefinitionProjectilePredictive_GetClassTypeName(T* result) { result->Init_004d54da(); return result; }