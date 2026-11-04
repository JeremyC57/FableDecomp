// CInputManager_007fd3f0::SetVectors(const C3DVector& a, const C3DVector& b, long c) -- copy two 3-vectors into +0xc and +0x18 and the word into +0x28
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FD3F0, length 64 bytes.
// The chunk label "CInputManager::UpdateChecksum" is propagated; named by its effect. Shape: the member addresses in eax (`lea eax,[ecx+0Ch]`), three dword copies each, `ret 0Ch`.
#include "engine/C3DVector.h"
struct CInputManager_007fd3f0 {
    unsigned char _pad[0xc];
    C3DVector     m_a;                             // +0x0c
    C3DVector     m_b;                             // +0x18
    unsigned char _pad24[4];
    long          m_c;                             // +0x28
    void SetVectors(const C3DVector& a, const C3DVector& pos, long c);
};

void CInputManager_007fd3f0::SetVectors(const C3DVector& a, const C3DVector& pos, long c)
{
    m_a = a;
    m_b = pos;
    m_c = c;
}