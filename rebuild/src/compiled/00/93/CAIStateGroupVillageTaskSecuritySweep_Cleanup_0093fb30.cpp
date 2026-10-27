// CAIStateGroup_VillageTaskSecuritySweep::Cleanup() -- if the group still holds its task, remove the creature from it through the village and clear PTask
// Leaf: ?Cleanup@CAIStateGroup_VillageTaskSecuritySweep@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093fb30, 140 bytes, thiscall, `ret`.
// Identity (proven): vtable 0x0128BAA0 (RTTI .?AVCAIStateGroup_VillageTaskSecuritySweep@@) slot 9 (slot 9 is Cleanup, slot 10 OnReplace in these state groups' vtables); Ego_r.pdb ?Cleanup@CAIStateGroup_VillageTaskSecuritySweep@@UAEXXZ; Ego_r.pdb members PTCVillageMember (+0x14) and PTask (CIntelligentPointer<CVillageTaskSecuritySweep>, vtable 0x0128B098, +0x18).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CTCVillageMember::GetPVillage() const 0x007EE540,
//   CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 / ~CBaseIntelligentPointer 0x00A01C10,
//   CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase>, const CThing&) 0x00659EFC (Ego_r.pdb).
// Body: the village fetched into a named local first (retail calls GetPVillage before building the arguments); the by-value task argument is built in its stack slot by CIntelligentPointer<CVillageTaskBase>'s converting constructor
//   (Ego_r.pdb ??$?0VCVillageTaskSecuritySweep@@@?$CIntelligentPointer@VCVillageTaskBase@@@@QAE@AAV?$CIntelligentPointer@VCVillageTaskSecuritySweep@@@@@Z, inline);
//   `PTask = 0` goes through a CIntelligentPointer<CVillageTaskSecuritySweep> temporary (constructor, operator=, destructor).
// Header debt: engine/CAIStateGroup_VillageTaskSecuritySweep.h / CAIStateGroupBase.h / CTCVillage.h are flat layouts; modelled locally.
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                            // Ego_r: vptr + PData, 8 bytes
public:
    CBaseIntelligentPointer();                             // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10
    CBaseObject* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CBaseObject* item);                      // 0x00A01B90
private:
    CBaseObjectPointer* PData;                             // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    template <class U> CIntelligentPointer(CIntelligentPointer<U>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CThing { };
class CThingAICreature : public CThing { };
class CVillageTaskBase { };
class CVillageTaskSecuritySweep : public CVillageTaskBase { };
class CTCVillage {
public:
    void RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase> task, const CThing& thing);   // 0x00659EFC
};
class CTCVillageMember {
public:
    CTCVillage* GetPVillage() const;                       // 0x007EE540
};
class CAIBrain {
public:
    unsigned char     _pad00[0x18];
    CThingAICreature* Creature;                            // +0x18
    CThingAICreature& GetCreature() const { return *Creature; }   // Ego_r.pdb ?GetCreature@CAIBrain@@QBEAAVCThingAICreature@@XZ (inline here)
};
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain*     PBrain;                                  // +0x04
    unsigned char _pad08[0x0C];
};
class CAIStateGroup_VillageTaskSecuritySweep : public CAIStateGroupBase {
public:
    CTCVillageMember*               PTCVillageMember;      // +0x14
    CIntelligentPointer<CVillageTaskSecuritySweep> PTask;                      // +0x18
    virtual void Cleanup();
};
void CAIStateGroup_VillageTaskSecuritySweep::Cleanup()
{
    if (PTask.GetPItem()) {
        CTCVillage* village = PTCVillageMember->GetPVillage();
        village->RemoveThingFromTask(PTask, PBrain->GetCreature());
        PTask = 0;
    }
}