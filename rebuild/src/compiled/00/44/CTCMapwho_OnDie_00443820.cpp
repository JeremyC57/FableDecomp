// Null-guarded teardown: run the cleanup member, then free the object.
// __fastcall object=ecx, no args.
struct T { void Cleanup_00441c20(); };
void operator delete(void*);   // 0x00bfe9bc
extern "C" void __fastcall OnDie(T* p) {
    if (p) {
        p->Cleanup_00441c20();
        operator delete(p);
    }
}