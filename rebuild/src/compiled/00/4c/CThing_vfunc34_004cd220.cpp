// CThing::vfunc_34(void*) -- CThing vtable slot 34: base implementation of a one-argument boolean query; always false
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD220, 5 bytes. CThing vtable 0x01239ABC slot 34. No callees.
// Shape: `xor al,al ; ret 4` -- a bool result (byte register only) and one ignored 4-byte stack argument.
struct CThing {
    virtual bool vfunc_34(void* arg);
};

bool CThing::vfunc_34(void* arg)
{
    return false;
}