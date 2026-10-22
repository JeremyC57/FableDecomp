#pragma optimize("s",on)
// vector deleting destructor whose deallocator is the engine free helper (0x00bfe9bc),
// cleaned with `pop ecx` (size peephole). __fastcall this=ecx, flags=stack (ret 4).
struct CNewFrontendGameComponent { void Dtor(); void* ScalarDeletingDestructor(unsigned flags); };
void operator delete(void*);
void* CNewFrontendGameComponent::ScalarDeletingDestructor(unsigned flags) {
    this->Dtor();
    if (flags & 1) operator delete(this);
    return this;
}