// Byte-exact reconstruction of CEngine::OnPostDeviceReset @ 0x00b25d00
// Leaf: ?OnPostDeviceReset@CEngine@@EAE_NXZ
// this (ecx) = CEngine*; callee receives (this - 0xc).
// [ecx+0xb1] is a bool flag; if clear, return true immediately.
// else call B::DoReset() (0xb24500) on (this-0xc) and return result != 2.

struct B {
    int DoReset(); // __fastcall member @ 0xb24500, returns int
};

// __fastcall on B: model as __fastcall free fn (this in ecx).
int __fastcall B_DoReset(B* self);

class CEngine { public:
    char pad1[0xad];
    unsigned char flag_b1; // at +0xb1
    private: virtual bool OnPostDeviceReset();
};

bool CEngine::OnPostDeviceReset()
{
    CEngine* self = this;
    if (self->flag_b1) {
        B* base = (B*)((char*)self - 0xc);
        return B_DoReset(base) != 2;
    }
    return true;
}