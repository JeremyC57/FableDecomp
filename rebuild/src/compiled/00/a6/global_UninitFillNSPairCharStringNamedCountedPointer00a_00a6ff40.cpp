// _global::UninitFillN_SPairCharStringNamedCountedPointer_00a6ff40(SPairCharStringNamedCountedPointer* first, unsigned long count, const SPairCharStringNamedCountedPointer& value, CRetailAllocator) -- placement-copy `count` {CCharString, {CCharString, CCountedPointer}} pairs, return the advanced pointer
// Leaf: UninitFillN_SPairCharStringNamedCountedPointer_00a6ff40
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6FF40, 90 bytes. Callee: CCharString copy ctor 0x0099EC30 (twice).
//
// Retail bytes:  push ecx ; test edx,edx ; push ebx ; mov ebx,ecx ; jbe E ; push ebp ; mov ebp,[value] ;
//   push esi ; push edi ; lea edi,[ebx+8] ; mov [count slot],edx ; L: test ebx,ebx ; je S ;
//   push ebp ; mov ecx,ebx ; call CCharString::CCharString ; lea esi,[ebp+4] ; push esi ;
//   lea ecx,[edi-4] ; call CCharString::CCharString ; [esi+4] -> [edi] ; mov esi,[esi+8] ;
//   test esi,esi ; mov [edi+4],esi ; je S ; inc [esi] ; S: ebx += 0x10 ; edi += 0x10 ; --count ;
//   jne L ; ... ; E: mov eax,ebx ; pop ebx ; pop ecx ; ret 8
// Dinkumware `_Uninit_fill_n` (__fastcall: first in ecx, count in edx, value reference and the
// by-value empty allocator on the stack). The manifest label is propagated; the bytes copy a
// 16-byte pair whose first is a CCharString and whose second is a {CCharString, CCountedPointer}
// pair (the inner copy addressed through `&value.second` in esi). Same frame as 0x00A32FB0 /
// 0x00A6FE70 (the {long, {CCharString, CCountedPointer}} twins), with the key copy now a call.
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

struct SPairCharStringNamedCountedPointer {
    CCharString first;                            // +0
    SPairCharStringCountedPointer second;         // +4
};

struct CRetailAllocator { };

SPairCharStringNamedCountedPointer* __fastcall UninitFillN_SPairCharStringNamedCountedPointer_00a6ff40(SPairCharStringNamedCountedPointer* first, unsigned long count, const SPairCharStringNamedCountedPointer& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) SPairCharStringNamedCountedPointer(value);
    return first;
}