// Uninitialise(CMarkedBufferOwner_00c43810* owner) -- free `__stdcall` helper: shut the owner down if its marked buffer is live, else detach its current target (SetTarget(0), tail call) if it has one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C35D50, length 39 bytes.
// Retail bytes:  mov ecx,[esp+4] ; mov eax,[ecx+5Ch] ; test eax,eax ; je B ; call Shutdown ; R: ret 4 ; B: mov eax,[ecx+78h] ;
//                test eax,eax ; je R ; mov dword ptr [esp+4],0 ; jmp SetTarget
// Manifest label "CRenderTarget::Uninitialise" is propagated: the body is a free one-argument `__stdcall` function of
// the 0x00C3xxxx module (`ret 4`, ecx never read on entry). Callees: CMarkedBufferOwner_00c43810::Shutdown @ 0x00C43810
// (landed; its marked buffer sits at +0x54 and the buffer pointer at +0x5C is what is tested here) and the setter
// @ 0x00C43E20 (manifest "CExclusiveAssert_UpdateSelectedWeapon"; a `__fastcall` taking the new target, compared with
// the current one at +0x78, returning bool). Shape: the zero argument of the tail call overwrites this function's own
// argument slot (`mov dword ptr [esp+4],0 ; jmp`); both void exits share one `ret 4`.
struct CTarget_00c43e20;

struct CMarkedBuffer_00c43810 {                    // the landed CMarkedBuffer_00c44550
    void* m_pBlock;                                // +0x00
    void* m_pCursor;                               // +0x04
    void* m_pBuffer;                               // +0x08
};

struct CMarkedBufferOwner_00c43810 {
    unsigned char          _pad[0x54];
    CMarkedBuffer_00c43810 m_buffer;               // +0x54
    unsigned char          _pad60[0x18];
    CTarget_00c43e20*      m_pTarget;              // +0x78
    void Shutdown();                               // 0x00C43810
    bool SetTarget(CTarget_00c43e20* target);      // 0x00C43E20
};

void __stdcall Uninitialise(CMarkedBufferOwner_00c43810* owner)
{
    if (owner->m_buffer.m_pBuffer)
        owner->Shutdown();
    else if (owner->m_pTarget)
        owner->SetTarget(0);
}