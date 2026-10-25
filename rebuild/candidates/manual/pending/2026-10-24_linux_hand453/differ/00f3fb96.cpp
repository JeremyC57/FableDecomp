// _global::__ehvec_ctor(void* ptr, unsigned int size, int count, ctor pCtor, dtor pDtor) -- CRT `eh vector constructor iterator': construct count elements of size bytes at ptr; if a constructor throws, the __finally unwinds the ones already built
// Grade: DIFFER(98v74) -- row cut: the 74 catalogued bytes match retail exactly (relocations masked) and so does the 24-byte __finally funclet VC7.1 emits right after `ret 14h` (retail 0x00F3FBE0..0x00F3FBF7, the same COMDAT); land against the 98-byte span
// Leaf: ??_L@YGXPAXIHP8_VecElem@@AEXXZ1@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3FB96, 74 bytes, __stdcall `ret 14h`; its __finally funclet follows at
//   0x00F3FBE0 (outside the catalog row) and calls __ArrayUnwind @ 0x00BFEE7A (ptr, size, i, pDtor).
// Identity: the VC7.1 CRT ehvecctr.cpp body (`??_L@YGXPAXIHP6EX0@Z1@Z`), statically linked: SEH frame through
//   __SEH_prolog @ 0x0040138C / __SEH_epilog @ 0x004013C7 (scope table 0x01301CF8), `success` at [ebp-20h], `i` at
//   [ebp-1Ch], the ctor called with the element in ecx (the CRT's __thiscall function pointer).
// Model: VC7.1 user code cannot spell a __thiscall function pointer, so the ctor/dtor are pointers to members of an
//   empty single-inheritance class (4 bytes, called with `mov ecx,ptr ; call [p]`); VC7.1 itself maps the name
//   __ehvec_ctor to `??_L` (`eh vector constructor iterator'), only the pointer types in the mangling differ (P8 for P6E). Size-optimised like the CRT (`/O1`):
//   `#pragma optimize("s", on)` selects the __SEH_prolog helper and the unrotated loop.
#pragma optimize("s", on)

struct _VecElem { };
typedef void (_VecElem::*_PVFV_ctor)();

void __stdcall __ArrayUnwind(void* ptr, unsigned int size, int count, _PVFV_ctor pDtor);   // 0x00BFEE7A

void __stdcall __ehvec_ctor(void* ptr, unsigned int size, int count, _PVFV_ctor pCtor, _PVFV_ctor pDtor)
{
    int i;
    int success = 0;
    __try
    {
        for (i = 0; i < count; i++)
        {
            (((_VecElem*)ptr)->*pCtor)();
            ptr = (char*)ptr + size;
        }
        success = 1;
    }
    __finally
    {
        if (!success)
            __ArrayUnwind(ptr, size, i, pDtor);
    }
}
