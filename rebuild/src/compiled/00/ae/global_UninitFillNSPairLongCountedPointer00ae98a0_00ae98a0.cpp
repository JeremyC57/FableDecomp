// _global::UninitFillN_SPairLongCountedPointer_00ae98a0(SPairLongCountedPointer* first, unsigned long count, const SPairLongCountedPointer& value, CRetailAllocator) -- placement-copy `count` {long, CCountedPointer} pairs, return the advanced pointer
// Leaf: UninitFillN_SPairLongCountedPointer_00ae98a0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE98A0, 52 bytes (+12 int3 pad). No callees.
//
// Retail bytes:  test edx,edx ; mov eax,ecx ; jbe E ; push esi ; mov esi,edx ; mov edx,[value] ;
//   L: test eax,eax ; je S ; [edx] -> [eax] ; [edx+4] -> [eax+4] ; mov ecx,[edx+8] ; test ecx,ecx ;
//   mov [eax+8],ecx ; je S ; inc [ecx] ; S: add eax,0xc ; dec esi ; jne L ; pop esi ; E: ret 8
// Dinkumware `_Uninit_fill_n` (__fastcall: first in ecx, count in edx, value reference and the
// by-value empty allocator on the stack). The manifest label is propagated; the bytes copy a
// 12-byte pair: a 4-byte key and a CCountedPointer (Data, Info; Info->RefCount bumped when
// present). The only caller is the manifest's Vector_InsertWithCount (0x00AEA440).
#include <new>

struct CCPPointerInfo {
    unsigned long RefCount;                       // +0
};

struct CCountedPointer {
    void* Data;                                   // +0
    CCPPointerInfo* Info;                         // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

struct SPairLongCountedPointer {
    unsigned long first;                          // +0
    CCountedPointer second;                       // +4
};

struct CRetailAllocator { };

SPairLongCountedPointer* __fastcall UninitFillN_SPairLongCountedPointer_00ae98a0(SPairLongCountedPointer* first, unsigned long count, const SPairLongCountedPointer& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) SPairLongCountedPointer(value);
    return first;
}