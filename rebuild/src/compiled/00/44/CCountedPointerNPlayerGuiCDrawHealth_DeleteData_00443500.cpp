// Null-guarded teardown: run the cleanup member, then free the object.
// Leaf: OnDie
// __fastcall object=ecx, no args.
struct T { void Cleanup(); };
void operator delete(void*);   // 0x00bfe9bc
extern "C" void __fastcall OnDie(T* p) {
    if (p) {
        p->Cleanup();
        operator delete(p);
    }
}