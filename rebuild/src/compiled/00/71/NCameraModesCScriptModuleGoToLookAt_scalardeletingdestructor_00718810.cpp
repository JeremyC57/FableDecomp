// Leaf: ?CScriptModuleGoToLookAt_CompositeScalarDeletingDestructor@@
// Naming wave 26 (family c694): the base destructor 0x005ED120 is ??1CScriptModuleBase@NCameraModes@@UAE@XZ (Ego_r.pdb; Fable.exe RTTI base arrays);
//   was the free ?CScriptModuleGoToLookAt_Destruct@@YIXPAX@Z.
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
void* __fastcall CScriptModuleGoToLookAt_CompositeScalarDeletingDestructor(void* lookAt, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(lookAt) + 76))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    static_cast<NCameraModes::CScriptModuleBase*>(lookAt)->NCameraModes::CScriptModuleBase::~CScriptModuleBase();
    if (flags & 1)
        operator delete(lookAt);
    return lookAt;
}