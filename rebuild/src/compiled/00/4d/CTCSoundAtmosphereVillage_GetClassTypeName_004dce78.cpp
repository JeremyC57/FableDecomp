#pragma optimize("s",on)
// CTCSoundAtmosphereVillage::GetClassTypeName() const -- calls CTCSoundAtmosphereVillage::StatGetClassTypeName @ 0x004D6122 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d6122(); };
extern "C" T* __stdcall CTCSoundAtmosphereVillage_GetClassTypeName(T* result) { result->Init_004d6122(); return result; }