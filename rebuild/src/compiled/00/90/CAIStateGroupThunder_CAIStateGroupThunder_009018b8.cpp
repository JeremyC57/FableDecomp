// CAIStateGroup_Thunder::CAIStateGroup_Thunder() -- the CAIStateGroupBase constructor, the vtable, then the two intelligent-pointer members' default constructors
// Leaf: ??0CAIStateGroup_Thunder@@QAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x009018b8, 52 bytes, thiscall, `ret`.
// Identity (proven): it stores vtable 0x0128CE2C (RTTI .?AVCAIStateGroup_Thunder@@) after the CAIStateGroupBase constructor 0x008FD255; Ego_r.pdb ??0CAIStateGroup_Thunder@@QAE@XZ and members PArenaCentre (CIntelligentPointer<CThing>, +0x14) and PTargetThing (CIntelligentPointer<CThingCreatureBase>, +0x1D after the bool bIsRangeAttack: the class is byte-packed in the PDB too); reached by a tail jump from 0x00902C4D.
// Callees: CAIStateGroupBase::CAIStateGroupBase() 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10.
// Header debt: engine/CAIStateGroup_Thunder.h / CAIStateGroupBase.h are flat layouts without bases or methods; modelled locally.
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
#pragma pack(push, 1)
class CAIStateGroup_Thunder : public CAIStateGroupBase {
public:
    CAIStateGroup_Thunder();
    virtual ~CAIStateGroup_Thunder();
    CIntelligentPointer<CThing> PArenaCentre;   // +0x14
    bool bIsRangeAttack;                                  // +0x1C
    CIntelligentPointer<CThingCreatureBase> PTargetThing;   // +0x1D
    long HitCount;                                        // +0x25
};
#pragma pack(pop)
CAIStateGroup_Thunder::CAIStateGroup_Thunder()
{
}