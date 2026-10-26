// _global::UninitFillN_SPairCharStringLongCountedPointer_00a49c10(SPairCharStringLongCountedPointer* first, unsigned long count, const SPairCharStringLongCountedPointer& value, CRetailAllocator) -- placement-copy `count` {CCharString, long, CCountedPointer} records, return the advanced pointer
// Leaf: UninitFillN_SPairCharStringLongCountedPointer_00a49c10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A49C10, 66 bytes (+14 int3 pad). Callee: CCharString copy ctor 0x0099EC30.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push ebx ; push edi ; mov edi,[value] ;
//   mov ebx,edx ; nop ; L: test esi,esi ; je S ; push edi ; mov ecx,esi ; call CCharString::CCharString ;
//   [edi+4] -> [esi+4] ; [edi+8] -> [esi+8] ; mov eax,[edi+0xc] ; test eax,eax ; mov [esi+0xc],eax ;
//   je S ; inc [eax] ; S: add esi,0x10 ; dec ebx ; jne L ; pop edi ; pop ebx ; E: mov eax,esi ;
//   pop esi ; ret 8
// Dinkumware `_Uninit_fill_n` (__fastcall: first in ecx, count in edx, value reference and the
// by-value empty allocator on the stack). The manifest label is propagated; the bytes copy a
// 16-byte record: a CCharString (out-of-line copy constructor), a 4-byte scalar and a
// CCountedPointer (Data, Info; Info->RefCount bumped when present). The only caller is the
// manifest's Vector_Reallocate_CCharString_CountedPointer (0x00A4AC60).
#include <new>

class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);       // 0x0099EC30
};

class CCPPointerInfo { public:
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

struct SPairCharStringLongCountedPointer {
    CCharString m_name;                           // +0
    unsigned long m_value;                        // +4
    CCountedPointer m_pointer;                    // +8
};

struct CRetailAllocator { };

SPairCharStringLongCountedPointer* __fastcall UninitFillN_SPairCharStringLongCountedPointer_00a49c10(SPairCharStringLongCountedPointer* first, unsigned long count, const SPairCharStringLongCountedPointer& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) SPairCharStringLongCountedPointer(value);
    return first;
}