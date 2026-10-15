// CPersistContext_TransferableComponent::TransferOut(CPersistContext*, CCharString*) -- sign-extends the short behind the target pointer to a long local and hands its address to the long writer 0x009E2560
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CBD30, 31 bytes (+1 int3 padding).
// Callee: 0x009E2560 TransferOutLong (__fastcall: context in ecx, output string in edx, &value on the stack).
//
// Persist WRITE hook (vtable slot 0) of a two-byte transferable component: the component holds a
// pointer to the transferred short at +0x4. The short is sign-extended (`movsx ecx, word ptr`) into
// a long local (the `push ecx` frame slot) whose address goes to the long writer. Same shape as the
// landed 0x007B41A0 family; the element type comes from the `movsx` width.
struct CPersistContext;
struct CCharString;
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560

struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, CCharString* out);
    short* m_pValue;                       // +0x4
};

void CPersistContext_TransferableComponent::TransferOut(CPersistContext* context, CCharString* out)
{
    long value = *m_pValue;
    TransferOutLong(context, out, &value);
}