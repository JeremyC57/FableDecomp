// CAIStateGroup_RangedCombatUsePreferredFirePoint::CAIStateGroup_RangedCombatUsePreferredFirePoint() -- the CAIStateGroupBase constructor, the vtable, then the two intelligent-pointer members' default constructors
// Leaf: ??0CAIStateGroup_RangedCombatUsePreferredFirePoint@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00900eec, 52 bytes, thiscall, `ret`.
// Identity (proven): it stores vtable 0x0128C854 (RTTI .?AVCAIStateGroup_RangedCombatUsePreferredFirePoint@@) after the CAIStateGroupBase constructor 0x008FD255; Ego_r.pdb ??0CAIStateGroup_RangedCombatUsePreferredFirePoint@@QAE@XZ and members PEnemy (CIntelligentPointer<CThingCreatureBase>, +0x14) and PFirePoint (CIntelligentPointer<CThing>, +0x1C); reached by a tail jump from 0x00902A56.
// Callees: CAIStateGroupBase::CAIStateGroupBase() 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10.
// Header debt: engine/CAIStateGroup_RangedCombatUsePreferredFirePoint.h / CAIStateGroupBase.h are flat layouts without bases or methods; modelled locally.
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
class CAIStateGroup_RangedCombatUsePreferredFirePoint : public CAIStateGroupBase {
public:
    CAIStateGroup_RangedCombatUsePreferredFirePoint();
    virtual ~CAIStateGroup_RangedCombatUsePreferredFirePoint();
    CIntelligentPointer<CThingCreatureBase> PEnemy;   // +0x14
    CIntelligentPointer<CThing> PFirePoint;   // +0x1C
    long TimeOutCounter;                                  // +0x24
};
CAIStateGroup_RangedCombatUsePreferredFirePoint::CAIStateGroup_RangedCombatUsePreferredFirePoint()
{
}