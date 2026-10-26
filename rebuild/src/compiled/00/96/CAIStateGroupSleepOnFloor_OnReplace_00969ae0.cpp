// CAIStateGroup_SleepOnFloor::OnReplace() -- release the sleep point (clear its occupant when it is this creature, drop the pointer); when asleep, play ST_SLEEP_ON_FLOOR_OUTOF if the creature is free to act and remove creature mode 0xD
// Leaf: OnReplace
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00969AE0, length 460 bytes (plain `ret`, virtual slot 10).
// Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10 (temporary
//          vtable 0x01238C6C), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface ternary form: 0xAC the occupiable
//          sleep point = bit 12 of +0x34, 0x31 CTCCreatureModeManager = bit 17 of +0x24; misses read the never-written
//          key slot), the point's GetOccupant @ 0x0087C3E0 / SetOccupant @ 0x0087C390 (manifest "CVillageTaskCarryCrate::
//          AssignCreatureToTask" is propagated), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0,
//          CCreatureAction_PlayAnimation ctor @ 0x008424F0 / ~CCreatureActionBase @ 0x00693EF0, CBrainOutput ctor
//          @ 0x00835620 (implicit dtor inlined: IP +0x40, then the counted pointer +0x20), CThingCreatureBase::
//          IsFreeToPerformAction @ 0x00662210, the action's virtual slot 11 (+0x2C, Clone), CCountedPointer::Reset
//          @ 0x00668D90, CAIBrain::SetBrainOutput @ 0x0088B8F0, CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, operator
//          delete @ 0x00BFE9BC.
// Layout: Ego_r.pdb CAIStateGroup_SleepOnFloor (PSleepPoint +0x14, Asleep +0x1c) over the landed CAIStateGroupBase model
//          (the generated header flattens the vptr); CBrainOutput as the landed AIAction_PerformAction 0x0097D8B0.
// Shape: the creature is a named local read before the occupant call (so the shared 0 lands in edi on both paths); the
//          animation name is the IMPLICIT const char* -> CCharString conversion (built before the other arguments, like
//          the landed 0x008424F0 callers); the action is used through a CCreatureActionBase* (a direct `action.Clone()`
//          is devirtualised); the brain-output test goes through an inline creature getter (register rotation).
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
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
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
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x0c];
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
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
};
struct CAIStateGroup_SleepOnFloor : CAIStateGroupBase {
    CIntelligentPointerThing PSleepPoint;       // +0x14
    bool Asleep;                                // +0x1c
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PBrain->PCreature; }
    CThingCreature* GetCreatureThing() const { return PBrain->PCreature; }
    virtual void OnReplace();
};

void CAIStateGroup_SleepOnFloor::OnReplace()
{
    CTCOccupiable* point;
    PSleepPoint.GetPItem()->GetInterface(&point, 0xac);
    CThing* creature = PBrain->PCreature;
    if (point->GetOccupant() == creature)
        point->SetOccupant(0);
    PSleepPoint = CIntelligentPointerThing(0);
    if (Asleep) {
        CCreatureAction_PlayAnimation action(GetCreature(), "ST_SLEEP_ON_FLOOR_OUTOF", CSoundPair(), false, true, false, 0, 0);
        CCreatureActionBase* pAction = &action;
        CBrainOutput output;
        if (GetCreatureThing()->IsFreeToPerformAction(pAction)) {
            output.Flags |= 8;
            output.PAction.Reset(pAction->Clone());
            PBrain->SetBrainOutput(output);
        }
        CTCCreatureModeManager* modes;
        PBrain->PCreature->GetInterface(&modes, 0x31);
        modes->RemoveMode(0xd);
    }
}