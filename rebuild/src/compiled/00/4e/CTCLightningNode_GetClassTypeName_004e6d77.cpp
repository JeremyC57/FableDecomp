#pragma optimize("s",on)
// CTCLightningNode::GetClassTypeName() const -- calls CTCLightningNode::StatGetClassTypeName @ 0x004D6674 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d6674(); };
extern "C" T* __stdcall CTCLightningNode_GetClassTypeName(T* result) { result->Init_004d6674(); return result; }