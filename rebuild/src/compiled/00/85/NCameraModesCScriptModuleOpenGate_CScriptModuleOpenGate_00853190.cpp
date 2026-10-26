// NCameraModes::CScriptModuleOpenGate::CScriptModuleOpenGate(const CThing* gate, long duration) -- constructor: the self-terminating base for `duration` frames, then PGate pointed at the gate
// Leaf: ??0CScriptModuleOpenGate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00853190, length 74 bytes.
// Callees: CScriptModuleBase::CScriptModuleBase() @ 0x005ED070, GFGetCurrentFrame @ 0x0049D870,
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Vtables: 0x012759CC (CScriptModuleSelfTerminateBase, stored by the inlined base constructor), 0x01275B0C
// (NCameraModes::CScriptModuleOpenGate), 0x01237F1C (CIntelligentPointer<const CThing>).
// Shape: the CScriptModuleSelfTerminateBase constructor (0x00852490) is inlined whole -- its vtable store
// survives because the frame call between it and the derived store is opaque; then the derived vtable,
// and PGate's converting constructor (base call, vtable, SetPItem(gate)). Pos/DeltaPos (+0x5c/+0x68,
// plain C3DVectors) are not initialised.
// Layout: PDB NCameraModes::CScriptModuleOpenGate (0x74 bytes): PGate +0x54, Pos +0x5c, DeltaPos +0x68.
long __fastcall GFGetCurrentFrame();                 // 0x0049D870

class CBaseObject;
class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                       // 0x00A01B10
    void SetPItem(const CBaseObject* item);          // 0x00A01B90
};

struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer_CThingConst(const CThing* thing) : CBaseIntelligentPointer() { SetPItem((const CBaseObject*)thing); }
};

class C3DVector { public: float X, Y, Z; };

struct CScriptModuleBase {
    virtual void v00();
    unsigned char m_state[0x4c - 4];
    CScriptModuleBase();                             // 0x005ED070
};

namespace NCameraModes {

struct CScriptModuleSelfTerminateBase : CScriptModuleBase {   // vtable 0x012759CC
    long WFDuration;                                 // +0x4c
    long EndFrame;                                   // +0x50
    virtual void v00();
    CScriptModuleSelfTerminateBase(long duration)
        : WFDuration(duration)
    {
        EndFrame = GFGetCurrentFrame() + WFDuration;
    }
};

struct CScriptModuleOpenGate : CScriptModuleSelfTerminateBase {   // vtable 0x01275B0C
    CIntelligentPointer_CThingConst PGate;           // +0x54
    C3DVector                       Pos;             // +0x5c
    C3DVector                       DeltaPos;        // +0x68
    virtual void v00();
    CScriptModuleOpenGate(const CThing* gate, long duration);
};

CScriptModuleOpenGate::CScriptModuleOpenGate(const CThing* gate, long duration)
    : CScriptModuleSelfTerminateBase(duration), PGate(gate)
{
}

}