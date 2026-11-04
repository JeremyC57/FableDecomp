// Reconstruction of retail 0x00c20560
// Vendor naming V2 (plan phase 1, one address, one name): 0x00C1BC00 ?Set20@CRecordBuilder_00c16310@@QAEXJ@Z (was ?Store@Inner@@QAEXJ@Z)
// __fastcall-style: this in ECX, one long arg on stack, ret 4.
// call helper(&this->second, a) [helper is this-in-ecx, arg on stack]; this->flags |= 0x40;

struct CRecordBuilder_00c16310 {
    void Set20(long a); // __fastcall -> this in ECX, a on stack (target 0x00c1bc00)
};

struct Obj {
    unsigned long flags;   // +0x00
    long          pad;     // +0x04
    CRecordBuilder_00c16310         second;  // +0x08
};

// this in ECX; second arg on stack (dummy EDX to force stack placement) -> ret 4
void __fastcall Recon(Obj* self, int /*edx*/, long a)
{
    self->second.Set20(a);
    self->flags |= 0x40;
}