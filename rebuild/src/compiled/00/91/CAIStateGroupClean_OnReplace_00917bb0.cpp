// CAIStateGroup_Clean::OnReplace() -- stop kneeling / stop the cleaning action (posting the ST_WASH_FLOOR_OUTOF get-up animation when the creature is free), reset the clean timer to 100 seconds, pick a new idle and destroy the right-hand item
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00917BB0, length 432 bytes. Vtable slot (OnReplace).
// Leaf: OnReplace@CAIStateGroup_Clean
// Callees: CThingCreatureBase::FinishCurrentAction @ 0x00662650 / IsPerformingAnyAction @ 0x006622A0 / IsFreeToPerformAction
//          @ 0x00662210, action-queue current id @ 0x00692620 (manifest "CWorld::DrawGetEnvironment" is propagated),
//          CBrainOutput @ 0x00835620 (implicit dtor inlined: counted pointer +0x20, ~CBaseIntelligentPointer @ 0x00A01C10 +0x40),
//          CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, CCreatureAction_PlayAnimation @ 0x008424F0, ~CCreatureActionBase
//          @ 0x00693EF0, CCountedPointer::Reset @ 0x00668D90, CAIBrain::SetBrainOutput @ 0x0088B8F0 (manifest
//          "CQuestManager::RemoveAvailableQuestCardType" is propagated), CWorld::GetFrame @ 0x0049D870, g_framesPerSecond
//          @ 0x01375550, CVectorMap::LowerBound @ 0x0040F020 (0xd3), CTCIdleScheduler::ChooseNewIdle @ 0x00801A50,
//          CAIStateGroupBase::DestroyItem @ 0x008FD1B0.
// Shape: the AIAction_PerformAction body (0x0097D8B0) inlined without the waits: the animation name is an implicit
//          const char* -> CCharString conversion temporary (built before the argument pushes, dead after the ctor); the
//          Clone goes through a CCreatureActionBase* (a call on the local object is devirtualised); direct PBrain->PCreature
//          for the first uses and the GetCreature() accessor for the IsFree / idle lookups (register roles); fps is a named
//          local read before GFGetCurrentFrame.
// Layout: PDB CAIStateGroup_Clean: Cleaning +0x14, KneelingIntoCleaning +0x15, FrameToClean +0x18, CleanActionID +0x1c;
//          no engine header for this class, modelled locally.
class CTCBase;
class CThing;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CCreatureActionBase;
struct CCreatureActionQueue {
    long GetCurrentActionID();                  // 0x00692620
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public:
    unsigned char _pad4c[0x100 - 0x4c];
    CCreatureActionQueue* PActionQueue;         // +0x100
    void FinishCurrentAction();                 // 0x00662650
    bool IsPerformingAnyAction();               // 0x006622A0
    bool IsFreeToPerformAction(const CCreatureActionBase* action);   // 0x00662210
};
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
struct CCountBlock {
    long Count;
    void (__fastcall* Destroy)(void* object);
    void* Object;
};
struct CCountedPointer {
    void* Object;
    CCountBlock* Block;
    void Reset(void* object);                   // 0x00668D90
    ~CCountedPointer()
    {
        if (Block) {
            --Block->Count;
            if (Block->Count == 0) {
                Block->Destroy(Block->Object);
                delete Block;
            }
        }
        Object = 0;
        Block = 0;
    }
};
class CBrainOutput { public:                           // 0x48 bytes
    unsigned long Flags;
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointer PAction;                    // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;            // +0x40
    CBrainOutput();                             // 0x00835620
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
    void SetBrainOutput(const CBrainOutput& output);   // 0x0088B8F0
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual void* Clone() const;                // slot 11 (+0x2c)
    unsigned char _body[0xa4];
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};
class CTCIdleScheduler { public:                       // interface 0xd3
    void ChooseNewIdle();                       // 0x00801A50
};
class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870
extern long g_framesPerSecond;                  // 0x01375550
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    void DestroyItem(const CCharString& itemName);   // 0x008FD1B0
};
struct CAIStateGroup_Clean : CAIStateGroupBase {
    bool Cleaning;                              // +0x14
    bool KneelingIntoCleaning;                  // +0x15
    long FrameToClean;                          // +0x18
    long CleanActionID;                         // +0x1c
    float GroundFloorZPos;                      // +0x20
    bool Initialised;                           // +0x24
    virtual void OnReplace();
};

void CAIStateGroup_Clean::OnReplace()
{
    if (KneelingIntoCleaning) {
        PBrain->PCreature->FinishCurrentAction();
        KneelingIntoCleaning = false;
    } else if (Cleaning) {
        if (PBrain->PCreature->IsPerformingAnyAction() && PBrain->PCreature->PActionQueue->GetCurrentActionID() == CleanActionID)
            PBrain->PCreature->FinishCurrentAction();
        Cleaning = false;
        CBrainOutput output;
        CCreatureAction_PlayAnimation action(PBrain->PCreature, "ST_WASH_FLOOR_OUTOF", CSoundPair(), false, true, false, 0, 0);
        if (GetCreature()->IsFreeToPerformAction(&action)) {
            output.Flags |= 8;
            CCreatureActionBase* performed = &action;
            output.PAction.Reset(performed->Clone());
            PBrain->SetBrainOutput(output);
        }
    }
    CleanActionID = -1;
    long fps = g_framesPerSecond;
    FrameToClean = CWorld::GetFrame() + fps * 100;
    CTCIdleScheduler* scheduler;
    if (GetCreature()->GetInterface(&scheduler, 0xd3))
        scheduler->ChooseNewIdle();
    DestroyItem("CARRY_SLOT_RIGHT_HAND");
}