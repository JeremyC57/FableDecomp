// CSoundReceiverLH::vfunc_2(const C3DVector& position) -- forward vtable slot 2 to slot 1 (0x00AAD060, the position setter)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AACF90, length 5 bytes (16 with padding; slot 4 follows at 0x00AACFA0).
// Vtable 0x0129E76C (CSoundReceiverLH, rtti-vtables.tsv) slot 2; the catalogue row 00aacf90 held this body and
// slot 4 (skipped ledger, chunk 901). Slot 1 is the landed CSoundReceiverLH::vfunc_1(const C3DVector&) (stores the
// listener position at +0x04 and commits it), so the same reference argument is passed straight through.
// Shape: `mov eax,[ecx] ; jmp [eax+4]`.
class C3DVector { public:
    float X;                                // +0x00
    float Y;                                // +0x04
    float Z;                                // +0x08
};

struct CSoundReceiverLH {                   // vtable 0x0129E76C
    virtual void v00();
    virtual void vfunc_1(const C3DVector& position);   // 0x00AAD060
    virtual void vfunc_2(const C3DVector& position);
    C3DVector m_position;                   // +0x04
    C3DVector m_forward;                    // +0x10
    C3DVector m_up;                         // +0x1c
    void*     m_pOwner;                     // +0x28
};

void CSoundReceiverLH::vfunc_2(const C3DVector& position)
{
    vfunc_1(position);
}