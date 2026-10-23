#pragma optimize("s",on)
// NThingShot::CTCShotDefinitionFireballSpell::GetClassTypeName() const -- calls NThingShot::CTCShotDefinitionFireballSpell::StatGetClassTypeName @ 0x004D553C into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d553c(); };
extern "C" T* __stdcall CTCShotDefinitionFireballSpell_GetClassTypeName(T* result) { result->Init_004d553c(); return result; }