// Null-guarded teardown: run the cleanup member, then free the object.
// Leaf: OnDie
// __fastcall object=ecx, no args.
struct T { void Cleanup_008410d0(); };
void operator delete(void*);   // 0x00bfe9bc
extern "C" void __fastcall OnDie(T* p) {
    if (p) {
        p->Cleanup_008410d0();
        operator delete(p);
    }
}