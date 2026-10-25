// CAudioSystem::SetError(long code) -- latch the audio library's first error: when the last-error word at 0x0143BB48 is clear, store the code there and tail-call the system's `__stdcall` error callback at +0x7c with it
// Leaf: ?SetError@CAudioSystem@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3D090, length 35 bytes (the 64-byte chunk row also holds the sibling setter @ 0x00C3D0C0, `m_pfnErrorCallback = arg`, after int3 padding; only this body is authored).
// Identity (sweep1, 2026-10-25): every caller passes `ecx = 0x0143BB58` (the libpk audio-system singleton, CAudioSystem in
//   this pass) and a negative error code: -6 in the API forwarder 0x00C35A10 (agent437, "SetError"), -0xE / -0xF in the buffer
//   queue's Enqueue 0x00C44460, a failed HRESULT in CAudioObject::PlaySource 0x00C43F80; agent437 (0x00C33DC0, 0x00C35A10,
//   0x00C35EC0) and agent439 (0x00C43390, 0x00C43D30, 0x00C43E20, 0x00C449C0) model it as CAudioSystem::SetError(long). The
//   word it latches is the same 0x0143BB48 that the landed 0x00C3DA00 / 0x00C3DCD0 / 0x00C3DDE0 / 0x00C3DF10 model as
//   `g_lastError_0143bb48` and set inline with the same "first error wins" test. Descriptive names (no PDB covers libpk).
// Label correction: the landed row was CSingleton_00c3d090::Activate(void*) (a "current instance" slot and an activation
//   callback); the chunk label "CDrawAutoPickups::ClearPrimitivesIfNecessary" is propagated.
// Shape: the global is read once, the argument is stored into it and re-pushed through edx; both early exits share `ret 4`
//   (the sibling call rewrites the argument slot and jumps).
typedef void (__stdcall* TAudioErrorCallback)(long code);

extern long g_lastError_0143bb48;                  // 0x0143BB48

class CAudioSystem {
public:
    unsigned char       _pad[0x7c];
    TAudioErrorCallback m_pfnErrorCallback;        // +0x7c
    void SetError(long code);
};

void CAudioSystem::SetError(long code)
{
    if (g_lastError_0143bb48 == 0) {
        g_lastError_0143bb48 = code;
        if (m_pfnErrorCallback)
            m_pfnErrorCallback(code);
    }
}
