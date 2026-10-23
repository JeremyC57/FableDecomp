#pragma optimize("s",on)
// CTCFlashOnHit::GetClassTypeName() const -- calls CTCFlashOnHit::StatGetClassTypeName @ 0x004D627F into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d627f(); };
extern "C" T* __stdcall CTCFlashOnHit_GetClassTypeName(T* result) { result->Init_004d627f(); return result; }