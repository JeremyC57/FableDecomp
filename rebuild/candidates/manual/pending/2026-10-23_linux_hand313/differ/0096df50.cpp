// CAIStateGroup_ScreamerPursueTarget::Update() -- pick the hero as the pursuit target, warn their stealth component, then chase them; once a sighting message arrives schedule a scream-into-aggressive action and stop when close enough or a step fails
// Grade: DIFFER(1159v1150) -- first pass (3 grades): structure decoded (hero pushed into a local STLport vector, sorted by distance, stealth warned, chase loop with the sighting-message scream timer); open: the early-exit path should keep its own inlined vector-free epilogue with first/last in ebp/edi, the ScreamFrame==0 block should fall through first, the GetPItem-before-virtual-GetTarget order, frame 0x158 v 0x168
#include <stdlib.h>
#include <new>
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        bool found = (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) != 0;
        if (found) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return found;
    }
};
struct CCreatureCombatTypeDef {
    unsigned char _pad[0x6c];
    float AggressiveRange;                      // +0x6c
    unsigned char _pad70[0x10];
    float PursueProximity;                      // +0x80
};
struct CThingCreatureBase : CThing {
    const CCreatureCombatTypeDef* GetCombatTypeDef() const;   // 0x00666D10
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct SScalarTag { SScalarTag() { } };
struct CThingArray {                            // STLport vector<CThing*>
    CThing** first;
    CThing** last;
    CThing** end;
    CThingArray() : first(0), last(0), end(0) { }
    ~CThingArray() { if (first) free(first); }
    void InsertGrow(CThing** where, CThing* const& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00436AC0
    void push_back(CThing* const& value)
    {
        if (last != end) {
            new (last) CThing*(value);
            ++last;
        } else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
void GrowInsert_00436ac0(void* v, void* where, const void* value);
#pragma auto_inline(off)
void CThingArray::InsertGrow(CThing** where, CThing* const& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    GrowInsert_00436ac0(this, where, &value);
}
#pragma auto_inline(on)
void __fastcall SortThingsByDistance(CThing** first, CThing** last, const C3DVector& from);   // 0x0096E4A0
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerPlayer : CBaseIntelligentPointer {       // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerPlayer(CThing* item) { SetPItem(item); }
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing(CThing* item) { SetPItem(item); }
};
struct CTCStealth {                             // interface 0x36
    void SetAsInDanger(CThing* from, long level);   // 0x00747B80
};
struct CTCModes {                               // interface 0x31
    bool HasMode(long mode);                    // 0x0073A8A0
};
struct CTCTargeting {                           // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b();
    virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v1a(); virtual void v1b(); virtual void v1c();
    virtual CThing* GetTarget();                // slot 0x74
};
struct CMessageEvent;
struct CMessageEventFilter_IsOfType {
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) { }
};
struct CMessageEventFilter_IsOriginatedByThing {
    CThing* Thing;
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);   // 0x006AF290
};
template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
struct CMessageEventManager {
    const CMessageEvent* FindMostRecentMessage(const CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing>& filter);   // 0x008D46D0
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* PMessages;            // +0x60
    unsigned long GetWorldFrame();              // 0x0049D870
    unsigned long* GetPRandomSeed();            // 0x0049D810
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[0x110];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_ScreamerIntoAggressive : CCombatActionBase {   // vtable 0x01296F14
    virtual void v00();
    CCreatureAction_ScreamerIntoAggressive(CThingCreatureBase* owner, CThing* target)
        : CCombatActionBase(owner, target, 0x96, CSoundPair()) { }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CPlayer {
    CThing* GetCharacter();                     // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    void Terminate();                           // 0x008FCF40
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
    CWorld* GetPWorld();                        // 0x008FCFC0
    bool IsStopping();                          // 0x008FD010
    long AIAction_PerformActionAndWait(CCombatActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing thing, float proximity, bool* arrived,
                                       long movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
};
struct CAIStateGroup_ScreamerPursueTarget : CAIStateGroupBase {
    CBaseIntelligentPointer PTarget;            // +0x14
    unsigned long ScreamFrame;                  // +0x1c
    virtual void Update();
};

void CAIStateGroup_ScreamerPursueTarget::Update()
{
    CThingCreatureBase* creature = GetCreature();
    CTCModes* modes;
    creature->GetInterface(&modes, 0x31);
    CTCTargeting* targeting;
    creature->GetInterface(&targeting, 8);
    CThingArray targets;
    CThing* hero = GetPPlayerManager()->GetMainPlayer()->GetCharacter();
    targets.InsertGrow(targets.last, hero, SScalarTag(), 1, true);
    CThing** last = targets.last;
    CThing** first = targets.first;
    if (first == last) {
        Terminate();
        return;
    }
    C3DVector from = GetCreature()->GetPosition();
    SortThingsByDistance(first, last, from);
    PTarget.SetPItem(CIntelligentPointerPlayer(*first).GetPItem());
    CTCStealth* stealth;
    if (PTarget.GetPItem()->GetInterface(&stealth, 0x36))
        stealth->SetAsInDanger(GetCreature(), 2);
    ScreamFrame = 0;
    bool close = false;
    for (;;) {
        if (IsStopping())
            break;
        if (!PTarget.GetPItem() || (PTarget.GetPItem()->m_flags91 & 1))
            break;
        if (PTarget.GetPItem() == targeting->GetTarget()) {
            CThingCreatureBase* me = GetCreature();
            CThing* target = PTarget.GetPItem();
            const C3DVector& a = me->GetPosition();
            const C3DVector& b = target->GetPosition();
            float dz = b.Z - a.Z;
            float dy = b.Y - a.Y;
            float dx = b.X - a.X;
            float range = GetCreature()->GetCombatTypeDef()->AggressiveRange;
            if (dx * dx + dy * dy + dz * dz < range * range)
                close = true;
        }
        if (ScreamFrame != 0) {
            if (GetPWorld()->GetWorldFrame() >= ScreamFrame) {
                bool calm = !modes->HasMode(0x44);
                if (calm) {
                    long result = AIAction_PerformActionAndWait(&CCreatureAction_ScreamerIntoAggressive(GetCreature(), PTarget.GetPItem()), false);
                    if (result != 2)
                        break;
                    if (close)
                        break;
                    continue;
                }
            }
        } else if (!modes->HasMode(0x44)) {
            if (GetPWorld()->PMessages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0xc) && CMessageEventFilter_IsOriginatedByThing(PTarget.GetPItem()))) {
                CThingCreatureBase* me = GetCreature();
                CThing* target = PTarget.GetPItem();
                const C3DVector& a = me->GetPosition();
                const C3DVector& b = target->GetPosition();
                float dy = b.Y - a.Y;
                float dx = b.X - a.X;
                float dist = (float)sqrt(dx * dx + dy * dy);
                ScreamFrame = GetPWorld()->GetWorldFrame() + 5;
                unsigned long* seed = GetPWorld()->GetPRandomSeed();
                *seed = *seed * 0x24A1U + 0x24DFU;
                *seed = GFROR13(*seed);
                ScreamFrame += *seed % 20 - (long)(dist * -3.0f);
            }
        }
        bool arrived;
        if (AIAction_MoveToThingProximity(2, PTarget.GetPItem(), GetCreature()->GetCombatTypeDef()->PursueProximity, &arrived,
                3, true, true, true, true, 0.0f) != 4)
            break;
        if (close)
            break;
    }
    Terminate();
}
