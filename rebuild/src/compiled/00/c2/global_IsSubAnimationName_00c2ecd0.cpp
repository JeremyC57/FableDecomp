// _global::IsSubAnimationName(const CString& name) -- true when the name is longer than five characters and its last five start with "_SUB" (the `_SUBn` suffix of a sub-animation event name)
// Leaf: IsSubAnimationName
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2ECD0, 77 bytes, `ret` with the string in ecx. Descriptive name (no PDB covers
//   this library): the catalog's `Std_IsSpecialFunctionName` is propagated. Callers: the animation-event handlers
//   0x00C2E8D0 / 0x00C2E960, which build a library CString temporary (0x00C0DAB0) and test the bool in al.
// Callee: MSVCR71 strncmp (IAT 0x01440138); "_SUB" @ 0x012B48AC. strlen is the intrinsic (inline scan), evaluated
//   twice as retail does (no CSE across the two calls).
// Shape: `__fastcall` with the string by reference in ecx; `strlen > 5` is unsigned (`jbe`); the literal is strncmp's
//   FIRST argument (pushed last); strncmp is the dllimport (call through the IAT); TRUE/FALSE as 1/0 in eax.
#include <stddef.h>
extern "C" size_t __cdecl strlen(const char* text);
#pragma intrinsic(strlen)
extern "C" __declspec(dllimport) int __cdecl strncmp(const char* a, const char* b, size_t count);   // IAT 0x01440138

class CString {
public:
    char* m_pchData;                            // +0x00
    operator const char*() const { return m_pchData; }
};

int __fastcall IsSubAnimationName(const CString& name)
{
    if (strlen(name) > 5 && strncmp("_SUB", (const char*)name + strlen(name) - 5, 4) == 0)
        return 1;
    return 0;
}