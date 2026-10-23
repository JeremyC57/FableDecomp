#pragma optimize("s",on)
struct Pool { void* Alloc_0043e4d0(unsigned s); };
void* __stdcall operator_new(unsigned size, Pool* pool){ return pool->Alloc_0043e4d0(size); }