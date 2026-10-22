#pragma optimize("s",on)
// vector deleting destructor whose deallocator is the engine free helper (0x00bfe9bc),
// cleaned with `pop ecx` (size peephole). __fastcall this=ecx, flags=stack (ret 4).
struct CThingHolySiteDef { void Dtor(); void* ScalarDeletingDestructor(unsigned flags); };
void operator delete(void*);
void* CThingHolySiteDef::ScalarDeletingDestructor(unsigned flags) {
    this->Dtor();
    if (flags & 1) operator delete(this);
    return this;
}