#pragma optimize("s",on)
// CTCInventoryItem::GetClassTypeName() const -- calls CTCInventoryItem::StatGetClassTypeName @ 0x004D30DB into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d30db(); };
extern "C" T* __stdcall CTCInventoryItem_GetClassTypeName(T* result) { result->Init_004d30db(); return result; }