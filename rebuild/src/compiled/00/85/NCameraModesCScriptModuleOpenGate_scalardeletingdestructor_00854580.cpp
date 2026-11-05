// Leaf: ?CScriptModuleOpenGate_CompositeScalarDeletingDestructor@@
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Naming wave 26 (family c694): the base destructor 0x00854310 is ??1CScriptModuleSelfTerminateBase@NCameraModes@@UAE@XZ (Ego_r.pdb; Fable.exe RTTI base arrays);
//   was the free ?CScriptModuleOpenGate_Destruct@@YIXPAX@Z.
#pragma optimize("t", on)
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
namespace NCameraModes {
class CScriptModuleBase { public:                 // vtable 0x01255F74 (.?AVCScriptModuleBase@NCameraModes@@)
    virtual ~CScriptModuleBase();                   // 0x005ED120
};
class CScriptModuleSelfTerminateBase : public CScriptModuleBase { public:   // RTTI: the direct base of OpenChest / UseLamp / OpenGate
    virtual ~CScriptModuleSelfTerminateBase();      // 0x00854310
};
}
void* __fastcall CScriptModuleOpenGate_CompositeScalarDeletingDestructor(void* gate, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(gate) + 84))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    static_cast<NCameraModes::CScriptModuleSelfTerminateBase*>(gate)->NCameraModes::CScriptModuleSelfTerminateBase::~CScriptModuleSelfTerminateBase();
    if (flags & 1)
        operator delete(gate);
    return gate;
}