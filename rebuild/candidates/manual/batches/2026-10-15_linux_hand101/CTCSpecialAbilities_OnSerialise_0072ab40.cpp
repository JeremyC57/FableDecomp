// CTCSpecialAbilities::OnSerialise(CPersistContext* context, long unused) -- persist the class-static current creature global def number (zero default)
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072AB40, length 36 bytes (`ret 8`: two arguments, the second unread);
// vtable 0x0126683C (CTCSpecialAbilities) slot 1, the CTCBase persist hook.
// Callees: CPersistContext::Transfer(const char*, long&, const long&) @ 0x00410BE0 (as the landed CTCGatherPointBase
// slot-1 hook 0x007FFE40) with the key "CurrentCreatureGlobalDefNum" @ 0x0126681C.
// The value is not a member: 0x0138306C is a static long in .data (initial value -1), so `this` is never read.
// Shape: the zero default lives in the `push ecx` frame slot; the context is loaded straight into ecx.
struct CPersistContext {
    void Transfer(const char* name, long& value, const long& defaultValue);   // 0x00410BE0
};

struct CTCSpecialAbilities {
    static long CurrentCreatureGlobalDefNum;                // 0x0138306C
    virtual void v00();
    virtual void OnSerialise(CPersistContext* context, long unused);
};

void CTCSpecialAbilities::OnSerialise(CPersistContext* context, long)
{
    long defaultValue = 0;
    context->Transfer("CurrentCreatureGlobalDefNum", CurrentCreatureGlobalDefNum, defaultValue);
}
