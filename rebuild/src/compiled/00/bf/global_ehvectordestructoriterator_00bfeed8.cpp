// Grade: DIFFER(96v72) -- oracle short, not a code difference: RELOCATION_MATCH against the full 96 retail bytes 0x00BFEED8..0x00BFEF37 (body + the __finally block VC7.1 emits inside the same symbol, checked with qx.py); the catalog row stops at the body`s `ret 10h` (72). Re-cut to 96 to land; test in probe/00bfeed8_test.cpp (PASS).
// __ehvec_dtor(void* ptr, size_t size, int count, void (*pDtor)(void*)) -- `eh vector destructor iterator'
// Retail 0x00BFEED8, 72 bytes (static CRT, ehvecdtr.cpp). Destroys `count` array elements of `size` bytes
// last-to-first through the element destructor; if a destructor throws, the __finally block (the local
// termination handler right after the body, 0x00BFEF20) hands the remaining elements to
// __ArrayUnwind (0x00BFEE7A) before the exception propagates.
// Shape: the CRT's own C++ source compiled for size: an SEH __try/__finally in a size-optimised function
// gets the compiler's shared frame helpers -- `push 0Ch ; push <scope table> ; call __SEH_prolog
// (0x0040138C)` and `call __SEH_epilog (0x004013C7)` -- with `success` at [ebp-1Ch], the try level at
// [ebp-4], `ptr`/`count` updated in their argument slots and the normal-exit `call` into the __finally.
// The element destructor is a __thiscall member in the CRT declaration; it takes `this` only, so the
// one-argument __fastcall pointer below is call-compatible (ecx = element) and spells the same call.
// Leaf: __ehvec_dtor

#pragma optimize("s", on)

typedef unsigned int size_t;

extern "C" void __stdcall __ArrayUnwind(void* ptr, size_t size, int count, void (__fastcall *pDtor)(void*));   // 0x00BFEE7A

extern "C" void __stdcall __ehvec_dtor(void* ptr, size_t size, int count, void (__fastcall *pDtor)(void*))
{
    int success = 0;

    ptr = (char*)ptr + size * count;

    __try
    {
        while (--count >= 0)
        {
            ptr = (char*)ptr - size;
            (*pDtor)(ptr);
        }
        success = 1;
    }
    __finally
    {
        if (!success)
            __ArrayUnwind(ptr, size, count, pDtor);
    }
}
