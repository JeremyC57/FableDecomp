// CInputEvent::CInputEvent() -- zero the two trailing words and clear the eight-word block at the front
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A04410, length 36 bytes.
// Callees: none (the 32-byte memset is the intrinsic). The manifest label "CDefaultEngineMemoryInit::CDefaultEngineMemoryInit"
// is propagated; the PDB class is 0x28 bytes with a vptr while this body zeroes +0x24/+0x28 and +0..+0x1c with no vtable
// store, so the record is named by address. Shape: the two scalar initialisers are stored first (+0x28, +0x24), then
// the intrinsic memset of the block runs through a second copy of `this` (`mov edx,eax`) with the same zero register;
// +0x20 is left uninitialised (an inline member constructor would zero the block through eax with no edx copy).
#include <string.h>

struct CDefaultEngineMemoryInit_00a04410 {
    long m_block[8];                   // +0x00 .. +0x1c
    long m_unset;                      // +0x20 (not initialised)
    long m_a;                          // +0x24
    long m_b;                          // +0x28
    CDefaultEngineMemoryInit_00a04410();
};

CDefaultEngineMemoryInit_00a04410::CDefaultEngineMemoryInit_00a04410()
{
    m_b = 0;
    m_a = 0;
    memset(m_block, 0, sizeof(m_block));
}