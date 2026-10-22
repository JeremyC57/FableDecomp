// CCreatureAction_BattleChargeMiss::FinishAction() -- base FinishAction; release the creature's special-ability lock (0xA7 CTCSpecialAbilities: AbilityMutexLock and CurrentAbility cleared), re-attach both feet effects (Left / Right particle emitters, interface 0x4B) to the creature, and re-equip the item taken out for the charge (fast unsheathe to the combat pose) when it still lives
// Leaf: ?FinishAction@CCreatureAction_BattleChargeMiss
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CDA30, 521 bytes, virtual FinishAction (`ret`), frame 0xC0: this is spilled to
//   [esp+10h] at entry, ebx is never used, the 0x4B key is an immediate in both feet lookups.
// Callees: CCombatActionBase::FinishAction 0x00859A20, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap
//   LowerBound 0x0040F020 (interfaces 0xA7, 0x4B), CCharString(const char*, long = -1) 0x0099EBF0 ("" 0x0122D70E,
//   "ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE" 0x0126CCE4), ~CCharString 0x0099EAE0, CTCDParticleEmitter::AttachToThing
//   0x006E0BE0, CCreatureAction_UnsheatheItemFromInventory ctor 0x0069F0F0 and its out-of-line destructor 0x005CC091
//   (manifest label "~CAggregate" is propagated), CThingCreatureBase::SetCurrentAction 0x006644F0.
// Shape (rescue R282s4): written against the real API (FableWin.pdb exports): CIntelligentPointer<T> with inline
//   operator-> / operator*, the template IS_THING_ALIVE(const T&), AttachToThing(CThing&, long, const CCharString&, long,
//   float), the unsheathe ctor taking (CThingCreatureBase&, const CCharString&, CThing&, long, long, CCreatureActionBase*)
//   and SetCurrentAction(const CCreatureActionBase&). Three levers decide the allocation:
//   (1) IS_THING_ALIVE tests the dead flag through an inline `bool IsDead() const { return (flags & 1) != 0; }`; the raw
//       `!(flags & 1)` test makes VC7.1 push ebx, keep &PCreature in ebx and promote the 0x4B key to ebp (514 bytes);
//   (2) the action's target and owner go through the inline accessors (`PCreature->`, `*PCreature`): VC7.1 evaluates them
//       before the argument pushes, owner first, as retail does; a direct GetPItem call for either one is evaluated in
//       argument order instead (519 bytes);
//   (3) the GetInterface out-store is the ternary `(it == last || key < it->type ? last : it)->component`; the if-assign
//       spelling brings ebx back (515 bytes).
//   The emitter is one function-scope local shared by both feet blocks: a missing 0x4B interface leaves it unassigned,
//   and retail then reads its frame home ([esp+0Ch]), so the source keeps that shape.
// Layout: local model. engine/CCreatureAction_BattleChargeMiss.h is the Ego_r layout (0x13C, Left +0x12C); TLC's
//   CCombatActionBase is 0x114 bytes (PCreature +0x08, PItemRemoved +0x2C), so Left / Right sit at +0x114 / +0x11C.
//   CThing component bits +0x20, interface map +0x44, dead flag +0x91 bit 0; CTCSpecialAbilities AbilityMutexLock +0x0C,
//   CurrentAbility +0x10.
struct CTCBase;
struct CThing;
struct CBaseObject;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};

struct CInterfaceMap {                                // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};

struct CThingCreatureBase;
struct CCreatureActionBase {                          // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
};
struct CCreatureAction_UnsheatheItemFromInventory : CCreatureActionBase {   // retail sizeof 0xb8
    unsigned char _body[0x10];
    CCreatureAction_UnsheatheItemFromInventory(CThingCreatureBase& owner, const CCharString& animation, CThing& item,
                                               long priority, long timeout, CCreatureActionBase* next);   // 0x0069F0F0
    ~CCreatureAction_UnsheatheItemFromInventory();    // 0x005CC091
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                 // +0x20 component bitset
    CInterfaceMap Components;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                            // +0x91 bit 0: dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* PData;                               // +0x04
    CBaseObject* GetPItem() const;                    // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* operator->() const { return (T*)GetPItem(); }
    T& operator*() const { return *(T*)GetPItem(); }
};
template<class T> inline bool IS_THING_ALIVE(const T& p)
{
    if (p.GetPItem() != 0) {
        if (!p->IsDead())
            return true;
    }
    return false;
}

struct CTCSpecialAbilities {                          // interface 0xA7
    unsigned char _pad00[0x0c];
    bool          AbilityMutexLock;                   // +0x0c
    long          CurrentAbility;                     // +0x10 EHeroAbility
};
struct CTCDParticleEmitter {                          // interface 0x4B
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};

struct CCombatActionBase {                            // retail sizeof 0x114 (CCreatureActionBase prefix)
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;  // +0x08
    unsigned char _pad10[0x2c - 0x10];
    CIntelligentPointer<CThing> PItemRemoved;         // +0x2c
    unsigned char _pad34[0x114 - 0x34];
    void FinishAction();                              // 0x00859A20
};
struct CCreatureAction_BattleChargeMiss : CCombatActionBase {
    CIntelligentPointer<CThing> Left;                 // +0x114 left feet effect
    CIntelligentPointer<CThing> Right;                // +0x11c right feet effect
    void FinishAction();
};

void CCreatureAction_BattleChargeMiss::FinishAction()
{
    CCombatActionBase::FinishAction();
    CTCSpecialAbilities* abilities;
    if (PCreature->GetInterface(&abilities, 0xa7)) {
        abilities->AbilityMutexLock = false;
        abilities->CurrentAbility = 0;
    }
    CTCDParticleEmitter* emitter;
    if (IS_THING_ALIVE(Left)) {
        Left->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(*PCreature, 1, "", 0, 0.0f);
    }
    if (IS_THING_ALIVE(Right)) {
        Right->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(*PCreature, 1, "", 0, 0.0f);
    }
    if (IS_THING_ALIVE(PItemRemoved)) {
        CCharString animation("ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE");
        PCreature->SetCurrentAction(CCreatureAction_UnsheatheItemFromInventory(*PCreature, animation, *PItemRemoved, 100, 250, 0));
    }
}