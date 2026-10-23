#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionTearInGround::GetClassTypeName() const -- calls NThingShot::CTCShotDefinitionTearInGround::StatGetClassTypeName @ 0x004D54ED into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d54ed(); };
extern "C" T* __stdcall CTCShotDefinitionTearInGround_GetClassTypeName(T* result) { result->Init_004d54ed(); return result; }