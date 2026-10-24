// _global::UninitFillN_CPhysicsMeshInfo_00a2aed0(CPhysicsMeshInfo* first, unsigned long count, const CPhysicsMeshInfo& value, CRetailAllocator) -- placement-copy `count` CPhysicsMeshInfo records, return the advanced pointer
// Leaf: UninitFillN_CPhysicsMeshInfo_00a2aed0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2AED0, 67 bytes (+13 int3 pad). No callees.
//
// Retail bytes:  test edx,edx ; mov eax,ecx ; jbe E ; push esi ; mov esi,edx ; mov edx,[value] ;
//   L: test eax,eax ; je S ; mov [eax],vtbl 0x0129CD28 ; copy +4,+8 ; mov [eax],vtbl 0x0129CD50 ;
//   mov ecx,[edx+0xc] ; test ecx,ecx ; mov [eax+0xc],ecx ; je S ; inc [ecx+4] ; S: add eax,0x10 ;
//   dec esi ; jne L ; pop esi ; E: ret 8
// Dinkumware `_Uninit_fill_n` (__fastcall: first in ecx, count in edx, value reference and the
// by-value empty allocator on the stack -> `ret 8`). The manifest label (a {__int64,
// CCountedPointer<CActiveEntityScriptBase>} pair) is propagated over byte-identical shapes and is
// wrong here: the two vtable stores are RTTI CMeshDataBankEntry (0x0129CD28) then CPhysicsMeshInfo
// (0x0129CD50), i.e. the element is a 16-byte CPhysicsMeshInfo (PDB: base CMeshDataBankEntry
// {vptr, Bank +4, Index +8}, MeshData CIVCountedPointer<CPhysicsMeshInfo::CMeshData> +0xc).
// The implicit copy constructor is inlined under the placement-new null test: base vtable, the
// base's two members, own vtable, then the intrusive counted pointer copy (IVRefCount at +4 of the
// pointee, addref when non-null). engine/CMeshDataBankEntry.h is a flattened data-only record
// (void* __vftable), so the polymorphic copy needs this local virtual model (same as the landed
// C3DAnimationInfo copy ctor 0x00A2A4F0, which shares the base).
#include <new>

struct CMeshDataBank;

struct CMeshDataBankEntry {                // vtable 0x0129CD28
    virtual void v00();
    CMeshDataBank* Bank;                   // +4
    unsigned long Index;                   // +8
};

struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);
    unsigned long IVRefCount;              // +4
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

struct CPhysicsMeshInfo : CMeshDataBankEntry {   // vtable 0x0129CD50
    virtual void v00();
    CIVCountedPointer MeshData;            // +0xc
};

struct CRetailAllocator { };

CPhysicsMeshInfo* __fastcall UninitFillN_CPhysicsMeshInfo_00a2aed0(CPhysicsMeshInfo* first, unsigned long count, const CPhysicsMeshInfo& value, CRetailAllocator allocator)
{
    for (; 0 < count; --count, ++first)
        new (first) CPhysicsMeshInfo(value);
    return first;
}