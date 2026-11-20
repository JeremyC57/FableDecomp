// CCountedPointer_005ecf50::CCountedPointer_005ecf50(T*) -- adopt the object: null-checked count block with the per-type destroy function
// Leaf: ??0CCountedPointer_005ecf50@@QAE@PAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005ED270, oracle length 78 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[esp+4] ; test eax,eax ; push esi ; mov esi,ecx ; mov [esi],eax ; je Z ; push 0Ch ; call operator new ; add esp,4 ; test eax,eax ; je N ; mov ecx,[esi] ; mov dword ptr [eax],1 ; mov dword ptr [eax+4],offset Destroy ; mov [eax+8],ecx ; mov [esi+4],eax ; mov eax,esi ; pop esi ; ret 4 ; N: xor eax,eax ; mov [esi+4],eax ; mov eax,esi ; pop esi ; ret 4 ; Z: mov dword ptr [esi+4],0 ; mov eax,esi ; pop esi ; ret 4
// Constructor of the intrusive-count smart pointer {object, count block}: stores the object,
// then for a non-null object allocates the 12-byte block {1, destroy fn 0x005ECF50, object}
// (operator new @ 0x00BFEA1A, null-checked), else leaves the block pointer null. Named like the
// Reset family (unique manifest label, else the destroy function). One of 21 bodies.
// Naming (NF157, 2026-11-20; PRIORITY q11649): the per-type deleter 0x005ECF50 stored in the block was the free stand-in
//   ?Destroy_005ecf50@@YIXPAX@Z; it is now the shared engine/CCountedPointer.h static member, FableWin.pdb 0x01DE64C0
//   ?DeleteData@?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@SIXPAX@Z (the self row 0x005ECF50 explicitly instantiates it; its
//   body `delete static_cast<T*>(data)` needs the complete class: engine/NCameraModes_CScriptModuleBase.h, so this TU's pick-any copy is the
//   owner's). The row's own stand-in names (CCountedPointer_005ecf50, SCountBlock) are unchanged.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CCountedPointer.h"
#include "engine/NCameraModes_CScriptModuleBase.h"

struct SCountBlock {
    long m_references;                     // +0
    void (__fastcall* m_pfnDestroy)(void*);// +4
    void* m_pObject;                       // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(CCountedPointer<NCameraModes::CScriptModuleBase>::DeleteData), m_pObject(object) { }
};

struct CCountedPointer_005ecf50 {
    void* m_pObject;                       // +0
    SCountBlock* m_pCount;                 // +4
    CCountedPointer_005ecf50(void* object);
};

CCountedPointer_005ecf50::CCountedPointer_005ecf50(void* object)
    : m_pObject(object)
{
    if (object)
        m_pCount = new SCountBlock(m_pObject);   // re-read through `this` (retail loads [esi] for the block), not the argument
    else
        m_pCount = 0;
}