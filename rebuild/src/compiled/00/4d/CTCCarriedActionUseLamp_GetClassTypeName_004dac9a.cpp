#pragma optimize("s",on)
// CTCCarriedActionUseLamp::GetClassTypeName() const -- calls CTCCarriedActionUseLamp::StatGetClassTypeName @ 0x004D2CA1 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2ca1(); };
extern "C" T* __stdcall CTCCarriedActionUseLamp_GetClassTypeName(T* result) { result->Init_004d2ca1(); return result; }