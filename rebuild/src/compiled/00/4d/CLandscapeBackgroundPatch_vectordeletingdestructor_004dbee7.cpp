#pragma optimize("s",on)
// Leaf: `vector_deleting_destructor'
// vector deleting destructor whose deallocator is the engine free helper (0x00bfe9bc),
// cleaned with `pop ecx` (size peephole). __fastcall this=ecx, flags=stack (ret 4).
struct T { void Dtor_004d799a(); void* VecDel(unsigned flags); };
void operator delete(void*);
void* T::VecDel(unsigned flags) {
    this->Dtor_004d799a();
    if (flags & 1) operator delete(this);
    return this;
}