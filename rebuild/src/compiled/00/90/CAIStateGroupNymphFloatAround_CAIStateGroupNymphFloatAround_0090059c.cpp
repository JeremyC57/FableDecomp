// CAIStateGroup_NymphFloatAround::CAIStateGroup_NymphFloatAround() -- the CAIStateGroupBase constructor, the vtable, then the two intelligent-pointer members' default constructors
// Leaf: ??0CAIStateGroup_NymphFloatAround@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0090059c, 52 bytes, thiscall, `ret`.
// Identity (proven): it stores vtable 0x0128C134 (RTTI .?AVCAIStateGroup_NymphFloatAround@@) after the CAIStateGroupBase constructor 0x008FD255; Ego_r.pdb ??0CAIStateGroup_NymphFloatAround@@QAE@XZ and members PEnemy (CIntelligentPointer<CThingCreatureBase>, +0x14) and LimboEffect (CIntelligentPointer<CThing>, +0x1C); reached by a tail jump from 0x00902694.
// Callees: CAIStateGroupBase::CAIStateGroupBase() 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10.
// Header debt: engine/CAIStateGroup_NymphFloatAround.h / CAIStateGroupBase.h are flat layouts without bases or methods; modelled locally.
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
class CAIStateGroup_NymphFloatAround : public CAIStateGroupBase {
public:
    CAIStateGroup_NymphFloatAround();
    virtual ~CAIStateGroup_NymphFloatAround();
    CIntelligentPointer<CThingCreatureBase> PEnemy;   // +0x14
    CIntelligentPointer<CThing> LimboEffect;   // +0x1C
};
CAIStateGroup_NymphFloatAround::CAIStateGroup_NymphFloatAround()
{
}