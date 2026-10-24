// _global::UninitFillN_SPairLongNamedCountedPointer_00a6fe70(SPairLongNamedCountedPointer* first, unsigned long count, const SPairLongNamedCountedPointer& value, CRetailAllocator) -- placement-copy `count` {long, {CCharString, CCountedPointer}} pairs, return the advanced pointer
// Leaf: UninitFillN_SPairLongNamedCountedPointer_00a6fe70
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6FE70, 87 bytes (+9 int3 pad). Callee: CCharString copy ctor 0x0099EC30.
//
// Retail bytes:  push ecx ; test edx,edx ; push ebx ; mov ebx,ecx ; jbe E ; push ebp ; mov ebp,[value] ;
//   push esi ; push edi ; lea edi,[ebx+8] ; mov [count slot],edx ; L: test ebx,ebx ; je S ;
//   mov eax,[ebp] ; lea esi,[ebp+4] ; push esi ; lea ecx,[edi-4] ; mov [ebx],eax ;
//   call CCharString::CCharString(const CCharString&) ; [esi+4] -> [edi] ; mov esi,[esi+8] ;
//   test esi,esi ; mov [edi+4],esi ; je S ; inc [esi] ; S: ebx += 0x10 ; edi += 0x10 ; --count ;
//   jne L ; ... ; E: mov eax,ebx ; pop ebx ; pop ecx ; ret 8
// Dinkumware `_Uninit_fill_n` (__fastcall: first in ecx, count in edx, value reference and the
// by-value empty allocator on the stack). The manifest label is propagated; the bytes copy a
// 16-byte pair whose first is a 4-byte scalar and whose second is a {CCharString, CCountedPointer}
// pair (the inner copy is addressed through `&value.second` in esi: name at +0, CCountedPointer
// Data +4, Info +8 with its RefCount bumped when present). Register pressure spills the count to
// the `push ecx` slot; the second member's address is strength-reduced into edi.
#include <new>

struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);       // 0x0099EC30
};

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

struct SPairCharStringCountedPointer {
    CCharString first;                            // +0
    CCountedPointer second;                       // +4
};

struct SPairLongNamedCountedPointer {
    unsigned long first;                          // +0
    SPairCharStringCountedPointer second;         // +4
};

struct CRetailAllocator { };

SPairLongNamedCountedPointer* __fastcall UninitFillN_SPairLongNamedCountedPointer_00a6fe70(SPairLongNamedCountedPointer* first, unsigned long count, const SPairLongNamedCountedPointer& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) SPairLongNamedCountedPointer(value);
    return first;
}