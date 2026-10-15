// CThingFilter_IsValid::func_00caef80() -- return `this`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAEF80, length 3 bytes (16 with padding); no direct reference (the
// address is taken through a table). One of the three identical accessors of the catalogue row 00caef70 (0x00CAEF80,
// 0x00CAEF90, 0x00CAEFA0; skipped ledger, chunk 904), in the DirectShow base-class region: the `GetOwner()`-style
// identity accessors of three objects. The class label is propagated, so each is modelled locally.
// Shape: `mov eax,ecx ; ret`.
struct CSelfAccessor_00caef80 {
    CSelfAccessor_00caef80* func_00caef80();
};

CSelfAccessor_00caef80* CSelfAccessor_00caef80::func_00caef80()
{
    return this;
}
