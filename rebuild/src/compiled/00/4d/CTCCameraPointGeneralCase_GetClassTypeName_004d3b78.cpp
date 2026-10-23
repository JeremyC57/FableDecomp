#pragma optimize("s",on)
// CTCCameraPointGeneralCase::GetClassTypeName() const -- calls CTCCameraPointGeneralCase::StatGetClassTypeName @ 0x004D3B2E into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d3b2e(); };
extern "C" T* __stdcall CTCCameraPointGeneralCase_GetClassTypeName(T* result) { result->Init_004d3b2e(); return result; }