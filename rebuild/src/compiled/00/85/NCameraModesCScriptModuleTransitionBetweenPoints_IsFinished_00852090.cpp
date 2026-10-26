// NCameraModes::CScriptModuleTransitionBetweenPoints::IsFinished() -- true when self-termination is allowed (not PreventSelfTermination, SelfTerminate set) and the current frame is past EndFrame
// Leaf: IsFinished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852090, length 41 bytes.
// Callees: GFGetCurrentFrame @ 0x0049D870 (`__fastcall`, no arguments).
// Shape: one `if (a && b && c) return true; return false;` -- the two bool members tested with
// `mov al,[..] ; test al,al` (PreventSelfTermination +0x8c first, the base's SelfTerminate +0x40 second),
// then the signed `>` against EndFrame (+0x84, `jle` to the false exit); two exits (`mov al,1` / `xor al,al`).
// Layout: NCameraModes::CScriptModuleBase members from the PDB (0x4c bytes) and the derived members of
// CScriptModuleTransitionBetweenPoints (PDB, 0x90 bytes). No engine header exists for these classes.
long __fastcall GFGetCurrentFrame();                 // 0x0049D870
class CBaseObject;
struct CBase;
class CGameCameraManager;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };

struct CThing_Retail {
    unsigned char  _pad00[0x91];
    unsigned char  m_dead : 1;                       // +0x91 bit 0
    unsigned char  m_91_1 : 7;
};

struct CIntelligentPointer_CThingConst {             // CIntelligentPointer<const CThing>: vptr + link
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem() const;                 // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
};

struct CScriptModuleBase {                           // NCameraModes::CScriptModuleBase, 0x4c bytes (PDB)
    virtual void v00();
    CIntelligentPointer_CThingConst PCamPoint;       // +0x04
    CIntelligentPointer_CThingConst PThingOrigin;    // +0x0c
    C3DVector           CoordOrigin;                 // +0x14
    CRightHandedSet     CoordAxis;                   // +0x20
    CBase*              POwner;                      // +0x38
    CGameCameraManager* PCameraManager;              // +0x3c
    bool                SelfTerminate;               // +0x40
    bool                ShortenOnCollision;          // +0x41
    long                CutOutOverride;              // +0x44
    float               FOVOverride;                 // +0x48
};
namespace NCameraModes {
struct CScriptModuleTransitionBetweenPoints : CScriptModuleBase {
    CIntelligentPointer_CThingConst PThing;          // +0x4c
    C3DVector Increment;                             // +0x54
    C3DVector LookAtIncrement;                       // +0x60
    C3DVector LookAtPos;                             // +0x6c
    C3DVector StartPos;                              // +0x78
    long      EndFrame;                              // +0x84
    float     FOV;                                   // +0x88
    bool      PreventSelfTermination;                // +0x8c
    bool IsFinished() const;
};

bool CScriptModuleTransitionBetweenPoints::IsFinished() const
{
    if (!PreventSelfTermination && SelfTerminate && GFGetCurrentFrame() > EndFrame)
        return true;
    return false;
}
}