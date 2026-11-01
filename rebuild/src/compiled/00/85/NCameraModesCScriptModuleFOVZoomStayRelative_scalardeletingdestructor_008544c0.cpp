// Leaf: ?CScriptModuleFOVZoomStayRelative_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
// Naming wave 27 (family c695): the base destructor 0x00854380 is ??1CScriptModuleFOVZoom@NCameraModes@@UAE@XZ (Ego_r.pdb
//   name set; RTTI StayRelative -> FOVZoom -> CScriptModuleBase; chunk 695), called qualified. Was a free
//   ?CScriptModuleFOVZoomStayRelative_Destruct@@YIXPAX@Z.
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
class CScriptModuleBase { public:                     // vtable 0x01255F74 (.?AVCScriptModuleBase@NCameraModes@@)
    virtual ~CScriptModuleBase();                     // 0x005ED120
};
class CScriptModuleFOVZoom : public CScriptModuleBase { public:   // RTTI: the direct base of CScriptModuleFOVZoomStayRelative
    virtual ~CScriptModuleFOVZoom();                  // 0x00854380 (chunk 695)
};
}
void* __fastcall CScriptModuleFOVZoomStayRelative_CompositeScalarDeletingDestructor(void* relative, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(relative) + 132))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    static_cast<NCameraModes::CScriptModuleFOVZoom*>(relative)->NCameraModes::CScriptModuleFOVZoom::~CScriptModuleFOVZoom();
    if (flags & 1)
        operator delete(relative);
    return relative;
}