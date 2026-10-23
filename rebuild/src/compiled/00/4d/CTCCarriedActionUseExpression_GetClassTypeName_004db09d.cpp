#pragma optimize("s",on)
// CTCCarriedActionUseExpression::GetClassTypeName() const -- calls CTCCarriedActionUseExpression::StatGetClassTypeName @ 0x004D4B9B into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d4b9b(); };
extern "C" T* __stdcall CTCCarriedActionUseExpression_GetClassTypeName(T* result) { result->Init_004d4b9b(); return result; }