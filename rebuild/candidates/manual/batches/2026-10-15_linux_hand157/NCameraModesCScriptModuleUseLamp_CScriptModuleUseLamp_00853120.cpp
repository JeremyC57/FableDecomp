// NCameraModes::CScriptModuleUseLamp::CScriptModuleUseLamp(const CThing* creature, const CThing* lamp, long duration) -- constructor: the self-terminating base for `duration` frames, then PCreature and PLamp pointed at the two things
// Leaf: ??0CScriptModuleUseLamp
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00853120, length 112 bytes (100 + int3 padding).
// Callees: NCameraModes::CScriptModuleBase::CScriptModuleBase() @ 0x005ED070, GFGetCurrentFrame @ 0x0049D870,
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, ::SetPItem @ 0x00A01B90 (twice).
// Vtables: 0x012759CC (CScriptModuleSelfTerminateBase, stored by the inlined base ctor), 0x01275AE4
// (NCameraModes::CScriptModuleUseLamp, rtti-vtables.tsv), 0x01237F1C (CIntelligentPointer<const CThing>).
// Label correction: the catalog calls this a second CScriptModuleOpenChest ctor; the vtable it stores
// (0x01275AE4) is NCameraModes::CScriptModuleUseLamp (same 0xa0-byte layout, PLamp in place of PChest).
// Shape: the landed CScriptModuleOpenGate ctor (0x00853190) with two intelligent-pointer members: the
// CScriptModuleSelfTerminateBase ctor inlined whole (its vtable store survives the opaque frame call),
// the derived vtable, then each member's converting ctor (base call, vtable, SetPItem). The orbit
// fields (Origin .. AxisY) are not initialised here.
// Layout: PDB NCameraModes::CScriptModuleUseLamp (0xa0 bytes): PCreature +0x54, PLamp +0x5c, Origin +0x64, Theta +0x70,
// Rad +0x74, Height +0x78, DeltaTheta +0x7c, DeltaRad +0x80, DeltaHeight +0x84, AxisX +0x88, AxisY +0x94.
long __fastcall GFGetCurrentFrame();                 // 0x0049D870

struct CBaseObject;
struct CThing;

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

struct C3DVector { float X, Y, Z; };

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

struct CScriptModuleUseLamp : CScriptModuleSelfTerminateBase {   // vtable 0x01275AE4
    CIntelligentPointer_CThingConst PCreature;       // +0x54
    CIntelligentPointer_CThingConst PLamp;            // +0x5c
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
    CScriptModuleUseLamp(const CThing* creature, const CThing* lamp, long duration);
};

CScriptModuleUseLamp::CScriptModuleUseLamp(const CThing* creature, const CThing* lamp, long duration)
    : CScriptModuleSelfTerminateBase(duration), PCreature(creature), PLamp(lamp)
{
}

}
