// _global::UninitFillN_SPairEPSCTypeCComponentInstance_00af6980(SPairEPSCTypeCComponentInstance* first, unsigned long count, const SPairEPSCTypeCComponentInstance& value, CRetailAllocator) -- placement-copy `count` std::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance>, return the advanced pointer
// Leaf: UninitFillN_SPairEPSCTypeCComponentInstance_00af6980
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF6980, 75 bytes (+5 int3 pad). No callees.
//
// Retail bytes:  test edx,edx ; mov eax,ecx ; jbe E ; push esi ; push edi ; mov edi,[value] ;
//   lea esi,[eax+8] ; mov ecx,edx ; L: test eax,eax ; je S ; [edi] -> [eax] ;
//   mov [esi-4],vtbl 0x0129F478 ; [edi+8] -> [esi] ; mov edx,[edi+0xc] ; test edx,edx ;
//   mov [esi+4],edx ; je N ; inc [edx] ; N: byte [edi+0x10] -> [esi+8] ; [edi+0x14] -> [esi+0xc] ;
//   S: add eax,0x18 ; add esi,0x18 ; dec ecx ; jne L ; pop edi ; pop esi ; E: ret 8
// Dinkumware `_Uninit_fill_n` (__fastcall: first in ecx, count in edx, value reference and the
// by-value empty allocator on the stack). The manifest label is propagated: the single vtable
// store is RTTI NParticleEngine::CComponentInstance (0x0129F478), and the 24-byte element is the
// PDB _Pair_base<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> (first +0,
// second +4). PDB CComponentInstance (0x14): vptr, Component CCountedPointer<CPSCBase> +4
// (Data, Info -> RefCount +0), Enabled bool +0xc, InstanceName ulong +0x10. Its implicit copy
// constructor is inlined under the placement-new null test (vtable, counted-pointer copy with the
// addref, then the bool and the name); `&value.second+4` is strength-reduced into esi.
// Callers: C3DMeshFilePartitionChunk-labelled 0x00AF69D0 (propagated), Vector_InsertWithCount_24Byte,
// Vector_InsertRangeResize.
#include <new>

namespace NParticleEngine {

enum EPSCType { PSC_TYPE_FIRST = 0 };

struct CPSCBase;

struct CCPPointerInfo {
    unsigned long RefCount;                       // +0
};

struct CCountedPointer {
    CPSCBase* Data;                               // +0
    CCPPointerInfo* Info;                         // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

struct CComponentInstance {                       // vtable 0x0129F478
    virtual void v00();
    CCountedPointer Component;                    // +4
    bool Enabled;                                 // +0xc
    unsigned long InstanceName;                   // +0x10
};

}

struct SPairEPSCTypeCComponentInstance {
    NParticleEngine::EPSCType first;              // +0
    NParticleEngine::CComponentInstance second;   // +4
};

struct CRetailAllocator { };

SPairEPSCTypeCComponentInstance* __fastcall UninitFillN_SPairEPSCTypeCComponentInstance_00af6980(SPairEPSCTypeCComponentInstance* first, unsigned long count, const SPairEPSCTypeCComponentInstance& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) SPairEPSCTypeCComponentInstance(value);
    return first;
}