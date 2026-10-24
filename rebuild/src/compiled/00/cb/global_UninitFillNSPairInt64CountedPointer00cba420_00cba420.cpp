// _global::UninitFillN_SPairInt64CountedPointer_00cba420(SPairInt64CountedPointer* first, unsigned long count, const SPairInt64CountedPointer& value, CRetailAllocator) -- placement-copy `count` {unsigned __int64, CCountedPointer} pairs, return the advanced pointer
// Leaf: UninitFillN_SPairInt64CountedPointer_00cba420
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBA420, 58 bytes (+6 int3 pad). No callees.
//
// Retail bytes:  test edx,edx ; mov eax,ecx ; jbe E ; push esi ; mov esi,edx ; mov edx,[value] ;
//   L: test eax,eax ; je S ; [edx] -> [eax] ; [edx+4] -> [eax+4] ; [edx+8] -> [eax+8] ; mov ecx,[edx+0xc] ; test ecx,ecx ;
//   mov [eax+0xc],ecx ; je S ; inc [ecx] ; S: add eax,0x10 ; dec esi ; jne L ; pop esi ; E: ret 8
// Dinkumware `_Uninit_fill_n` (__fastcall: first in ecx, count in edx, value reference and the
// by-value empty allocator on the stack). This row is the instantiation the manifest label names:
// std::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > (PDB CCountedPointer:
// Data +0, Info +4 -> CCPPointerInfo::RefCount +0), copied memberwise. The bytes copy a
// 16-byte pair: an unsigned __int64 key and a CCountedPointer (Data, Info; Info->RefCount bumped when
// present). The only caller is the manifest's CVectorMap_EntryScriptBase::AllocateGrowth (0x00CBC850).
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

struct SPairInt64CountedPointer {
    unsigned __int64 first;                       // +0
    CCountedPointer second;                       // +8
};

struct CRetailAllocator { };

SPairInt64CountedPointer* __fastcall UninitFillN_SPairInt64CountedPointer_00cba420(SPairInt64CountedPointer* first, unsigned long count, const SPairInt64CountedPointer& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) SPairInt64CountedPointer(value);
    return first;
}