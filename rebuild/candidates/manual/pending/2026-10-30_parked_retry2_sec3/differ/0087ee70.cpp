// CCombatAction_StabThing::PerformAction() -- the stab lands: a live creature target gets a fired-shot event from the attacker and a StabbedToDeath action; a dead body (type 0xd) plays STABBED_ALREADY_DEAD / STABBED_TO_DEAD from its ability def's animation table through its animating-object component (added if missing); then post the stab deed (0x10, plus 0x3a against a faction) to the attacker's opinion deed log
// Grade: DIFFER(1191v1219) -- 277 difflines (parked 1168/241): __forceinline CDefPointer copy/dtor + GetDef() as 'return CDefPointer(PDef)' (raw-pointer AddRef ctor) give retail's inline def copy (cmp/je/inc [def+4]) as the && temporary with the 1/2/4 condition-flag word; animation/TC names passed as literals to the const CCharString& parameters. Residue: def in esi (retail edi) with the zero register swapped, abilityDef reload. Should move onto engine/CDefPointer.h at landing (needs a raw-pointer ctor + default ctor in the header). Tried: plain forceinline (1184/260), forceinline GetDef with local copy (1190/277), +literals (1222/292)
// Leaf: PerformAction
// Retail 0x0087EE70, 1219 bytes, plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; CVectorMap::LowerBound 0x0040F020 / 0x004365B0; operator new
//   0x00BFEA1A; CEventFiredShot(CThing*) 0x0087FED0; CTCEntityEvents::AddEvent 0x008113C0; CCreatureAction_StabbedToDeath
//   (CThingCreatureBase*) 0x0087E7A0 / ~CCreatureActionBase 0x00693EF0; CThingCreatureBase::SetCurrentAction 0x006644F0;
//   CDefClassBase::GetSubDef<CAbilityDef> 0x0042B0A2; animation table find 0x005DC2E0 / filtered pick 0x005DCC00;
//   CCombatActionBase::GetPWorld 0x00692940; CWorld::GetWorldSeed 0x0049D810; CThing::AddTC 0x004C9D60 /
//   UpdateGraphicAppearance 0x004C7590 / GetPosition 0x004C73D0; CTCAnimatingObject::PlayAnimation 0x007F9010;
//   CTCHeroOpinionDeedLog::PostDeed 0x006C6990; CCharString 0x0099EBF0 / () 0x0099E4B0 / = const char* 0x0099EFE0 / ~
//   0x0099EAE0. Animation filter vtables 0x0122FFDC (default) / 0x0122FFCC (base).
// Layout: local model -- the PDB-generated engine/CCombatAction_StabThing.h is opaque padding; from this function's use:
//   CCombatActionBase {owner +0x08}, stab target +0xa8; CThing {type +0x10, interface bits +0x20, map +0x44, flags +0x6c,
//   def +0x70, dead flag +0x91, animation filter +0xd0}; CAbilityDef animation table +0x34; faction component +0x18.
void* __cdecl operator new(unsigned int size);
struct CCharString {
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const char* text);          // 0x0099EFE0
};
struct C3DVector { float X, Y, Z; };
struct CAnimFilterBase {                               // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() { }
};
struct CAnimFilter : CAnimFilterBase {                 // vtable 0x0122FFDC
    virtual bool Accept(long animation) const;
};
struct SAnimEntry { long Animation; };
struct CAnimationTable {
    bool Find(const CCharString& name, const CAnimFilterBase* filter) const;   // 0x005DC2E0
    const SAnimEntry* Pick(const CCharString& name, unsigned long seed, const CAnimFilterBase* filter, long index) const;   // 0x005DCC00
};
struct CRefCountedDef {
    virtual void v00();
    virtual void Destroy();                            // slot 1
    long RefCount;                                     // +0x04
};
struct CAbilityDef : CRefCountedDef {
    unsigned char   _pad08[0x34 - 0x08];
    CAnimationTable Animations;                        // +0x34
};
template <class T> struct CDefPointer {
    T* Data;
    CDefPointer() : Data(0) { }
    __forceinline CDefPointer(T* p) : Data(p) { if (Data) ++Data->RefCount; }
    __forceinline CDefPointer(const CDefPointer& other) : Data(other.Data)
    {
        if (Data)
            ++Data->RefCount;
    }
    __forceinline ~CDefPointer()
    {
        if (Data && --Data->RefCount == 0)
            Data->Destroy();
    }
    T* operator->() const { return Data; }
};
struct CDefClassBase : CRefCountedDef {
    bool GetSubDef(CDefPointer<CAbilityDef>* out) const;   // 0x0042B0A2
};
struct CWorld {
    unsigned long GetWorldSeed() const;                // 0x0049D810
};
struct CEventBase { };
struct CThing;
struct CEventFiredShot : CEventBase {
    unsigned char _data[0x10];
    CEventFiredShot(CThing* shooter);                  // 0x0087FED0
};
struct CTCBase {
    virtual void v00();
};
struct CTCEntityEvents : CTCBase {
    void AddEvent(CEventBase* event);                  // 0x008113C0
};
struct CTCAnimatingObject : CTCBase {
    void PlayAnimation(long animation, long flags, bool loop);   // 0x007F9010
};
struct CFaction;
struct CTCFactionMember : CTCBase {
    unsigned char _pad04[0x18 - 0x04];
    CFaction*     Faction;                             // +0x18
};
struct CThingAICreature;
struct CTCHeroOpinionDeedLog : CTCBase {
    long PostDeed(long deed, const C3DVector* position, CThing* target, CThingAICreature* witness, long postType,
                  bool a, bool b, CFaction* faction, bool c, long d);   // 0x006C6990
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};
struct CFactionMap {
    struct SEntry { long type; CTCFactionMember* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x004365B0
};
struct CThing {
    virtual void v00();
    unsigned char      _pad04[0x10 - 0x04];
    unsigned char      Type;                           // +0x10
    unsigned char      _pad11[0x20 - 0x11];
    unsigned long      m_presentBits[9];               // +0x20
    CInterfaceMap      m_interfaces;                   // +0x44
    unsigned char      _pad4c[0x6c - 0x4c];
    unsigned char      Flags6c;                        // +0x6c
    unsigned char      _pad6d[0x70 - 0x6d];
    CDefClassBase*     PDef;                           // +0x70
    unsigned char      _pad74[0x91 - 0x74];
    unsigned char      Flags91;                        // +0x91
    unsigned char      _pad92[0xd0 - 0x92];
    const CAnimFilterBase* AnimFilter;                 // +0xd0
    const C3DVector& GetPosition() const;              // 0x004C73D0
    CTCBase* AddTC(const CCharString& name, long a, long b);   // 0x004C9D60
    void UpdateGraphicAppearance(long reason);         // 0x004C7590
    __forceinline CDefPointer<CDefClassBase> GetDef() const { return CDefPointer<CDefClassBase>(PDef); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    __forceinline bool GetFactionMember(CTCFactionMember** ppOut)
    {
        if (m_presentBits[0x49 >> 5] & (1UL << (0x49 & 31))) {
            long type = 0x49;
            CFactionMap* map = (CFactionMap*)&m_interfaces;
            CFactionMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (it == map->last || 0x49 < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CCreatureActionBase {
    unsigned char _data[0xa0];
    ~CCreatureActionBase();                            // 0x00693EF0
};
struct CThingCreatureBase;
struct CCreatureAction_StabbedToDeath : CCreatureActionBase {
    CCreatureAction_StabbedToDeath(CThingCreatureBase* victim);   // 0x0087E7A0
};
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* Data;
    CThingCreatureBase* GetPItem() const;              // 0x00A01B50
};
class CCombatActionBase {
public:
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer Owner;                     // +0x08
    CWorld* GetPWorld() const;                         // 0x00692940
};
class CCombatAction_StabThing : public CCombatActionBase {
public:
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer Target;                    // +0xa8
    void PerformAction();
};

void CCombatAction_StabThing::PerformAction()
{
    if (!Target.GetPItem() || (Target.GetPItem()->Flags91 & 1))
        return;
    if (Target.GetPItem()->Flags6c & 8) {
        CThingCreatureBase* victim = Target.GetPItem();
        CTCEntityEvents* events;
        victim->GetInterface(&events, 0x42);
        events->AddEvent(new CEventFiredShot(Owner.GetPItem()));
        victim->SetCurrentAction(&CCreatureAction_StabbedToDeath(victim));
    } else if (Target.GetPItem()->Type == 0xd) {
        CDefPointer<CAbilityDef> abilityDef;
        bool found = Target.GetPItem()->GetDef()->GetSubDef(&abilityDef) &&
                     abilityDef->Animations.Find("STABBED_TO_DEAD", &CAnimFilter());
        if (found) {
            CCharString name;
            CThing* target = Target.GetPItem();
            CAnimFilter defaultFilter;
            const CAnimFilterBase* filter = &defaultFilter;
            CAnimationTable& animations = abilityDef->Animations;
            if (animations.Find("STABBED_ALREADY_DEAD", target->AnimFilter)) {
                name = "STABBED_ALREADY_DEAD";
                filter = target->AnimFilter;
            } else if (animations.Find("STABBED_TO_DEAD", target->AnimFilter)) {
                name = "STABBED_TO_DEAD";
                filter = target->AnimFilter;
            } else {
                name = animations.Find("STABBED_TO_DEAD", &CAnimFilter()) ? "STABBED_ALREADY_DEAD"
                                                                                       : "STABBED_TO_DEAD";
            }
            const SAnimEntry* entry = animations.Pick(name, GetPWorld()->GetWorldSeed(), filter, -1);
            CTCAnimatingObject* animating;
            if (!Target.GetPItem()->GetInterface(&animating, 0x83))
                animating = (CTCAnimatingObject*)Target.GetPItem()->AddTC("CTCAnimatingObject", 0, 0);
            animating->PlayAnimation(entry->Animation, 0, false);
            Target.GetPItem()->UpdateGraphicAppearance(2);
        }
    }
    CTCHeroOpinionDeedLog* deeds;
    if (Owner.GetPItem()->GetInterface(&deeds, 0x51)) {
        CTCFactionMember* member;
        if (Target.GetPItem()->GetFactionMember(&member) && member->Faction) {
            CFaction* faction = member->Faction;
            deeds->PostDeed(0x10, &Owner.GetPItem()->GetPosition(), Target.GetPItem(), 0, 7, false, false, faction,
                            false, 0);
            deeds->PostDeed(0x3a, &Owner.GetPItem()->GetPosition(), Target.GetPItem(), 0, 7, false, false,
                            member->Faction, true, 0);
            return;
        }
        deeds->PostDeed(0x10, &Owner.GetPItem()->GetPosition(), Target.GetPItem(), 0, 7, false, false, 0, false, 0);
    }
}
