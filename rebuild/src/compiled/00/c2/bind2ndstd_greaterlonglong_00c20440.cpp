// bind2nd_std::greater_long_,long_ @ 00c20440
// Vendor naming V2 (plan phase 1, one address, one name): 0x00C1BBD0 ?Set18@CRecordBuilder_00c16310@@QAEXJ@Z (was ?add@Inner@@QAEXJ@Z)
// __fastcall(void* self, long arg):
//   mov eax,[esp+4]; push esi; mov esi,ecx; push eax;
//   lea ecx,[esi+8]; call 0xc1bbd0;   (helper, ecx=self+8, one stack long arg)
//   or dword[esi],0x20; pop esi; ret 4

struct CRecordBuilder_00c16310 {
    // member => __fastcall: ecx=this, one stack long arg
    void Set18(long arg);
};

struct Obj {
    unsigned int flags;   // +0x00
    char pad[4];          // +0x04
    CRecordBuilder_00c16310 inner;          // +0x08
};

void __fastcall bind2nd_greater(Obj* self, int /*edx*/, long arg)
{
    self->inner.Set18(arg);
    self->flags |= 0x20;
}