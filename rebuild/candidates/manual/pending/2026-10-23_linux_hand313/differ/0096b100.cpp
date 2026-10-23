// CAIStateGroup_WhisperPotionCombat::Update() -- boss combat round: draw a weapon, flag the target's stealth as in danger, jump away when too close, roar at a hostile or nearby player, count hits towards the next boss phase, otherwise answer a drain attack
// Grade: DIFFER(1293v1269) -- ebx/ebp role swap (boss pointer cached in ebp v reloaded from the frame in retail), 3D offset copy scheduling, player 2D length x87 order (fstp placement); single-case switch gives retail's dec eax/jne
struct CTCBase;
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    float Length() const { return (float)sqrt(X * X + Y * Y); }
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
    unsigned char _pad4c[0x70 - 0x4c];
    struct CDefClassBase* PDefClass;            // +0x70 (creature def class)
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
    }
};
struct CDefClassBase;
struct CBossPhaseDef {
    virtual void v00();
    virtual void DeleteThis();
    long RefCount;                              // +0x04
    unsigned char _pad08[0x40 - 0x08];
    long HitsPerPhase;                          // +0x40
};
struct CDefClassBase {
    void GetSubDef(CBossPhaseDef** out);        // 0x00956590
};
#pragma auto_inline(off)
void CDefClassBase::GetSubDef(CBossPhaseDef** out)
{
    *out = 0;
}
#pragma auto_inline(on)
struct CCombatTypeDef {
    unsigned char _pad[0x6c];
    float AttackRange;                          // +0x6C
};
struct CThingCreatureBase : CThing {
    void UnsheatheMeleeWeapon();                // 0x006649C0
    const CCombatTypeDef* GetCombatTypeDef();   // 0x00666D10
};
bool __fastcall IsThingCarryingWeapon(CThingCreatureBase* creature);   // 0x006A1540
bool __fastcall IsAbleToBeAttacked(CThing* thing);                     // 0x0085FB30
bool __fastcall AreThingsEnemies(CThing* a, CThing* b);                // 0x0076D2F0
bool __fastcall IS_THING_ALIVE(CThing* const& thing);                  // 0x005C05C0
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerCreature& p);   // 0x006961F0
struct CTCStealth {
    void SetAsInDanger(CThingCreatureBase* creature, long level);   // 0x00747B80
};
struct CTCBoss {
    unsigned char _pad[0x10];
    long GenericCounter;                        // +0x10
    long GetPhase();                            // 0x007AED10
    void SetNextBossPhase(long phase, bool now);   // 0x007AEDA0
};
struct CCombatActionBase {
    unsigned char _data[0x114];
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCreatureAction_DragonRoar : CCombatActionBase {
    CCreatureAction_DragonRoar(CThingCreatureBase* creature, CThing* target);   // 0x009566C0
};
struct CCombatAction_ResponseToDrainFar : CCombatActionBase {
    CCombatAction_ResponseToDrainFar(CThingCreatureBase* creature, CThing* target);   // 0x00906B00
};
struct CCombatAction_ResponseToDrainNear : CCombatActionBase {
    CCombatAction_ResponseToDrainNear(CThingCreatureBase* creature, CThing* target);  // 0x00906AC0
};
struct CCharacter;
struct CPlayer {
    CThingCreatureBase* GetCharacter();         // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
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
    void Terminate();                           // 0x008FCF40
    CPlayerManager* GetPlayerManager();         // 0x008FCF90
    bool IsStopping();                          // 0x008FD010
    long AIAction_PerformActionAndWait(const CCombatActionBase& action, bool b);   // 0x0097DB70
};
struct CAIStateGroup_WhisperPotionCombat : CAIStateGroupBase {
    CIntelligentPointerCreature PTarget;        // +0x14
    unsigned char TrapThings[0x0c];             // +0x1c
    long FrameLastRun;                          // +0x28
    C3DVector JumpToPos;                        // +0x2c
    bool FindValidCombatPositions(C3DVector* pos);   // 0x0096A3C0
    void DisappearJump();                       // 0x0096AF60
    virtual void Update();
};

void CAIStateGroup_WhisperPotionCombat::Update()
{
    if (!IsThingCarryingWeapon(GetCreature()))
        GetCreature()->UnsheatheMeleeWeapon();
    long result = 0;
    CTCBoss* boss;
    GetCreature()->GetInterface(&boss, 0xad);
    if (!PTarget.GetPItem() || (PTarget.GetPItem()->m_flags91 & 1) || IsStopping()) {
        Terminate();
        return;
    }
    {
        CThing* target = PTarget.GetPItem();
        if (target->m_presentBits[1] & 0x400000) {
            long type = 0x36;
            CInterfaceMap* map = &target->m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            ((CTCStealth*)((it == map->last || 0x36 < it->type) ? map->last : it)->component)->SetAsInDanger(GetCreature(), 3);
        }
    }
    {
        CThingCreatureBase* creature = GetCreature();
        const C3DVector& targetPos = PTarget.GetPItem()->GetPosition();
        const C3DVector& myPos = creature->GetPosition();
        C2DVector offset(myPos.X - targetPos.X, myPos.Y - targetPos.Y);
        bool canJump = true;
        switch (boss->GetPhase()) {
        case 1:
            canJump = FindValidCombatPositions(&JumpToPos);
            break;
        }
        if (canJump) {
            float dist = (float)sqrt(offset.Y * offset.Y + offset.X * offset.X);
            if (dist < GetCreature()->GetCombatTypeDef()->AttackRange) {
                DisappearJump();
                if (!IS_THING_ALIVE(PTarget) || IsStopping()) {
                    Terminate();
                    return;
                }
            }
        }
    }
    if (!PTarget.GetPItem() || (PTarget.GetPItem()->m_flags91 & 1)) {
        Terminate();
        return;
    }
    if (IsAbleToBeAttacked(PTarget.GetPItem())) {
        {
            CThingCreatureBase* creature = GetCreature();
            const C3DVector& targetPos = PTarget.GetPItem()->GetPosition();
            C3DVector offset;
            offset = creature->GetPosition() - targetPos;
            float dist = (float)sqrt(offset.Y * offset.Y + offset.X * offset.X);
            if (!(dist > GetCreature()->GetCombatTypeDef()->AttackRange)) {
                Terminate();
                return;
            }
        }
        CThingCreatureBase* player = GetPlayerManager()->GetMainPlayer()->GetCharacter();
        if (IS_THING_ALIVE(player)) {
            bool roar;
            if (player == PTarget.GetPItem()) {
                roar = true;
            } else {
                const C3DVector& targetPos = PTarget.GetPItem()->GetPosition();
                const C3DVector& playerPos = player->GetPosition();
                C2DVector d(playerPos.X - targetPos.X, playerPos.Y - targetPos.Y);
                roar = d.Length() > 3.5f || AreThingsEnemies(player, GetCreature());
            }
            if (roar)
                result = AIAction_PerformActionAndWait(CCreatureAction_DragonRoar(GetCreature(), PTarget.GetPItem()), false);
        }
        if (!IS_THING_ALIVE(PTarget) || IsStopping() || result != 2) {
            Terminate();
            return;
        }
        switch (boss->GetPhase()) {
        case 1: {
            CBossPhaseDef* def = 0;
            GetCreature()->PDefClass->GetSubDef(&def);
            if (++boss->GenericCounter >= def->HitsPerPhase)
                boss->SetNextBossPhase(boss->GetPhase() + 1, false);
            if (--def->RefCount == 0)
                def->DeleteThis();
            break;
        }
        }
        {
            CThingCreatureBase* creature = GetCreature();
            const C3DVector& targetPos = PTarget.GetPItem()->GetPosition();
            C3DVector offset;
            offset = creature->GetPosition() - targetPos;
            float dist = (float)sqrt(offset.Y * offset.Y + offset.X * offset.X);
            if (dist < GetCreature()->GetCombatTypeDef()->AttackRange && FindValidCombatPositions(&JumpToPos)) {
                DisappearJump();
                if (!IS_THING_ALIVE(PTarget) || IsStopping()) {
                    Terminate();
                    return;
                }
            }
        }
        Terminate();
        return;
    }
    CThingCreatureBase* creature = GetCreature();
    CThing* target = PTarget.GetPItem();
    const C3DVector& myPos = creature->GetPosition();
    const C3DVector& targetPos = target->GetPosition();
    float dx = targetPos.X - myPos.X;
    float dy = targetPos.Y - myPos.Y;
    float dist = (float)sqrt(dy * dy + dx * dx);
    if (dist > GetCreature()->GetCombatTypeDef()->AttackRange)
        AIAction_PerformActionAndWait(CCombatAction_ResponseToDrainFar(GetCreature(), PTarget.GetPItem()), false);
    else
        AIAction_PerformActionAndWait(CCombatAction_ResponseToDrainNear(GetCreature(), PTarget.GetPItem()), false);
    Terminate();
}
