// _global::UninitFillN_SPairCharStringCountedPointer_00cba4e0(SPairCharStringCountedPointer* first, unsigned long count, const SPairCharStringCountedPointer& value, CRetailAllocator) -- placement-copy `count` {CCharString, CCountedPointer} pairs, return the advanced pointer
// Leaf: UninitFillN_SPairCharStringCountedPointer_00cba4e0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBA4E0, 60 bytes (+4 int3 pad). Callee: CCharString copy ctor 0x0099EC30.
//
// Retail bytes:  test edx,edx ; push esi ; mov esi,ecx ; jbe E ; push ebx ; push edi ; mov edi,[value] ;
//   mov ebx,edx ; nop ; L: test esi,esi ; je S ; push edi ; mov ecx,esi ; call CCharString::CCharString ;
//   [edi+4] -> [esi+4] ; mov eax,[edi+8] ; test eax,eax ; mov [esi+8],eax ; je S ; inc [eax] ;
//   S: add esi,0xc ; dec ebx ; jne L ; pop edi ; pop ebx ; E: mov eax,esi ; pop esi ; ret 8
// Dinkumware `_Uninit_fill_n` (__fastcall: first in ecx, count in edx, value reference and the
// by-value empty allocator on the stack). The manifest label is propagated; the bytes copy a
// 12-byte std::pair<CCharString, CCountedPointer<...> >: the name through the out-of-line
// CCharString copy constructor, then the counted pointer (Data, Info; Info->RefCount bumped when
// present). The only caller is the manifest's CScriptThing_VectorReallocate (0x00CBC940).
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

struct SPairCharStringCountedPointer {
    CCharString first;                            // +0
    CCountedPointer second;                       // +4
};

struct CRetailAllocator { };

SPairCharStringCountedPointer* __fastcall UninitFillN_SPairCharStringCountedPointer_00cba4e0(SPairCharStringCountedPointer* first, unsigned long count, const SPairCharStringCountedPointer& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) SPairCharStringCountedPointer(value);
    return first;
}