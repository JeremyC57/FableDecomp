// CRandomDelay_00c2d100::PickDelay(float rate) -- draw a geometrically distributed delay: base + 1 + (long)(log10(1 - rand()/32767) / log10(1 - rate * 10 / period))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D100, length 87 bytes.
// Leaf: PickDelay
// Callees: the CRT `rand` through its import slot (call [0x01440124]), the intrinsic log10 (`fldlg2 ; fxch ; fyl2x`), __ftol2 @ 0x00BFEA70.
// Constants: 10.0f @ 0x01237F0C, 1/32767 (3.0518509e-05f) @ 0x0129C81C, 1.0 (double) @ 0x01237F00.
// The chunk label "CTCLook::UpdateLookTimer" is propagated: the callers (0x00C61CC1 / 0x00C62423, the movement-band code of
// the player creature) pass a sub-object at +0x70 whose first word is a base tick and second a period; the class is named
// by address. Shape: the rate is rescaled IN PLACE (`fstp` back into the argument slot: the parameter is reassigned) by two
// compound statements -- `rate * 10.0f / m_period` in one expression is re-associated to `fdiv` then `fmul`; the
// random draw is converted through a frame slot (`fild`), both subtractions are `1.0 - x` in double (`fsubr qword`), and the
// final `lea eax,[eax+ecx+1]` adds the base word and the constant 1 to the converted quotient.
extern "C" __declspec(dllimport) int __cdecl rand(void);
extern "C" double __cdecl log10(double);
#pragma intrinsic(log10)

struct CRandomDelay_00c2d100 {
    long  m_base;                                  // +0x00
    float m_period;                                // +0x04
    long PickDelay(float rate);
};

long CRandomDelay_00c2d100::PickDelay(float rate)
{
    rate *= 10.0f;
    rate /= m_period;
    return m_base + (long)(log10(1.0 - rand() * 3.0518509e-05f) / log10(1.0 - rate)) + 1;
}