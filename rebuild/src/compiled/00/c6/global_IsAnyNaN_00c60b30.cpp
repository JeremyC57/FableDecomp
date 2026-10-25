// IsAnyNaN(const EAXVECTOR& v) -- true when any component of an EAX vector is NaN
// Leaf: ?IsAnyNaN@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C60B30, 76 bytes (`__cdecl`, the vector by reference; callers `test al,al`).
// Callees: MSVCR71 `_isnan` (thunk 0x00BFEC26) per component.
// Identity: the manifest's "Math_IsAnyNaN" is right in substance. Its callers are the EAX source-property setter
//   0x00C60FF0 (position/velocity vectors at +4 / +0x10 of the property block); the EAX 4.0 context's pan setters
//   0x00C63750 / 0x00C639A0 inline the same test. Descriptive name; EAXVECTOR is the EAX SDK's three-float vector.
// Shape: `return a || b || c;` as bool: the true exit is `mov eax,1` and the false exit reuses `_isnan`'s zero eax
//   (an explicit `if (...) return true; return false;` or `?:` gives `mov al,1`).
extern "C" int __cdecl _isnan(double x);

struct EAXVECTOR {
    float x, y, z;
};

bool IsAnyNaN(const EAXVECTOR& v)
{
    return _isnan(v.x) || _isnan(v.y) || _isnan(v.z);
}