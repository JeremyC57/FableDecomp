// NCameraModes::CScriptModuleSpline::IsFinished() -- true when the camera point is gone or dead, else whether the spline really finished
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- two separate early `return true` statements (null cam point, dead cam point) instead of one `||`: VC7.1 places the shared `mov al,1` exit right after the dead-bit test as retail
// Leaf: IsFinished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00853260, length 38 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 once (PCamPoint, +0x04).
// Shape: one fetch into eax, null test, dead bit (+0x91 bit 0), then `mov al,[esi+58h] ; test al,al ;
// setne al` = an unsigned char result of `ReallyFinished != 0` (reproduced). Tried: `if (!p || dead) return
// true; return RF != 0;`, the inverted `if (p && !dead) return RF != 0; return true;`, `return !p || dead
// || RF;` (bool: full-register 0/1, 42 bytes) and the nested `if (p) { if (dead) return true; return
// RF != 0; } return true;` -- every form leaves the `return true` tail after the setne tail.
// Layout: PDB NCameraModes::CScriptModuleSpline (Index +0x4c .. ReallyFinished +0x58, FinishedSpline +0x59).
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
struct CScriptModuleSpline : CScriptModuleBase {
    float Index;                                     // +0x4c
    float CurrentSplineFOV;                          // +0x50
    float CurrentSplineRollAngle;                    // +0x54
    bool  ReallyFinished;                            // +0x58
    bool  FinishedSpline;                            // +0x59
    long  NodeToStartOn;                             // +0x5c
    long  NodeToEndOn;                               // +0x60
    unsigned char IsFinished() const;
};

unsigned char CScriptModuleSpline::IsFinished() const
{
    CThing_Retail* camPoint = PCamPoint.GetPItem();
    if (camPoint == 0)
        return true;
    if (camPoint->m_dead)
        return true;
    return ReallyFinished != 0;
}
}