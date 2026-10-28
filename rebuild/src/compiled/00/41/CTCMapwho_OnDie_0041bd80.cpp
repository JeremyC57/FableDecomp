// Null-guarded teardown: run the cleanup member, then free the object.
// Leaf: OnDie
// __fastcall object=ecx, no args.
struct CTexture { void Uninitialise(); };   // 0x009f9f70 ?Uninitialise@CTexture@@QAEXXZ
void operator delete(void*);   // 0x00bfe9bc
extern "C" void __fastcall OnDie(CTexture* p) {
    if (p) {
        p->Uninitialise();
        operator delete(p);
    }
}