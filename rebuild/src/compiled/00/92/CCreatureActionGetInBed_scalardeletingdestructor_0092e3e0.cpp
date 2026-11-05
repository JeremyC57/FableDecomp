// CCreatureAction_GetInBed::`scalar deleting destructor'(unsigned int flags) -- destroy the two CCharStrings at +0xb4 / +0xb0 (0x0099EAE0) and the intelligent pointer at +0xa8 (0x00A01C10), then the CPhysicsMeshInfo base destructor (0x00693EF0) and free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092E3E0, length 65 bytes.
// Leaf: ??_GCNamedPhysicsMeshInfo_0092e3e0@@QAEPAXI@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// The chunk label is propagated; named by address. Shape: no vtable store (the derived vtable folds into the base's; the base destructor is out of line), `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CPhysicsMeshInfo { public:
    virtual void v00();
    unsigned char _body[0xa4];
    ~CPhysicsMeshInfo();                           // 0x00693EF0
};
struct __declspec(novtable) CNamedPhysicsMeshInfo_0092e3e0 : CPhysicsMeshInfo {
    CBaseIntelligentPointer m_pointera8;           // +0xa8
    CCharString             m_b0;                  // +0xb0
    CCharString             m_b4;                  // +0xb4
    ~CNamedPhysicsMeshInfo_0092e3e0() { }
};

void Kill_0092e3e0(CNamedPhysicsMeshInfo_0092e3e0* object)                      // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}