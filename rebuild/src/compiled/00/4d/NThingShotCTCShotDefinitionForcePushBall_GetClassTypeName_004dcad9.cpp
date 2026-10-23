#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionForcePushBall::GetClassTypeName() const -- calls NThingShot::CTCShotDefinitionForcePushBall::StatGetClassTypeName @ 0x004D5529 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d5529(); };
extern "C" T* __stdcall CTCShotDefinitionForcePushBall_GetClassTypeName(T* result) { result->Init_004d5529(); return result; }