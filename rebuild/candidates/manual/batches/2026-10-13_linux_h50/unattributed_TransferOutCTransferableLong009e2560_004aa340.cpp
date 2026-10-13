// CTransferableLong_009e2560::vfunc_0(CPersistContext*, long) -- copies the target dword into a local and hands it to the __fastcall helper 0x009E2560
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA340, oracle length 30 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ecx ; mov eax,[ecx+0x4] ; mov ecx,[eax] ; lea edx,[esp] ; mov [esp],ecx ; mov ecx,[esp+8] ; push edx ; mov edx,[esp+10h] ; call TransferOut ; pop ecx ; ret 8
// Persist write hook of a four-byte transferable component: the dword behind the target
// pointer at +0x4 is copied into a stack local whose address goes to the __fastcall helper
// @ 0x009E2560 (context in ecx, the dword second parameter in edx, &local on the stack).
// The manifest label (`CPersistContext_TransferableComponent::vfunc_0`) is propagated; the
// class is named by the helper. One of 15 bodies of this shape.
struct CPersistContext;
void __fastcall TransferOut_009e2560(CPersistContext* context, long name, long* value);   // 0x009E2560

struct CTransferableLong_009e2560 {
    virtual void v00();
    long* m_pValue;                        // +0x4
    virtual void TransferOut(CPersistContext* context, long name);
};

void CTransferableLong_009e2560::TransferOut(CPersistContext* context, long name)
{
    long value = *m_pValue;
    TransferOut_009e2560(context, name, &value);
}
