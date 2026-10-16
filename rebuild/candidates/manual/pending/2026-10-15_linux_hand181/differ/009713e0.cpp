// CAIStateGroup_CreatureUseCoverBase::DuckBehindCover(long param) -- with a live target: say the combat "duck" speech, then run a projectile-crouch action against the target and report whether it completed (result 2)
// Grade: DIFFER(190v190) -- register allocation only: ebx/ebp swapped for &PTargetThing and the speech id, eax/ecx/edx rotation around the def release and the action args, `mov ecx,[eax+4Ch] ; push ; push ; add ecx,38h` vs ours `mov eax ; lea ecx,[eax+38h]`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009713E0, length 190 bytes (`ret 4`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (IS_THING_ALIVE inlined on PTargetThing +0x14, then the target),
// CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (twice; manifest "CPlayer::GetCurrentMode" is propagated),
// CombatDialogueManager::SaySpeech(CThing* creature, long speech) @ 0x008AFC40 (result dropped; the manager at +0x38
// of the world's +0x4c object), CCreatureAction_ProjectileCrouch ctor (creature, target, param) @ 0x008B8260,
// CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase*, bool) @ 0x0097DB70,
// CCombatActionBase::~CCombatActionBase @ 0x0062E900 (manifest "~CTCLightningOrb" is propagated).
// Shape: the speech id is read through a by-value CDefPointer copy of the combat object's def (+0x44, field +0x50):
// add-ref with a null test, release through the def's inline Release() with none. The action is a temporary passed by
// address with `false` pushed before its constructor arguments (landed 0x00956460 form); frame 0x124 = the action.
struct CThing;

struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer   // inline ctor stores the template's vtable after the base ctor
{
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    T* Get() const { return (T*)GetPItem(); }
};

struct CTCBase;

struct CInterfaceMap                                // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                  // +0x00
    SEntry* last;                                   // +0x04
    SEntry* LowerBound(const long* type);           // 0x004365B0
};

struct CThing
{
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad_0x4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead

    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CInterfaceMap::SEntry* FindEntry(long type)    // the entry; `->component` read at the call site
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        CInterfaceMap::SEntry* sel;
        if (it == end)
            sel = end;
        else if (it->type <= type)
            sel = it;
        else
            sel = end;
        return sel;
    }
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CAIBrain
{
    unsigned char _pad0[0x18];
    CThing* Creature;                               // +0x18
};

struct CDefBase                                     // counted def: vptr, reference count at +4
{
    virtual void v00();
    virtual void Destroy();                         // slot 1 (+0x04)
    long RefCount;                                  // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};

template <class T>
struct CDefPointer
{
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};

struct CCombatDef : CDefBase
{
    unsigned char _pad08[0x50 - 0x08];
    long DuckSpeech;                                // +0x50
};

struct CombatDialogueManager
{
    bool SaySpeech(CThing* creature, long speech);  // 0x008AFC40
};

struct CCombatManager                               // the world's +0x4c object
{
    unsigned char _pad00[0x38];
    CombatDialogueManager DialogueManager;          // +0x38
    unsigned char _pad39[0x44 - 0x39];
    CDefPointer<CCombatDef> PDef;                   // +0x44
    CDefPointer<CCombatDef> GetDef() const { return PDef; }
};

struct CWorld
{
    unsigned char _pad00[0x4c];
    CCombatManager* PCombatManager;                 // +0x4c
};

struct CCreatureActionBase
{
    virtual void v00();
    unsigned char _body[0xa4];                      // +0x04..+0xa7
};

struct CCombatActionBase : CCreatureActionBase
{
    unsigned char _combat[0x6c];                    // +0xa8..+0x113
    ~CCombatActionBase();                           // 0x0062E900
};

struct CCreatureAction_ProjectileCrouch : CCombatActionBase
{
    unsigned char _own[0x10];                       // +0x114..+0x123
    CCreatureAction_ProjectileCrouch(CThing* creature, CThing* target, long param);   // 0x008B8260
};

struct CAIStateGroup_CreatureUseCoverBase
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CIntelligentPointer<const CThing> PTargetThing; // +0x14
    CWorld* GetPWorld();                            // 0x008FCFC0
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    bool DuckBehindCover(long param);
};

__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0)
    {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

bool CAIStateGroup_CreatureUseCoverBase::DuckBehindCover(long param)
{
    if (!IS_THING_ALIVE(PTargetThing))
        return false;
    long speech = GetPWorld()->PCombatManager->GetDef()->DuckSpeech;
    CThing* creature = PBrain->Creature;
    CombatDialogueManager& dialogue = GetPWorld()->PCombatManager->DialogueManager;
    dialogue.SaySpeech(creature, speech);
    CThing* actor = PBrain->Creature;
    long result = AIAction_PerformActionAndWait(
        &CCreatureAction_ProjectileCrouch(actor, PTargetThing.GetPItem(), param), false);
    return result == 2 ? true : false;
}
