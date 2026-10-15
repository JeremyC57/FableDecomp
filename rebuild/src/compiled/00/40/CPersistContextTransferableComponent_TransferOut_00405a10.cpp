// CPersistContext_TransferableComponent::TransferOut(CPersistContext*, CCharString*) -- widens the byte behind the target pointer to a long local and hands its address to the long writer 0x009E2560
// Leaf: TransferOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00405A10, 31 bytes. Callee: 0x009E2560 (TransferOutLong, __fastcall).
//
// Persist WRITE hook (vtable slot 0) of a one-byte transferable component: the component holds a
// pointer to the transferred byte at +0x4. The byte is zero-extended (`movzx`) into a long local
// (`push ecx` frame slot) whose address goes to the __fastcall long writer @ 0x009E2560 (context in
// ecx, output string in edx, &value on the stack). Same shape as the landed 0x007B41A0 family with an
// unsigned char target. The manifest label (`CPersistContext_TransferableComponent::vfunc_0`) is a
// propagated template name; the element type is read from the `movzx`.
struct CPersistContext;
struct CCharString;
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560

struct CPersistContext_TransferableComponent {
    virtual void TransferOut(CPersistContext* context, CCharString* out);
    unsigned char* m_pValue;               // +0x4
};

void CPersistContext_TransferableComponent::TransferOut(CPersistContext* context, CCharString* out)
{
    long value = *m_pValue;
    TransferOutLong(context, out, &value);
}