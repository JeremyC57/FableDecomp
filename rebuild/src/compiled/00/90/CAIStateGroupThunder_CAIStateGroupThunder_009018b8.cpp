// CAIStateGroup_Thunder::CAIStateGroup_Thunder() -- the CAIStateGroupBase constructor, the vtable, then the two intelligent-pointer members' default constructors
// Leaf: ??0CAIStateGroup_Thunder@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009018b8, 52 bytes, thiscall, `ret`.
// Identity (proven): it stores vtable 0x0128CE2C (RTTI .?AVCAIStateGroup_Thunder@@) after the CAIStateGroupBase constructor 0x008FD255; Ego_r.pdb ??0CAIStateGroup_Thunder@@QAE@XZ and members PArenaCentre (CIntelligentPointer<CThing>, +0x14) and PTargetThing (CIntelligentPointer<CThingCreatureBase>, +0x1D after the bool bIsRangeAttack: the class is byte-packed in the PDB too); reached by a tail jump from 0x00902C4D.
// Callees: CAIStateGroupBase::CAIStateGroupBase() 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10.
// Header debt: engine/CAIStateGroup_Thunder.h / CAIStateGroupBase.h are flat layouts without bases or methods; modelled locally.
class CBaseObjectPointer;
class CBaseIntelligentPointer {                            // Ego_r: vptr + PData, 8 bytes
public:
    CBaseIntelligentPointer();                             // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10
private:
    CBaseObjectPointer* PData;                             // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
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