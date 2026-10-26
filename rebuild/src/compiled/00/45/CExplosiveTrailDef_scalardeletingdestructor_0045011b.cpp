#pragma optimize("s",on)
// vector deleting destructor that first re-seats the vptr (the compiler-generated
// destructor prologue for a polymorphic class), then destroys and optionally frees.
// __fastcall this=ecx, flags=stack (ret 4).
extern void* g_vtable[];              // 0x01230BA0
class CExplosiveTrailDef { public: void** vptr; void Dtor(); void* ScalarDeletingDestructor(unsigned flags); };
void operator delete(void*);
void* CExplosiveTrailDef::ScalarDeletingDestructor(unsigned flags) {
    this->vptr = g_vtable;
    this->Dtor();
    if (flags & 1) operator delete(this);
    return this;
}