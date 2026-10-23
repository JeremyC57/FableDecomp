#pragma optimize("s",on)
// CTCActionUsePutInInventory::GetClassTypeName() const -- calls CTCActionUsePutInInventory::StatGetClassTypeName @ 0x004D2BC6 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2bc6(); };
extern "C" T* __stdcall CTCActionUsePutInInventory_GetClassTypeName(T* result) { result->Init_004d2bc6(); return result; }