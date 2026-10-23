#pragma optimize("s",on)
// CTCWeatherVane::GetClassTypeName() const -- calls CTCWeatherVane::StatGetClassTypeName @ 0x004D3498 into the hidden CCharString return slot and returns it; stand-in: a construct-and-return helper that runs a member function on the argument and
// return the argument. __stdcall (ret 4); the callee takes this in ecx.
struct T { void Init_004d3498(); };
extern "C" T* __stdcall CTCWeatherVane_GetClassTypeName(T* result) { result->Init_004d3498(); return result; }