// Grade: DIFFER(94v47) -- catalog row holds only the first 47 bytes (to `ret 10h`); this source is a RELOCATION_MATCH against the full 94-byte retail symbol (body, inlined filter funclet, handler, cold terminate call and its int3; qx.py). Re-bound the row to 0x5E bytes, then land.
// __ArrayUnwind(void* ptr, size_t size, int count, void (__thiscall* pDtor)(void*)) -- CRT: destroy the built part of an array
// Library: msvcrt.lib(build\intel\dll_obj\ehvecdtr.obj) ?__ArrayUnwind@@YGXPAXIHP6EX0@Z@Z
// Leaf: __ArrayUnwind
// Retail: Fable.exe @ 0x00BFEE7A, 93 bytes including the filter funclet and the cold terminate call (VC7.1 CRT
// ehvecdtr.cpp, size-optimised: `push 14h ; push scopetable ; call __SEH_prolog` 0x0040138C, __SEH_epilog 0x004013C7).
// Walks the array backwards from ptr, calling the element destructor (ecx = element) count times inside __try; the
// __except filter is ArrayUnwindFilter inlined into the funclet: a C++ exception (0xE06D7363) during unwind calls
// terminate (0x00BFEFAA, noreturn, placed last), anything else continues the search.
// Catalog row: only the first 47 bytes (up to `ret 10h`); the compiled symbol also holds the filter, the handler and
// the terminate call (93 bytes).
#pragma optimize("s", on)
struct EHExceptionRecord_ { unsigned long ExceptionCode; };
struct EXCEPTION_POINTERS_ { EHExceptionRecord_* ExceptionRecord; void* ContextRecord; };
__declspec(noreturn) void __cdecl terminate();         // 0x00BFEFAA MSVCR71 ?terminate@@YAXXZ (import thunk)
extern "C" void* __cdecl _exception_info(void);
typedef void (__thiscall *PFNDTOR)(void*);
#define EH_EXCEPTION_NUMBER 0xE06D7363
static int ArrayUnwindFilter(EXCEPTION_POINTERS_* pExPtrs)
{
    EHExceptionRecord_* pExcept = pExPtrs->ExceptionRecord;
    switch (pExcept->ExceptionCode) {
    case EH_EXCEPTION_NUMBER:
        terminate();
    default:
        return 0;           // EXCEPTION_CONTINUE_SEARCH
    }
}
extern "C" void __stdcall __ArrayUnwind(void* ptr, unsigned int size, int count, PFNDTOR pDtor)
{
    __try {
        while (--count >= 0) {
            ptr = (char*)ptr - size;
            (*pDtor)(ptr);
        }
    }
    __except (ArrayUnwindFilter((EXCEPTION_POINTERS_*)_exception_info())) {
    }
}
