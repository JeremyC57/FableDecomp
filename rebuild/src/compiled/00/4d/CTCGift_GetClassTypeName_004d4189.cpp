#pragma optimize("s",on)
// CTCGift::GetClassTypeName() const -- calls CTCGift::StatGetClassTypeName @ 0x004D415A into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d415a(); };
extern "C" T* __stdcall CTCGift_GetClassTypeName(T* result) { result->Init_004d415a(); return result; }