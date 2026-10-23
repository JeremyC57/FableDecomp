#pragma optimize("s",on)
// CTCActionUseMapTable::GetClassTypeName() const -- calls CTCActionUseMapTable::StatGetClassTypeName @ 0x004D2BD9 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d2bd9(); };
extern "C" T* __stdcall CTCActionUseMapTable_GetClassTypeName(T* result) { result->Init_004d2bd9(); return result; }