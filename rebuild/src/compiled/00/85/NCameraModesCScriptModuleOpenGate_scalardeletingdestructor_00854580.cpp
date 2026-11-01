// Leaf: ?CScriptModuleOpenGate_CompositeScalarDeletingDestructor@@
// Naming wave 26 (family c694): the base destructor 0x00854310 is ??1CScriptModuleSelfTerminateBase@NCameraModes@@UAE@XZ (Ego_r.pdb; Fable.exe RTTI base arrays);
//   was the free ?CScriptModuleOpenGate_Destruct@@YIXPAX@Z.
#pragma optimize("t", on)
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
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