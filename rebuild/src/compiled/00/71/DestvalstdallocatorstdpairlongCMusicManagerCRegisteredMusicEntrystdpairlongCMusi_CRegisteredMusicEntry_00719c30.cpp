struct Sub;
// Leaf: CRegisteredMusicEntry>_>
struct Obj;

// call 0xa01c10 : method on subobject at this+0x114 (ecx = &sub)
extern "C" void __fastcall SubHelper_00a01c10(Sub* s);
// jmp 0x62e900 : method on this (ecx = this)
extern "C" void __fastcall FinishHelper(Obj* o);

struct Sub { int pad; };
struct Obj {
    char pad[0x114];
    Sub sub;
};

void __fastcall DestVal_Music_D(Obj* self)
{
    SubHelper_00a01c10(&self->sub);
    FinishHelper(self);
}