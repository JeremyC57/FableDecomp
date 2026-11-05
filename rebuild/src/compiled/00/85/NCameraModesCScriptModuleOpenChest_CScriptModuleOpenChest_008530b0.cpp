// NCameraModes::CScriptModuleOpenChest::CScriptModuleOpenChest(const CThing* creature, const CThing* chest, long duration) -- constructor: the self-terminating base for `duration` frames, then PCreature and PChest pointed at the two things
// Leaf: ??0CScriptModuleOpenChest
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008530B0, length 112 bytes (100 + int3 padding).
// Callees: NCameraModes::CScriptModuleBase::CScriptModuleBase() @ 0x005ED070, CWorld::GetFrame @ 0x0049D870,
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, ::SetPItem @ 0x00A01B90 (twice).
// Vtables: 0x012759CC (CScriptModuleSelfTerminateBase, stored by the inlined base ctor), 0x01275ABC
// (NCameraModes::CScriptModuleOpenChest, rtti-vtables.tsv), 0x01237F1C (CIntelligentPointer<const CThing>).
// Shape: the landed CScriptModuleOpenGate ctor (0x00853190) with two intelligent-pointer members: the
// CScriptModuleSelfTerminateBase ctor inlined whole (its vtable store survives the opaque frame call),
// the derived vtable, then each member's converting ctor (base call, vtable, SetPItem). The orbit
// fields (Origin .. AxisY) are not initialised here.
// Layout: PDB NCameraModes::CScriptModuleOpenChest (0xa0 bytes): PCreature +0x54, PChest +0x5c, Origin +0x64, Theta +0x70,
// Rad +0x74, Height +0x78, DeltaTheta +0x7c, DeltaRad +0x80, DeltaHeight +0x84, AxisX +0x88, AxisY +0x94.
class CWorld { public: static long __fastcall GetFrame(); };                 // 0x0049D870

class CBaseObject;
class CThing;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer_CThingConst(const CThing* thing) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)((const CBaseObject*)thing)); }
};

#include "engine/C3DVector.h"

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
        EndFrame = CWorld::GetFrame() + WFDuration;
    }
};

struct CScriptModuleOpenChest : CScriptModuleSelfTerminateBase {   // vtable 0x01275ABC
    CIntelligentPointer_CThingConst PCreature;       // +0x54
    CIntelligentPointer_CThingConst PChest;           // +0x5c
    C3DVector Origin;                                // +0x64
    float     Theta;                                 // +0x70
    float     Rad;                                   // +0x74
    float     Height;                                // +0x78
    float     DeltaTheta;                            // +0x7c
    float     DeltaRad;                              // +0x80
    float     DeltaHeight;                           // +0x84
    C3DVector AxisX;                                 // +0x88
    C3DVector AxisY;                                 // +0x94
    virtual void v00();
    CScriptModuleOpenChest(const CThing* creature, const CThing* chest, long duration);
};

CScriptModuleOpenChest::CScriptModuleOpenChest(const CThing* creature, const CThing* chest, long duration)
    : CScriptModuleSelfTerminateBase(duration), PCreature(creature), PChest(chest)
{
}

}