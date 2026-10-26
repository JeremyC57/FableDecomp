// CAIStateGroup_ScreamerPursueTarget::OnReplace() -- if the target is alive, flag it (interface 0x36,
// SetAsInDanger(creature, 0)); then, if the target is still alive and the creature is not in mode 0x44, perform a
// CCreatureAction_ScreamerIntoAggressive on it through a CBrainOutput (flag 8)
// Leaf: OnReplace
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096DDB0, length 401 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10, CVectorMap::LowerBound @ 0x0040F020
//          (inlined GetInterface: 0x36 = bit 22 of +0x24, 0x31 = bit 17 of +0x24, miss reads the key slot),
//          SetAsInDanger @ 0x00747B80, CTCCreatureModeManager::HasMode @ 0x0073A8A0 (manifest "CPlayer::HasMode"),
//          CBrainOutput ctor @ 0x00835620, CCombatActionBase(creature, target, 0x96, CSoundPair) @ 0x00857F30 then the
//          derived vtable 0x01296F14 (CCreatureAction_ScreamerIntoAggressive, rtti), its Clone through the folded
//          vtable slot [0x01296F40], CCountedPointer::Reset @ 0x00668D90, ~CCombatActionBase @ 0x0062E900,
//          CAIBrain::SetBrainOutput @ 0x0088B8F0, operator delete @ 0x00BFE9BC (count block, inlined counted-pointer dtor).
// Layout: Ego_r.pdb CAIStateGroup_ScreamerPursueTarget (PTarget +0x14, TickToGetAggressive +0x1c); CBrainOutput as the
//         landed 0x0097D8B0 / 0x00969AE0 model; CCombatActionBase sizeof 0x114 (landed Clone 0x0096E510).
// Shape: the derived action ctor is an inline (forced) wrapper over the out-of-line base ctor so the vptr store is
//         visible and the virtual Clone folds to [vtable+0x2c]; the action lives in its own block (destroyed before
//         SetBrainOutput); CBrainOutput's dtor is inline (forced) -- implicit, it was emitted out of line here.
class CTCBase;
class CThingCreatureBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {        // CIntelligentPointer<CThing> (vtable 0x01238C6C)
    virtual void v00();
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
struct CTCOccupiable {                          // interface 0xac
    CThing* GetOccupant();                      // 0x0087C3E0
    void SetOccupant(CThing* occupant);         // 0x0087C390
};
class CTCCreatureModeManager { public:                 // interface 0x31
    bool HasMode(long mode);                    // 0x0073A8A0
};
class CTCStealth { public:                             // interface 0x36
    void SetAsInDanger(CThing* thing, long status);     // 0x00747B80
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
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
    virtual void* Clone() const;                    // slot 11 (+0x2c)
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                         // 0x00693EF0
};
class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _combat[0x114 - 0xa8];
    CCombatActionBase(CThingCreatureBase* creature, CThing* target, long action, const CSoundPair& sound);   // 0x00857F30
    ~CCombatActionBase();                           // 0x0062E900
};
struct CCreatureAction_ScreamerIntoAggressive : CCombatActionBase {   // vtable 0x01296F14, sizeof 0x114
    virtual void* Clone() const;                    // 0x0096E510
    __forceinline CCreatureAction_ScreamerIntoAggressive(CThingCreatureBase* creature, CThing* target)
        : CCombatActionBase(creature, target, 0x96, CSoundPair()) { }
};
struct CCountBlock { long Count; void (__fastcall* Destroy)(void* object); void* Object; };
struct CCountedPointer {
    void* Object;
    CCountBlock* Block;
    void Reset(void* object);                       // 0x00668D90
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
class CBrainOutput { public:                               // 0x48 bytes
    unsigned long Flags;                            // +0x00 (8: perform action)
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointer PAction;                        // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;                // +0x40
    CBrainOutput();                                 // 0x00835620
    __forceinline ~CBrainOutput() { }
};
struct CThingCreature : CThing {
    bool IsFreeToPerformAction(const CCreatureActionBase* action);   // 0x00662210
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreature* PCreature;                      // +0x18
    void SetBrainOutput(const CBrainOutput& output);  // 0x0088B8F0
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
};struct CAIStateGroup_ScreamerPursueTarget : CAIStateGroupBase {
    CIntelligentPointerThing PTarget;           // +0x14
    unsigned long TickToGetAggressive;          // +0x1c
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PBrain->PCreature; }
    virtual void OnReplace();
};

void CAIStateGroup_ScreamerPursueTarget::OnReplace()
{
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1)) {
        CTCStealth* stealth;
        if (PTarget.GetPItem()->GetInterface(&stealth, 0x36))
            stealth->SetAsInDanger(PBrain->PCreature, 0);
    }
    CTCCreatureModeManager* modes;
    PBrain->PCreature->GetInterface(&modes, 0x31);
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1) && !modes->HasMode(0x44)) {
        CBrainOutput output;
        {
            CCreatureAction_ScreamerIntoAggressive action(GetCreature(), PTarget.GetPItem());
            CCreatureActionBase* pAction = &action;
            output.Flags |= 8;
            output.PAction.Reset(pAction->Clone());
        }
        PBrain->SetBrainOutput(output);
    }
}