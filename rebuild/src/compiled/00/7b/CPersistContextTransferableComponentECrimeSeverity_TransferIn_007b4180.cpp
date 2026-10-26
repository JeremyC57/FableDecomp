// CTransferableLong_009e2590::vfunc_1(CPersistContext*, unsigned char) -- reads the byte through the __fastcall helper 0x009E2590, stores it through the pointer at +0x4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B4180, oracle length 32 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov edx,[esp+8] ; push esi ; mov esi,[ecx+0x4] ; mov ecx,[esp+8] ; lea eax,[esp+0Ch] ; push eax ; call TransferIn ; mov ecx,[esp+0Ch] ; mov [esi],ecx ; pop esi ; ret 8
// Persist read hook of a four-byte transferable component: the dword second parameter is passed
// by value AND by address to the __fastcall helper @ 0x009E2590 (context in ecx, value in edx,
// &value on the stack), which writes the parsed value back into the parameter slot; it
// is then stored through the component's target pointer at +0x4 (read before the call). The manifest label
// (`CPersistContext_TransferableComponent::vfunc_1`) is propagated; the class is named by the
// helper. One of 21 bodies of this shape.
class CPersistContext;
void __fastcall TransferIn_009e2590(CPersistContext* context, long value, long* result);   // 0x009E2590 (manifest: ctx, CStringParser*, long*)

struct CTransferableLong_009e2590 {
    virtual void v00();
    long* m_pValue;                  // +0x4
    virtual void TransferIn(CPersistContext* context, long value);
};

void CTransferableLong_009e2590::TransferIn(CPersistContext* context, long value)
{
    long* target = m_pValue;                  // read before the call (retail loads [ecx+4] first): a named local
    TransferIn_009e2590(context, value, &value);
    *target = (long)value;
}