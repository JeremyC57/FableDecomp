// CAIStateGroup_MinionReactToCriminal::CAIStateGroup_MinionReactToCriminal() -- the CAIStateGroupBase constructor, the vtable, then the two intelligent-pointer members' default constructors
// Leaf: ??0CAIStateGroup_MinionReactToCriminal@@QAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008fec9c, 52 bytes, thiscall, `ret`.
// Identity (proven): it stores vtable 0x0128B1F4 (RTTI .?AVCAIStateGroup_MinionReactToCriminal@@) after the CAIStateGroupBase constructor 0x008FD255; Ego_r.pdb ??0CAIStateGroup_MinionReactToCriminal@@QAE@XZ and members PHero (CIntelligentPointer<CThingPlayerCreature>, +0x14) and PHome (CIntelligentPointer<CThingBuilding>, +0x1C); its scalar deleting destructor's neighbour 0x00901EAD jumps here (the state-group factory).
// Callees: CAIStateGroupBase::CAIStateGroupBase() 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10.
// Header debt: engine/CAIStateGroup_MinionReactToCriminal.h / CAIStateGroupBase.h are flat layouts without bases or methods; modelled locally.
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThing;
class CThingCreatureBase;
class CThingPlayerCreature;
class CThingBuilding;
class CAIStateGroupBase {                                  // retail 0x14 bytes
public:
    CAIStateGroupBase();                                   // 0x008FD255
    virtual ~CAIStateGroupBase();
    unsigned char _pad04[0x10];                            // PBrain, ScriptAIPriority, Name, Terminated, Priority, Enabled
};
class CAIStateGroup_MinionReactToCriminal : public CAIStateGroupBase {
public:
    CAIStateGroup_MinionReactToCriminal();
    virtual ~CAIStateGroup_MinionReactToCriminal();
    CIntelligentPointer<CThingPlayerCreature> PHero;   // +0x14
    CIntelligentPointer<CThingBuilding> PHome;   // +0x1C
    long FrameLastLeftStategroup;                         // +0x24
    long FrameToNextSpeak;                                // +0x28
    bool OverrodeLocking, HeroInsideHouse, MeInsideHouse, GuardInsideHouse;   // +0x2C
};
CAIStateGroup_MinionReactToCriminal::CAIStateGroup_MinionReactToCriminal()
{
}