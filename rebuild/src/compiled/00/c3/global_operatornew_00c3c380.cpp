extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (MSVCR71)
void* __stdcall operator_new(unsigned int n){ return ((void*)(malloc(n))); }