# Lessons from chunk 125

- Callee register-clobber knowledge (0x00C1BC90): the callee must be defined OUT OF CLASS above the caller (with `#pragma auto_inline(off)`); a body written inside the class definition is compiled lazily and gives VC7.1 no clobber facts (every call reloads `mov ecx,esi`).
- An inlined counted-pointer `operator=` whose stores after the release are the zero register (and whose add-ref vanished) reads BOTH source words into locals before the block compare (0x00DDDCE0); reading the source after the release reloads it and keeps an `inc`.
- A 64-bit key read once into ebx/ebp and copied into the same 8-byte frame slot before each `find` is a by-value `unsigned __int64` local handed to inline helpers taking the key BY VALUE (0x006E9E40/0x006E9EC0); helpers taking the creature by reference re-read the id every time.
- A vector difference that stays on the FPU stack (`fld/fsub` x3 then squares, `fstp st(3)` + two pops after `fsqrt`) is a C3DVector with a user default + memberwise copy constructor built in a named local by an inline `operator-`; the POD form spills two 12-byte frame temporaries (0x00984B40). Guards that all jump to one trailing `xor al,al` are nested ifs.
- A float compared with `cmp dword ptr [m],0BF800000h` is a bit-pattern compare in the source (`*(const long*)&f == 0xBF800000`); `== -1.0f` gives `fucompp`.
- An 8-byte frame mismatch around a by-value action temporary means the temporary class is bigger: the cast-shield-spell temporary is 0x124 bytes (0x11c + its two argument members), not the 0x11c of the other cast-spell actions (0x008C29F0).
- `0.0f / g_long` is not constant-folded by VC7.1 (`fild g ; fdivr [0.0f]`); write it as is.
- STLport map `find` returning an `iterator` with a user constructor gives the hidden-pointer call (`push &key ; push &it`); compare `it.node == header` for `end()`.
- The 0x00C0xxxx "CCharString" labels are MFC 7 CString source verbatim (ReleaseBuffer, TrimLeft/TrimRight(TCHAR), ~CString): the assert is a `__fastcall` hook pointer plus an empty `CDebugBreak()` temporary, the CRT/Win32 calls are `extern "C" __declspec(dllimport)`.
- Mixed `xor al,al` early returns and a `mov eax,1 / xor eax,eax` final return come from a `bool` function whose last statement returns a float comparison.
- A scratch-register rotation diff that survives six source variants (eax/edx swapped for a pushed value and a hidden-result address) is not worth more attempts: park it (0x00865EB0).
