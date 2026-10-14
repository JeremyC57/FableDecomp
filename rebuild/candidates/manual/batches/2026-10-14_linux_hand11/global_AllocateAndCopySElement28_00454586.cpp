#pragma optimize("s", on)
// AllocateAndCopy_SElement28(unsigned long count, SElement28* first, SElement28* last) -- allocates a buffer for `count` 28-byte elements (none for zero) and placement-copies [first, last) into it; returns the buffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00454586, length 68 bytes.
// Callees: malloc @ 0x00BFEA0E.
// Pragma: #pragma optimize("s", on) -- `pop ecx` after malloc and `push 7 ; pop ecx` for the block
// copy count are the size-optimised forms.
// Shape: a __stdcall free function (three stack arguments, `ret 0xc`, no register argument; the
// manifest's "CActiveFile::ReallocateAndCopy" label is propagated). The allocation is the
// `count ? malloc(count * 28) : 0` of a vector grow path; the copy loop is the Dinkumware
// uninitialised copy: each element goes through placement new (hence the null test on the
// destination) as a 7-dword block (a plain POD, so `rep movsd`); the destination is written as the
// byte offset `buffer + ((char*)p - (char*)first)`, which VC folds to the hoisted byte delta `buffer - first` in ebx added to
// the running source pointer (`lea edi,[ebx+edx]`) -- a two-pointer loop keeps two counters. The
// buffer is returned in eax.
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E

struct SElement28 {                                          // 28-byte POD record (block-copied)
    unsigned char _body[0x1c];
};

SElement28* __stdcall AllocateAndCopy_SElement28(unsigned long count, SElement28* first, SElement28* last)
{
    SElement28* buffer = count ? (SElement28*)malloc(count * sizeof(SElement28)) : 0;
    for (SElement28* p = first; p != last; ++p)
        new ((SElement28*)((char*)buffer + ((char*)p - (char*)first))) SElement28(*p);
    return buffer;
}
