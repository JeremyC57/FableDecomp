// IsNaN(const _D3DVECTOR& v) -- true when any component of a DirectSound 3D vector is NaN
// Leaf: ?IsNaN@@YA_NABU_D3DVECTOR@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C60B30, 76 bytes (`__cdecl`, the vector by reference; callers `test al,al`).
// Callees: MSVCR71 `_isnan` (thunk 0x00BFEC26) per component.
// Identity: the manifest's "Math_IsAnyNaN" is right in substance. Its callers are the EAX source-property setter
//   0x00C60FF0 (position/velocity vectors at +4 / +0x10 of the property block); the EAX 4.0 context's pan setters
//   0x00C63750 / 0x00C639A0 inline the same test.
// Name (owner decision 2026-11-03 D3): the original GameCODA 2.10 EAXBuffer.obj defines bool IsNaN(const _D3DVECTOR&); its 76-byte
//   body is byte-identical to this one after the three _isnan relocations, and its Set3DBuffer passes the DS3D position /
//   velocity to it (Codex pass 12 SDK_COMPARISON 00c60b30). Pass 11 called it IsAnyNaN(const _EAXVECTOR&).
// Shape: `return a || b || c;` as bool: the true exit is `mov eax,1` and the false exit reuses `_isnan`'s zero eax
//   (an explicit `if (...) return true; return false;` or `?:` gives `mov al,1`).
extern "C" int __cdecl _isnan(double x);

#include "engine/DSound3D.h"

bool IsNaN(const D3DVECTOR& v)
{
    return _isnan(v.x) || _isnan(v.y) || _isnan(v.z);
}