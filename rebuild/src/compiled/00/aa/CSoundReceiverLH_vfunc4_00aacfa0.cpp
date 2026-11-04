// CSoundReceiverLH::vfunc_4() -- return a reference to the listener position at +0x04
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AACFA0, length 4 bytes (16 with padding).
// Vtable 0x0129E76C (CSoundReceiverLH, rtti-vtables.tsv) slot 4; the catalogue row 00aacf90 held slot 2 and this
// body (skipped ledger, chunk 901). Layout as the landed 0x00AAD060: position +0x04, forward +0x10, up +0x1c.
// Shape: `lea eax,[ecx+4] ; ret`.
#include "engine/C3DVector.h"

struct CSoundReceiverLH {                   // vtable 0x0129E76C
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual const C3DVector& vfunc_4();
    C3DVector m_position;                   // +0x04
    C3DVector m_forward;                    // +0x10
    C3DVector m_up;                         // +0x1c
    void*     m_pOwner;                     // +0x28
};

const C3DVector& CSoundReceiverLH::vfunc_4()
{
    return m_position;
}