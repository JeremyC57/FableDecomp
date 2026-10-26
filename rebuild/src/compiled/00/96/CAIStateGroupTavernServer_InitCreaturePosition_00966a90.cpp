// CAIStateGroup_TavernServer::InitCreaturePosition() -- place the server beside the pub when the state is runnable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00966A90, length 56 bytes (+ int3 padding).
// Callees: virtual slot 11 (+0x2c, IsRunnable) on this, CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (PPub at +0x34, engine header CAIStateGroup_TavernServer.h),
// CThingCreatureBase::AutoPlaceNearBuilding(CThingBuilding*, float) @ 0x00666EF0.
// Shape: identical to CAIStateGroup_TavernKeeper::InitCreaturePosition @ 0x009681A0 with the pub
// pointer at +0x34: runnable test, creature in a named local (edi) before the 5.0f push, long
// result (`mov eax,1` / shared `xor eax,eax`). Base modelled locally (flattened header).
class CThingBuilding;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CThingAICreature { public:
    bool AutoPlaceNearBuilding(CThingBuilding* building, float radius);    // 0x00666EF0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_TavernServer : CAIStateGroupBase {
    CIntelligentPointer<CThingBuilding> PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThingBuilding> PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThingBuilding> PTable;             // +0x24 CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThingBuilding> PTankard;           // +0x2c CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThingBuilding> PPub;               // +0x34 CIntelligentPointer<CThingBuilding>
    long InitCreaturePosition();
};

long CAIStateGroup_TavernServer::InitCreaturePosition()
{
    if (IsRunnable()) {
        CThingAICreature* creature = PBrain->Creature;
        if (creature->AutoPlaceNearBuilding(PPub.GetPItem(), 5.0f))
            return 1;
    }
    return 0;
}