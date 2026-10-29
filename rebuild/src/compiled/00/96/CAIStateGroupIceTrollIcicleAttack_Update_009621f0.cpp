// CAIStateGroup_IceTrollIcicleAttack::Update() -- with a live target: mark the target's stealth component in danger, rise up if knocked down, face the target, then post the icicle attack through a brain output for two free frames, wait out any running action and boast; terminate at the end or on any failure
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009621F0, length 733 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10,
// CVectorMap::LowerBound @ 0x0040F020, CTCStealth::SetAsInDanger @ 0x00747B80, CTCCreatureModeManager::HasMode @ 0x0073A8A0,
// CCreatureAction_RiseUp ctor @ 0x008BD880, ~CCombatActionBase @ 0x0062E900, AIAction_PerformActionAndWait @ 0x0097DB70,
// AIAction_LookAtThing @ 0x0097D6F0, IsStopping @ 0x008FD010, CCombatActionBase ctor @ 0x00857F30 (code 0x96; vtable
// CCreatureAction_IceTrollIcicleAttack 0x012960C4), CThingCreatureBase::IsFreeToPerformAction @ 0x00662210 /
// IsPerformingAnyAction @ 0x006622A0, CBrainOutput ctor @ 0x00835620, CCountedPointer::Reset @ 0x00668D90,
// CAIBrain::SetBrainOutput @ 0x0088B8F0 / Yield (virtual slot 2), operator delete @ 0x00BFE9BC, AIAction_DoNothing
// @ 0x0097E200, CCharString ctor @ 0x0099EBF0 / dtor @ 0x0099EAE0, CCreatureAction_PlayCombatAnimation ctor @ 0x008B7FF0,
// AIAction_CombatPerformActionAndWait @ 0x0097DA40, Terminate @ 0x008FCF40.
// Shape: the component tests are `bool has = HasTC()` locals (shr/and stored to a frame byte); the stealth lookup is a
// direct GetTC into a named pointer; the boast result is compared against 1 and both arms terminate (the compare folds
// away, the result copy in edi stays).
// Layout: Ego_r.pdb CAIStateGroup_IceTrollIcicleAttack PTarget +0x14.
class CThing;
class CThingCreatureBase;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long t) const { return (m_presentBits[t >> 5] >> (t & 31)) & 1; }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        bool has = HasTC(interfaceType);
        if (has) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CCreatureActionBase;
class CThingCreatureBase : public CThing { public:
    bool IsFreeToPerformAction(const CCreatureActionBase* action);   // 0x00662210
    bool IsPerformingAnyAction();               // 0x006622A0
};
class CTCStealth { public:                             // interface 0x36
    void SetAsInDanger(CThing* source, long status);   // 0x00747B80
};
class CTCCreatureModeManager { public:                 // interface 0x31
    bool HasMode(long mode);                    // 0x0073A8A0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(const CBaseIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
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
    unsigned long Flags;                        // +0x00 (8: perform action)
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointer PAction;                    // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;            // +0x40
    CBrainOutput();                             // 0x00835620
};
class CCreatureActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual void* Clone() const;                // slot 11 (+0x2c)
    unsigned char _body[0xa4];
};
class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _combat[0x6c];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
    virtual ~CCombatActionBase();                       // 0x0062E900
};
class CCreatureAction_IceTrollIcicleAttack : public CCombatActionBase { public:   // vtable 0x012960C4
    virtual void v00();
    CCreatureAction_IceTrollIcicleAttack(CThingCreatureBase* owner, CThing* target)
        : CCombatActionBase(owner, target, 0x96, CSoundPair()) { }
};
class CCreatureAction_RiseUp : public CCombatActionBase { public:
    long m_riseState;
    CCreatureAction_RiseUp(CThingCreatureBase* creature);   // 0x008BD880
};
class CCreatureAction_PlayCombatAnimation : public CCombatActionBase { public:
    long m_loop;                                // retail sizeof 0x118 (frame)
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* creature, const CCharString& animation, bool loop);   // 0x008B7FF0
};
inline void SetPerformAction(CBrainOutput& output, CCombatActionBase* action)
{
    output.Flags |= 8;
    output.PAction.Reset(action->Clone());
}
class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
    unsigned char _pad04[0x18 - 0x04];
    CThingCreatureBase* PCreature;              // +0x18
    void SetBrainOutput(const CBrainOutput& output);   // 0x0088B8F0
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    bool IsStopping();                          // 0x008FD010
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_CombatPerformActionAndWait(CCombatActionBase* action, bool wait);   // 0x0097DA40
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long flags, float turnSpeed);   // 0x0097D6F0
    long AIAction_DoNothing();                  // 0x0097E200
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_IceTrollIcicleAttack : CAIStateGroupBase {
    CBaseIntelligentPointer PTarget;            // +0x14
    void Update();
};

void CAIStateGroup_IceTrollIcicleAttack::Update()
{
    if (IS_THING_ALIVE(PTarget)) {
        CThing* target = PTarget.GetPItem();
        bool has = target->HasTC(0x36);
        if (has) {
            CTCStealth* stealth = target->GetTC<CTCStealth>(0x36);
            stealth->SetAsInDanger(GetCreature(), 3);
        }
        CTCCreatureModeManager* modes;
        if (GetCreature()->GetInterface(&modes, 0x31) && modes->HasMode(0x1e)) {
            long result = AIAction_PerformActionAndWait(&CCreatureAction_RiseUp(GetCreature()), false);
            if (result == 1) {
                Terminate();
                return;
            }
        }
        if (AIAction_LookAtThing(1, PTarget, 1, 0.01f) == 2 && IS_THING_ALIVE(PTarget) && !IsStopping()) {
            long remaining = 2;
            do {
                CThingCreatureBase* creature = GetCreature();
                CCreatureAction_IceTrollIcicleAttack action(creature, PTarget.GetPItem());
                if (GetCreature()->IsFreeToPerformAction(&action)) {
                    CBrainOutput output;
                    SetPerformAction(output, &action);
                    PBrain->SetBrainOutput(output);
                    --remaining;
                }
                PBrain->Yield();
                if (IsStopping()) {
                    Terminate();
                    return;
                }
            } while (remaining > 0);
            while (GetCreature()->IsPerformingAnyAction()) {
                if (AIAction_DoNothing() == 1) {
                    Terminate();
                    return;
                }
            }
            long result = AIAction_CombatPerformActionAndWait(
                &CCreatureAction_PlayCombatAnimation(GetCreature(), "ICICLE_BOAST", true), false);
            if (result == 1) {
                Terminate();
                return;
            }
        }
    }
    Terminate();
}