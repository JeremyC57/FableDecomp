// CAIStateGroup_MinionSitAtSchool::GetPOwnerObject(CThing** out) -- return the creature building when it is alive.
// Leaf: ?GetPOwnerObject@CAIStateGroup_MinionSitAtSchool@@MAE_NPAPAVCThing@@@Z
// Identity: proven; own RTTI 0128B674 slot16 and exact protected virtual PDB bool(CThing**) API; PBrain+4->Creature+24->WorkBuilding+0x170; actual inherited CBaseIntelligentPointer::GetPItem00A01B50.
// Visible callee: original landed00A01B50 body with private const ClearData and mutable private PData, unchanged actual object/refcount semantics.
// Layout: shared CAIBrain/CThingAICreature headers included unchanged; typed tracked-pointer projection over named opaque building field. Shared state-group headers flatten CBaseClass polymorphism and omit protected virtual API, so local actual CAIStateGroupBase/UsesSeat/MinionSitOnSeatBase chain models only inherited PBrain+4. Building primary CThing base is+0 from RTTI, no building fields or size used.
#define CAIBrain CAIBrain_HeaderFields
#include "engine/CAIBrain.h"
#undef CAIBrain
#include "engine/CThingAICreature.h"
// Original inline accessor: FableWin name set GetCreature@CAIBrain@@QBEAAVCThingAICreature@@XZ; inherited header Creature+0x18. Layout alias restores the omitted method without editing header data.
class CAIBrain:public CAIBrain_HeaderFields{public:CThingAICreature& GetCreature()const{return *Creature;}};
class CThing;class CThingBuilding;
class CBaseObject;
class CBaseObjectPointer {                             // Ego_r.pdb, 8 bytes
public:
    CBaseObject*  object;                              // +0x00 the observed object, 0 once it died
    unsigned long refcount;                            // +0x04 intelligent pointers sharing this record
};
class CBaseIntelligentPointer {                        // vtable 0x0129C95C
public:
    virtual ~CBaseIntelligentPointer();                // 0x00A01C10
    CBaseObject* GetPItem() const;                     // 0x00A01B50
private:
    friend struct TestTracking;
    void ClearData() const                             // 0x00A01B20 (private const in Ego_r), inlined here
    {
        if (PData != 0) {
            PData->refcount--;
            if (PData->refcount == 0 && PData->object == 0)
                delete PData;
            PData = 0;
        }
    }
    mutable CBaseObjectPointer* PData;                 // +0x04
};

#pragma auto_inline(off)
CBaseObject* CBaseIntelligentPointer::GetPItem() const
{
    if (PData != 0) {
        if (PData->object != 0)
            return PData->object;
        ClearData();
    }
    return 0;
}

#pragma auto_inline(on)
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T* GetPItem()const{return (T*)CBaseIntelligentPointer::GetPItem();}};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CAIStateGroupBase:public CBaseClass{public:CAIBrain* PBrain;};class CAIStateGroup_UsesSeat:public CAIStateGroupBase{};class CAIStateGroup_MinionSitOnSeatBase:public CAIStateGroup_UsesSeat{};
class CAIStateGroup_MinionSitAtSchool:public CAIStateGroup_MinionSitOnSeatBase{protected:virtual bool GetPOwnerObject(CThing**);};
bool CAIStateGroup_MinionSitAtSchool::GetPOwnerObject(CThing** out){CThing* owner=(CThing*)((CIntelligentPointer<CThingBuilding>&)PBrain->GetCreature().WorkBuilding).GetPItem();if(owner){*out=owner;return true;}return false;}