// CDrawAutoPickups::OnVisibilityChange(bool visible) -- advance the component's 7-bit state for a visibility change (idle 0 -> 1 once its primitive is selected, 8 -> 16), raise the change callback, then flip the Visible bit (marking the component dirty unless idle) and raise the callback again; returns the primitive selection's error, else 0
// Grade: DIFFER(177v181) -- the SetState(1) compare is byte-narrowed (retail `and ecx,7Fh ; cmp ecx,1` on the dword) and the Visible flip is the bitfield xor-merge (retail branches: `or eax,100h` / `and eax,0FFFFFEFFh`); next: an explicit if/else on the raw flags for Visible and a long-typed state compare
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C43FD0, length 192 bytes (ret 4; called directly from 0x00C35E08).
// Callees: CDrawAutoPickups::SelectAndDrawPrimitive 0x00C43DE0 (long, non-zero = failure; manifest label
// "DrawAutoPickups_SelectAndDrawPrimitive"), the owner's `__stdcall` change callback at +0xDC (this, the low 16 flag bits,
// the user data at +0xE0).
// Layout (local model, no PDB layout for the class): flags dword +0x10 (State bits 0..6, Visible bit 8), dirty bits +0x14,
// the selected primitive +0x50.
// Shape: a two-case switch on the cached state (`test ; je case 0 ; cmp 8 ; jne`); the 7-bit State bitfield is set
// through an inline SetState that skips an unchanged state and raises the callback: in case 8 its "unchanged" test
// reuses the cached state (the redundant `cmp edi,10h`), after SelectAndDrawPrimitive it re-reads the flags; the stores
// fold to `and 0FFFFFF81h / or 1` and `and 0FFFFFF90h / or 10h`. The Visible flip tests the cached state for the dirty bit.
struct CDrawAutoPickups;
typedef void (__stdcall *TChangeCallback)(CDrawAutoPickups* component, unsigned long flags, void* userData);

union UComponentFlags {
    unsigned long Raw;
    struct {
        unsigned long State   : 7;              // bits 0..6
        unsigned long Unused7 : 1;
        unsigned long Visible : 1;              // bit 8
    } Bits;
};

struct CDrawAutoPickups {
    void*           __vftable;
    unsigned char   _pad04[0x10 - 4];
    UComponentFlags Flags;                      // +0x10
    unsigned long   DirtyFlags;                 // +0x14
    unsigned char   _pad18[0x50 - 0x18];
    void*           SelectedPrimitive;          // +0x50
    unsigned char   _pad54[0xdc - 0x54];
    TChangeCallback ChangeCallback;             // +0xdc
    void*           CallbackData;               // +0xe0
    long SelectAndDrawPrimitive();              // 0x00C43DE0
    long OnVisibilityChange(bool visible);
    unsigned long GetState() const { return Flags.Raw & 0x7f; }
    void RaiseChange()
    {
        TChangeCallback callback = ChangeCallback;
        if (callback)
            callback(this, Flags.Raw & 0xffff, CallbackData);
    }
    void SetState(unsigned long newState)
    {
        if (GetState() != newState) {
            Flags.Bits.State = newState;
            RaiseChange();
        }
    }
};

long CDrawAutoPickups::OnVisibilityChange(bool visible)
{
    unsigned long state = GetState();
    switch (state) {
    case 0:
        if (SelectedPrimitive == 0) {
            long error = SelectAndDrawPrimitive();
            if (error)
                return error;
        }
        SetState(1);
        break;
    case 8:
        SetState(16);
        break;
    }
    if (visible != (bool)Flags.Bits.Visible) {
        Flags.Bits.Visible = visible;
        if (state != 0)
            DirtyFlags |= 1;
        RaiseChange();
    }
    return 0;
}
