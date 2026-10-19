// CDrawAutoPickups::OnVisibilityChange(bool visible) -- advance the object's 7-bit state (0 -> 1 unless the pending-primitive check reports a result, 8 -> 16), then apply the visible flag (bit 8), marking it dirty and notifying the state callback on every change
// Grade: DIFFER(181v181) -- the only difference is the re-read state test of SetState(1): retail `and ecx,7Fh ; cmp ecx,1`, the lane narrows it to `and cl,7Fh ; cmp cl,1` (long cast, local copy and a 7-bit bitfield all narrow; lane gap like the 24b `& 0xFFFFFF01` narrowing)
// Leaf: OnVisibilityChange
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C43FD0, length 179 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label (and "DrawAutoPickups_SelectAndDrawPrimitive" on the callee 0x00C43DE0) come from a
// propagation pass; the body sits in the 0x00C4xxxx library module, so the class name is unverified.
// Callees: the check @ 0x00C43DE0 (thiscall, no arguments, nonzero result returned as is), the state callback stored at
// +0xdc (`__stdcall`, called with this, the low 16 flag bits and the user word at +0xe0).
// Layout (descriptive): flags +0x10 (state = bits 0..6, visible = bit 8), dirty bits +0x14, pending primitive +0x50,
// callback +0xdc, callback user data +0xe0.
// Shape (best of 4): the non-zero state arm first (`if (state != 0) { if (state == 8) ... } else ...`); the state change is an inline SetState(n) (`if (state != n) { flags = flags & ~0x7f | n; Notify(); }`, VC
// folds the mask with the new bits) and Notify() is an inline null-tested callback; the state read before the check
// stays in edi for the later dirty test.
struct CStateObject;
typedef long (__stdcall* TStateCallback)(CStateObject* object, unsigned long flags, void* user);

struct CStateObject {
    unsigned char _pad0[0x10];
    unsigned long Flags;                           // +0x10
    unsigned long DirtyFlags;                      // +0x14
    unsigned char _pad1[0x38];
    void* PendingPrimitive;                        // +0x50
    unsigned char _pad2[0x88];
    TStateCallback Callback;                       // +0xdc
    void* CallbackUser;                            // +0xe0
    long CheckPrimitive();                         // 0x00C43DE0
    void Notify()
    {
        if (Callback)
            Callback(this, Flags & 0xffff, CallbackUser);
    }
    void SetState(long state)
    {
        if ((long)(Flags & 0x7f) != state) {
            Flags = (Flags & ~0x7fUL) | state;
            Notify();
        }
    }
};

struct CDrawAutoPickups : CStateObject {
    long OnVisibilityChange(bool visible);
};

long CDrawAutoPickups::OnVisibilityChange(bool visible)
{
    unsigned long state = Flags & 0x7f;
    if (state != 0) {
        if (state == 8)
            SetState(0x10);
    } else {
        if (PendingPrimitive == 0) {
            long result = CheckPrimitive();
            if (result)
                return result;
        }
        SetState(1);
    }
    if (visible != (unsigned char)((unsigned char)(Flags >> 8) & 1)) {
        if (visible)
            Flags |= 0x100;
        else
            Flags &= ~0x100UL;
        if (state)
            DirtyFlags |= 1;
        Notify();
    }
    return 0;
}
