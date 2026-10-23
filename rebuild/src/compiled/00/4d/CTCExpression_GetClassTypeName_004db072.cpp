#pragma optimize("s",on)
// CTCExpression::GetClassTypeName() const -- calls CTCExpression::StatGetClassTypeName @ 0x004D4B72 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d4b72(); };
extern "C" T* __stdcall CTCExpression_GetClassTypeName(T* result) { result->Init_004d4b72(); return result; }