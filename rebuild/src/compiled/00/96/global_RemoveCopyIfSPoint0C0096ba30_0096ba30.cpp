// _global::RemoveCopyIf_SPoint0C_0096ba30(const SPoint0C*, const SPoint0C*, SPoint0C*, SOutsideHeightBand) -- STLport remove_copy_if: copy the 12-byte points whose z lies within a tolerance of a height, returns the advanced result
// Leaf: RemoveCopyIf_SPoint0C_0096ba30
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096BA30, length 81 bytes. No callees.
// STLport `remove_copy_if(first, last, result, pred)` from a /Gr TU (__fastcall: first in ecx, last in edx, result
// and the 8-byte by-value predicate {height, tolerance} on the stack, `ret 0Ch`):
// `for (; first != last; ++first) if (!pred(*first)) { *result = *first; ++result; }`. The predicate rejects a
// point whose `fabs(p.z - height)` is not below the tolerance (`fld [p+8] ; fsub [height] ; fabs ; fcomp [tol]`,
// `test ah,5 ; jp` = skip unless ordered-less); a kept 12-byte trivially copyable point moves as a dword block
// through ebx. The manifest label (`_Move<vector<AttackParticleAndEndPoints>*>`) is propagated: there is no
// tag argument and the body filters.
#include <math.h>

struct SPoint0C {                                // 12-byte point (x, y, z)
    float x;                                     // +0x00
    float y;                                     // +0x04
    float z;                                     // +0x08
};

struct SOutsideHeightBand {                      // by-value predicate, 8 bytes
    float m_height;                              // +0x00
    float m_tolerance;                           // +0x04
    bool operator()(const SPoint0C& p) const { return !(fabs(p.z - m_height) < m_tolerance); }
};

SPoint0C* __fastcall RemoveCopyIf_SPoint0C_0096ba30(const SPoint0C* first, const SPoint0C* last, SPoint0C* result, SOutsideHeightBand pred)
{
    for (; first != last; ++first)
        if (!pred(*first)) {
            *result = *first;
            ++result;
        }
    return result;
}