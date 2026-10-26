// CThing::vfunc_35(void*) -- CThing vtable slot 35: base implementation of a one-argument boolean query; always false
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD230, 5 bytes. CThing vtable 0x01239ABC slot 35. No callees.
// Shape: `xor al,al ; ret 4` -- a bool result (byte register only) and one ignored 4-byte stack argument
// (byte-identical to slot 34 at 0x004CD220; a separate virtual, not folded).
class CThing { public:
    virtual bool vfunc_35(void* arg);
};

bool CThing::vfunc_35(void* arg)
{
    return false;
}