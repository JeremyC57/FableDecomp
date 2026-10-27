// CVillageTaskPatrol::IsThingInvolvedWithTask(const CThing& thing) const -- true when PatrolStart or CreatureAssignedToTask resolves to the thing
// Leaf: ?IsThingInvolvedWithTask@CVillageTaskPatrol@@UBE_NABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00860390, 49 bytes, thiscall, `ret 4`.
// Identity (proven): CVillageTaskPatrol vtable 0x0125A900 (RTTI .?AVCVillageTaskPatrol@@) slot 5; the manifest label (from FableWin.pdb) is
//   ?IsThingInvolvedWithTask@CVillageTaskPatrol@@UBE_NABVCThing@@@Z; Ego_r.pdb CVillageTaskPatrol: PatrolStart (const CThingTrackNode) +0x08, CreatureAssignedToTask (const CThing) +0x10.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (twice).
// Shape (family K029, representative 0x00860310): `return PatrolStart.GetPItem() == &thing || CreatureAssignedToTask.GetPItem() == &thing;` --
//   the typed GetPItem wrappers are inline, the two exits load 0 / 1. Header debt: engine/CVillageTaskPatrol.h flattens the vptr
//   and the intelligent pointers into bytes, so the class is modelled locally from the PDB layout.
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                          // Ego_r: vptr + PData, 8 bytes
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CThing { public: virtual ~CThing(); };
class CThingTrackNode : public CThing { };
class CVillageTaskBase {                                  // RTTI base; 8 bytes (vptr + one word)
public:
    virtual ~CVillageTaskBase();
    virtual bool IsThingInvolvedWithTask(const CThing& thing) const = 0;   // slot 5 in retail
    unsigned char _pad_0x04[4];                          // +0x04
};
class CVillageTaskPatrol : public CVillageTaskBase {
public:
    virtual bool IsThingInvolvedWithTask(const CThing& thing) const;
    CIntelligentPointer<const CThingTrackNode> PatrolStart;   // +0x08
    CIntelligentPointer<const CThing> CreatureAssignedToTask;   // +0x10
    bool TaskLocked;                                     // +0x18
};
bool CVillageTaskPatrol::IsThingInvolvedWithTask(const CThing& thing) const
{
    return PatrolStart.GetPItem() == &thing || CreatureAssignedToTask.GetPItem() == &thing;
}