#pragma optimize("s",on)
// CTCMoneyReceiver::GetClassTypeName() const -- calls CTCMoneyReceiver::StatGetClassTypeName @ 0x004D49CA into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d49ca(); };
extern "C" T* __stdcall CTCMoneyReceiver_GetClassTypeName(T* result) { result->Init_004d49ca(); return result; }