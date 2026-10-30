// CAIStateGroup_Guildmaster::Update() -- while the guildmaster still has living helpers and no shield spell, cast the will shield; then, once enough time has passed since the last will action, teleport toward a better position or (otherwise) gather bodyguards and cycle through three combat actions; with no helpers left, cancel the shield spell; terminate when no longer runnable; the guildmaster def is released on every exit
// Grade: DIFFER(729v732) nd=133 (was 139; frame now 0x364 as retail) -- lever: action sizes to the retail container layout (CCombatAction_WillEnflameInto 0x118 = base 0x114 + Level, engine header; GuildmasterAttack0 no extra 0x10). Tried: scoped boss lookup copy (133). Residue: def CDefPointer zeroed before the GetSubDef push (retail after), key/boss frame words swapped, ecx/eax picks in HasTC. Next: model boss via the shared GetTC form and the CDefPointer from its engine header.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00977E00, length 732 bytes.
// Callees: CVectorMap::LowerBound @ 0x0040F020, CDefClassBase::GetSubDef<CGuildmasterDef> @ 0x00978190, CTCBoss::
// CountLivingHelperCreatures @ 0x007AEEC0, CCombatAction_WillEnflameInto ctor @ 0x00956760, ~CCombatActionBase
// @ 0x0062E900, AIAction_PerformActionAndWait @ 0x0097DB70, IsStillRunnable @ 0x009779A0, Terminate @ 0x008FCF40,
// CAIStateGroupBase::GetPWorld @ 0x008FCFC0, CWorld::GetWorldFrame @ 0x0049D870 (static `__fastcall`),
// CTCPhysicalShieldSpell::CancelMove @ 0x0079E300, GetBetterTeleportPos @ 0x00977820, CCreatureAction_TeleportOut ctor
// @ 0x008BCFB0, FindAndStoreBodyguards @ 0x00977B10, combat-action ctors @ 0x00978510 / 0x009784A0 / 0x00978580,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50.
// Layout: Ego_r.pdb CAIStateGroup_Guildmaster AttackNum +0x14, PEnemy +0x18, TeleportMarkers (retail 12-byte vector)
// +0x28, LastWillFrame +0x34, LastTeleportFrame +0x38.
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CGuildmasterDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x28 - 0x08];
    float TeleportInterval;                     // +0x28 (seconds)
    float WillInterval;                         // +0x2c (seconds)
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CGuildmasterDef>* out);   // 0x00978190
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                      // +0x70
    CDefClassBase* GetDef() const { return m_pDef; }
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
    }
};
struct CThingCreatureBase : CThing { };
struct CTCBoss {                                // interface 0xad
    long CountLivingHelperCreatures() const;    // 0x007AEEC0
};
struct CTCPhysicalShieldSpell {                 // interface 0x93
    void CancelMove();                          // 0x0079E300
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CWorld {
    static long __fastcall GetWorldFrame();     // 0x0049D870
};
extern long g_framesPerSecond;                  // 0x01375550
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
};
struct CCombatActionBase : CCreatureActionBase {
    unsigned char _combat[0x6c];
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCombatAction_WillEnflameInto : CCombatActionBase {
    long Level;                                 // +0x114 (retail sizeof 0x118)
    CCombatAction_WillEnflameInto(CThingCreatureBase* creature, float a, float b);   // 0x00956760
};
struct CCreatureAction_TeleportOut : CCombatActionBase {
    unsigned char _teleport[0x10];
    CCreatureAction_TeleportOut(CThingCreatureBase* creature, CThing* enemy, const C3DVector& pos, bool b);   // 0x008BCFB0
};
struct CCombatAction_GuildmasterAttack0 : CCombatActionBase {
    CCombatAction_GuildmasterAttack0(CThingCreatureBase* creature);   // 0x00978510
};
struct CCombatAction_GuildmasterAttack1 : CCombatActionBase {
    CCombatAction_GuildmasterAttack1(CThingCreatureBase* creature);   // 0x009784A0
};
struct CCombatAction_GuildmasterAttack2 : CCombatActionBase {
    CCombatAction_GuildmasterAttack2(CThingCreatureBase* creature);   // 0x00978580
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_Guildmaster : CAIStateGroupBase {
    long AttackNum;                             // +0x14
    CBaseIntelligentPointer PEnemy;             // +0x18
    CBaseIntelligentPointer PLastTeleportMarker;   // +0x20
    unsigned char TeleportMarkers[0x0c];        // +0x28
    long LastWillFrame;                         // +0x34
    long LastTeleportFrame;                     // +0x38
    bool GetBetterTeleportPos(C3DVector* pos);  // 0x00977820
    bool IsStillRunnable();                     // 0x009779A0
    void FindAndStoreBodyguards();              // 0x00977B10
    void Update();
};

void CAIStateGroup_Guildmaster::Update()
{
    CThingCreatureBase* creature = GetCreature();
    CTCBoss* boss;
    creature->GetInterface(&boss, 0xad);
    CDefPointer<CGuildmasterDef> def;
    creature->GetDef()->GetSubDef(&def);
    if (!GetCreature()->HasTC(0x93) && boss->CountLivingHelperCreatures() > 0)
        AIAction_PerformActionAndWait(&CCombatAction_WillEnflameInto(GetCreature(), 1800.0f, 5.0f), false);
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    long fps = g_framesPerSecond;
    float sinceWill = (float)(GetPWorld()->GetWorldFrame() - LastWillFrame) / fps;
    if (boss->CountLivingHelperCreatures() == 0) {
        if (GetCreature()->HasTC(0x93))
            GetCreature()->GetTC<CTCPhysicalShieldSpell>(0x93)->CancelMove();
    } else if (sinceWill > def->WillInterval) {
        long fps2 = g_framesPerSecond;
        C3DVector pos;
        if ((float)(GetPWorld()->GetWorldFrame() - LastTeleportFrame) / fps2 > def->TeleportInterval
            && GetBetterTeleportPos(&pos)) {
            AIAction_PerformActionAndWait(&CCreatureAction_TeleportOut(GetCreature(), PEnemy.GetPItem(), pos, true), false);
            LastTeleportFrame = GetPWorld()->GetWorldFrame();
        } else {
            FindAndStoreBodyguards();
            switch (AttackNum % 3) {
            case 0:
                AIAction_PerformActionAndWait(&CCombatAction_GuildmasterAttack0(GetCreature()), false);
                break;
            case 1:
                AIAction_PerformActionAndWait(&CCombatAction_GuildmasterAttack1(GetCreature()), false);
                break;
            case 2:
                AIAction_PerformActionAndWait(&CCombatAction_GuildmasterAttack2(GetCreature()), false);
                break;
            }
            ++AttackNum;
            LastWillFrame = GetPWorld()->GetWorldFrame();
        }
    }
    if (!IsStillRunnable())
        Terminate();
}
