// CPersistContext_TransferableComponent::TransferOut(CPersistContext*, CCharString*) -- sign-extends the signed char behind the target pointer to a long local and hands its address to the long writer 0x009E2560
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CBE00, 31 bytes.
// Callee: 0x009E2560 TransferOutLong (__fastcall: context in ecx, output string in edx, &value on the stack).
//
// Persist WRITE hook (vtable slot 0) of a signed one-byte transferable component: the component
// holds a pointer to the transferred byte at +0x4. The byte is sign-extended (`movsx ecx, byte ptr`)
// into a long local (the `push ecx` frame slot) whose address goes to the long writer. Same shape as
// the landed 0x007B41A0 family; `signed char` (not plain `char`, which is also signed under the
// era defaults) names the intent.
struct CPersistContext;
struct CCharString;
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560

struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, CCharString* out);
    signed char* m_pValue;                 // +0x4
};

void CPersistContext_TransferableComponent::TransferOut(CPersistContext* context, CCharString* out)
{
    long value = *m_pValue;
    TransferOutLong(context, out, &value);
}
