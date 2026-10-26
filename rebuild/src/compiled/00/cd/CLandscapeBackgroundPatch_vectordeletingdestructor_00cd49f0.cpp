// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD49F0, 56 bytes. Catalog name: CLandscapeBackgroundPatch::`vector_deleting_destructor'
// Leaf: `vector_deleting_destructor'
// (propagated label). Body: a scalar deleting destructor (`??_G` shape, flags in the stack slot) of a record whose
// non-trivial members are four CCharStrings: +0x14, +0x10, +0xc and finally +0x0, each destroyed with
// CCharString::~CCharString @ 0x0099EAE0, then the block is freed with operator delete (MSVCR71 thunk 0x00BFE9BC)
// when flags & 1. __thiscall shape modelled as __fastcall with a dead edx.
class CCharString { public:
    void* PStringData;
    ~CCharString();                 // 0x0099EAE0
};
void operator delete(void* p);      // MSVCR71 import thunk 0x00BFE9BC
struct CStringRecord_00cd49f0 {
    CCharString str0;               // +0x00
    void* f4;                       // +0x04
    void* f8;                       // +0x08
    CCharString strC;               // +0x0c
    CCharString str10;              // +0x10
    CCharString str14;              // +0x14
};
void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CStringRecord_00cd49f0* self, int edx_dummy, unsigned int flags)
{
    self->str14.~CCharString();
    self->str10.~CCharString();
    self->strC.~CCharString();
    self->str0.~CCharString();
    if (flags & 1)
        operator delete(self);
    return self;
}
