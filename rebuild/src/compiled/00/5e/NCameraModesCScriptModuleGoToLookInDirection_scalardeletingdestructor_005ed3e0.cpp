// NCameraModes::CScriptModuleGoToLookInDirection::`scalar deleting destructor'(unsigned int flags) -- run ~CScriptModuleBase inline (clear the owner and camera-manager pointers, destroy the two intelligent pointers) and free the module when bit 0 of the flags is set
// Leaf: ??_GCScriptModuleGoToLookInDirection@NCameraModes@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005ed3e0, 55 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): vtable 0x01255F9C slot 0 (RTTI .?AVCScriptModuleGoToLookInDirection@NCameraModes@@, rebuild/manifest/rtti-vtables.tsv); the body stores
//   the CScriptModuleBase vtable 0x01255F74 (the derived implicit destructors inline the base one without their own store).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (PThingOrigin +0x0C, then PCamPoint +0x04), operator
//   delete 0x00BFE9BC.
// Shape (family L019): Ego_r.pdb CScriptModuleBase members (bases from Fable.exe RTTI: every module derives directly from
//   CScriptModuleBase); `new` emits the vtable and the ??_G, the explicit `p->X::~X()` keeps VC7.1 from calling an
//   out-of-line ??1 from the ??_G (GOTCHAS, family pass 2).
class CBaseObjectPointer;
class CBaseIntelligentPointer {                            // Ego_r.pdb: vptr + PData, 8 bytes
public:
    CBaseIntelligentPointer();                            // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                   // 0x00A01C10
    CBaseObjectPointer* PData;                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CThing;
class C3DVector {
public:
    float X, Y, Z;
};
class CRightHandedSet {                                    // Ego_r.pdb, 24 bytes
public:
    C3DVector Up;                                         // +0x00
    C3DVector Forward;                                    // +0x0C
};
class CBase;
class CGameCameraManager;
namespace NCameraModes {
class CScriptModuleBase {                                  // Ego_r.pdb, 76 bytes (vptr +0x00)
public:
    CIntelligentPointer<CThing> PCamPoint;                // +0x04
    CIntelligentPointer<const CThing> PThingOrigin;       // +0x0C
    C3DVector CoordOrigin;                                // +0x14
    CRightHandedSet CoordAxis;                            // +0x20
    CBase* POwner;                                        // +0x38
    CGameCameraManager* PCameraManager;                   // +0x3C
    bool SelfTerminate;                                   // +0x40
    bool ShortenOnCollision;                              // +0x41
    long CutOutOverride;                                  // +0x44
    float FOVOverride;                                    // +0x48
    virtual ~CScriptModuleBase()                          // vtable 0x01255F74 slot 0 = its ??_G
    {
        POwner = 0;
        PCameraManager = 0;
    }
};
class CScriptModuleGoToLookInDirection : public CScriptModuleBase {   // Ego_r.pdb
public:
    C3DVector GoTo;                                       // +0x4C
    C3DVector LookDirection;                              // +0x58
};
}
NCameraModes::CScriptModuleGoToLookInDirection* MakeCScriptModuleGoToLookInDirection() { return new NCameraModes::CScriptModuleGoToLookInDirection; }
void KillCScriptModuleGoToLookInDirection(NCameraModes::CScriptModuleGoToLookInDirection* p) { p->NCameraModes::CScriptModuleGoToLookInDirection::~CScriptModuleGoToLookInDirection(); }