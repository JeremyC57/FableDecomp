#pragma optimize("s",on)
// CTCSmashableWindow::GetClassTypeName() const -- calls CTCSmashableWindow::StatGetClassTypeName @ 0x004D3E73 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d3e73(); };
extern "C" T* __stdcall CTCSmashableWindow_GetClassTypeName(T* result) { result->Init_004d3e73(); return result; }