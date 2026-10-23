// CTCCombat::SetStruck(const CAttackInfo&, CStruckInfo&) -- react to being struck: optionally note a pending combo counter, let a blocking TC (0x93) absorb the hit, apply an invulnerable owner's def-based reaction, otherwise (for a hit the attack/struck pair allows from a live attacker) run the held item's block checks, pick and queue the struck reaction action (combo, knock-back or plain struck), set the recovery timer and wake the attacker's combat TC
// Grade: DIFFER(1588v1639) -- first draft of the whole flow (combo check, block TC 0x93, invulnerable reaction, held-item block, three queued struck actions with conditional temporaries, recovery timer); retail keeps struck in ebx / owner in ebp with this spilled, frame 0xd4+ larger (0x164 v 0x14c), and the counted-pointer copies and the reaction-def path are not yet in retail's order
// Leaf: SetStruck
// Retail: Fable.exe (TLC, VC7.1) @ 0x00734C60, 1648 bytes, `ret 8`.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    SEntry* LowerBoundConst(const long* type);          // 0x004365B0
};
struct CRefCounted {
    virtual void v00();
    virtual void Release();                             // slot 1
    long RefCount;                                      // +0x04
};
template <class T> struct CCountedPointer {
    T* P;
    CCountedPointer() : P(0) { }
    CCountedPointer(const CCountedPointer& other) : P(other.P) { if (P) ++P->RefCount; }
    ~CCountedPointer() { if (P && --P->RefCount == 0) P->Release(); }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (other.P) ++other.P->RefCount;
        if (P && --P->RefCount == 0) P->Release();
        P = other.P;
        return *this;
    }
};
struct CReactionDef : CRefCounted {
    unsigned char _pad08[0x14 - 0x08];
    struct STiming { unsigned char _pad[8]; float Duration; }* Timing;   // +0x14
};
struct CControllerBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38();
    virtual bool IsScripted();                          // slot 15 (+0x3c)
};
struct CThing {
    virtual void GetCarriedReaction(CCountedPointer<CReactionDef>* out);   // stand-in; the real call is vtable +0xe8
    unsigned char _pad04[0x10 - 0x04];
    unsigned char Mode;                                 // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned char Flags6c;                              // +0x6c
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    unsigned char _pad92[0x100 - 0x92];
    CControllerBase* Controller;                        // +0x100
    bool IsDead() const { return (m_flags & 1) != 0; }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline T* GetTCConst(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBoundConst(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    bool IsInvulnerable();                              // 0x0085EF00
    void StopActions(long a, bool b);                   // 0x00661EF0
    void Reevaluate();                                  // 0x00662650
    bool AddAction(void* action);                       // 0x006644F0
};
struct CThingRef {
    CThing* P;
    bool IsAlive() const;                               // 0x0040DCE0
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CDefRef { unsigned char _pad[8]; struct { unsigned char _pad[0x68]; long ReactionIndex; }* Def; };
struct CStruckInfo {
    bool HasDirection;                                  // +0x00
    unsigned char _pad01;
    unsigned char Direction[0x1f - 0x02];               // +0x02
    bool NoCombo;                                       // +0x1f
    unsigned char _pad20[0x25 - 0x20];
    bool Handled;                                       // +0x25
    bool Reacted;                                       // +0x26
    unsigned char _pad27;
    bool Stunned;                                       // +0x28
    unsigned char _pad29[0x48 - 0x29];
    CDefRef* AttackDef;                                 // +0x48
    unsigned char _pad4c[0x50 - 0x4c];
    CBaseIntelligentPointer Attacker;                   // +0x50
    CBaseIntelligentPointer Weapon;                     // +0x58
    bool IsBlockable() const;                           // 0x00868950
    bool IsUnblockable() const;                         // 0x00868920
    float GetDamage(CThing* victim) const;              // 0x00868690
    void SetBlocked();                                  // 0x00868A40
    void SetReaction(CCountedPointer<CReactionDef> reaction);   // 0x00868640
    bool IsMelee() const;                               // 0x00868860
    bool IsRanged() const;                              // 0x008688C0
};
struct CAttackInfo {
    bool AllowsMelee() const;                           // 0x00815240
    bool AllowsRanged() const;                          // 0x00815230
};
struct CTCBlock {
    virtual void s00();
    virtual void s04();
    virtual void s08();
    virtual void s0c();
    virtual void s10();
    virtual void s14();
    virtual void s18();
    virtual void s1c();
    virtual void s20();
    virtual void s24();
    virtual void s28();
    virtual void s2c();
    virtual void s30();
    virtual void s34();
    virtual void s38();
    virtual void s3c();
    virtual void s40();
    virtual void s44();
    virtual void s48();
    virtual void s4c();
    virtual void s50();
    virtual void s54();
    virtual void s58();
    virtual void s5c();
    virtual void s60();
    virtual void s64();
    virtual void s68();
    virtual void s6c();
    virtual void SetBlockDirection(const void* direction);   // vtable +0x70
    bool IsBlocking();                                  // 0x0079DCB0
    void AbsorbDamage(float damage);                    // 0x0079E250
    bool TryBlock(CThing* attacker);                    // 0x0079DD80
};
struct CTCHeroStunned { void Cancel(); };               // 0x006D7490
struct CTCCarrying { CThing* GetCarriedThing(); };      // 0x006A1440
struct CTCWeapon {
    bool CanBlock(CThing* owner, CThing* attacker, CThing* weapon);       // 0x005DACF0
    bool IsCounterBlock(CThing* owner, CThing* attacker, CThing* weapon); // 0x005DAD10
    long GetReactionType();                             // 0x005DA7F0
};
struct CComboCounter { bool Check(long a, long b); };   // 0x007332F0
struct CComboEntry { unsigned char _pad[0x14]; long Id; unsigned char _pad18[4]; CComboCounter Counter; };
struct CFrameTimer { long GetFrame(); };                // 0x0049D810
struct CDefManager { void GetDef(long index, CCountedPointer<CReactionDef>* out); };   // 0x0066C340
struct CTCCombatOther { void Wake(bool b); };           // 0x00733E10
struct CActionComboStruck {
    unsigned char _data[0x120];
    CActionComboStruck(CThing* owner, CThing* attacker);    // 0x00739A00
    ~CActionComboStruck();                              // 0x0062E900
};
struct CActionKnockBack {
    unsigned char _data[0x120];
    CActionKnockBack(CThing* owner, CThing* attacker);  // 0x00855C40
    ~CActionKnockBack();                                // 0x0062E900
};
struct CActionStruck {
    unsigned char _data[0x120];
    CActionStruck(CThing* owner, CThing* attacker, long type);   // 0x00855B50
    ~CActionStruck();                                   // 0x0062E900
};
extern long g_FramesPerSecond;                          // 0x01375550

class CTCCombat {
public:
    void* __vftable;
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x24 - 0x08];
    CThing* Owner;                                      // +0x24
    unsigned char _pad28[0x62 - 0x28];
    bool AlwaysReact;                                   // +0x62
    bool ComboEnabled;                                  // +0x63
    long RecoveryTimer;                                 // +0x64
    long StunLevel;                                     // +0x68
    CFrameTimer* GetFrameTimer();                       // 0x00686A80
    CDefManager* GetDefManager();                       // 0x006869D0
    bool FindCombo(CComboEntry** out, long type);       // 0x00733750
    bool CanBeStruck(CStruckInfo& struck);              // 0x007348C0
    void ResetCombo();                                  // 0x007337E0
    void FaceAttacker(CThing* attacker);                // 0x00734050
    bool ChooseReaction(CCountedPointer<CReactionDef>* out, long type);   // 0x00734880
    void SetStruck(const CAttackInfo& attack, CStruckInfo& struck);
};

void CTCCombat::SetStruck(const CAttackInfo& attack, CStruckInfo& struck)
{
    CThing* owner = Owner;
    bool noCombo = struck.NoCombo;
    const void* direction = 0;
    bool comboPending = false;
    CControllerBase* controller = owner->Controller;
    if (struck.HasDirection)
        direction = struck.Direction;
    CThing* attacker = struck.Attacker.GetPItem();
    if (owner->Mode == 1 && ComboEnabled && attacker && !attacker->IsDead() && !noCombo) {
        CComboEntry* combo = 0;
        if (FindCombo(&combo, 5) && combo->Counter.Check(combo->Id, GetFrameTimer()->GetFrame()))
            comboPending = true;
    }
    if (!struck.IsBlockable() && (owner->m_presentBits[4] & 0x80000)) {
        CTCBlock* block = owner->GetTC<CTCBlock>(0x93);
        if (block->IsBlocking()) {
            if (!struck.IsUnblockable())
                struck.Stunned = true;
            if (direction)
                block->SetBlockDirection(direction);
            block->AbsorbDamage(struck.GetDamage(owner));
            if (block->TryBlock(attacker)) {
                struck.SetBlocked();
                struck.Handled = true;
                return;
            }
            struck.Reacted = true;
            return;
        }
    }
    if (owner->IsInvulnerable()) {
        struck.Reacted = true;
        struck.Stunned = true;
        CCountedPointer<CReactionDef> reaction;
        long index = struck.AttackDef->Def->ReactionIndex;
        if (index > 0)
            GetDefManager()->GetDef(index, &reaction);
        struck.SetReaction(reaction);
        return;
    }
    if (!((attack.AllowsMelee() && struck.IsMelee()) || (attack.AllowsRanged() && struck.IsRanged())))
        return;
    if (!attacker || attacker->IsDead() || !CanBeStruck(struck))
        return;
    if (StunLevel <= 6 && (ParentThing->m_presentBits[3] & 0x100))
        ParentThing->GetTC<CTCHeroStunned>(0x68)->Cancel();
    CThing* weapon = struck.Weapon.GetPItem();
    CThingRef attackerRef;
    attackerRef.P = attacker;
    CCountedPointer<CReactionDef> reaction;
    CThing* carried = (owner->m_presentBits[2] & 0x40) ? owner->GetTCConst<CTCCarrying>(0x46)->GetCarriedThing() : 0;
    CCountedPointer<CReactionDef> carriedReaction;
    if (carried && !carried->IsDead()) {
        if (carried->Flags6c & 4) {
            CCountedPointer<CReactionDef> tmp;
            carried->GetCarriedReaction(&tmp);
            carriedReaction = tmp;
        }
        if (!carried->IsDead()) {
            if (!(carried->m_presentBits[2] & 0x100))
                return;
            CTCWeapon* blocker = carried->GetTCConst<CTCWeapon>(0x48);
            if (!blocker->CanBlock(owner, attacker, weapon))
                return;
            if (blocker->IsCounterBlock(owner, attacker, weapon))
                noCombo = true;
            reaction = carriedReaction;
            goto react;
        }
    }
    {
        long index = struck.AttackDef->Def->ReactionIndex;
        CCountedPointer<CReactionDef> def;
        if (index > 0) {
            GetDefManager()->GetDef(index, &def);
            reaction = def;
        }
    }
react:
    if (!attackerRef.IsAlive() || !(attacker->Flags6c & 8))
        return;
    {
        bool stun = true;
        if (controller && controller->IsScripted())
            owner->StopActions(0, true);
        else {
            if (owner->Mode != 2 || AlwaysReact)
                owner->Reevaluate();
            if (noCombo && owner->AddAction(&CActionComboStruck(owner, attacker))) {
                stun = false;
                struck.NoCombo = true;
            } else if (!(comboPending && owner->AddAction(&CActionKnockBack(owner, attacker)))) {
                owner->AddAction(&CActionStruck(owner, attacker, 0));
                long type = 0;
                CThingRef weaponRef;
                weaponRef.P = weapon;
                if (weaponRef.IsAlive() && (weapon->m_presentBits[2] & 0x100))
                    type = weapon->GetTCConst<CTCWeapon>(0x48)->GetReactionType();
                ResetCombo();
                FaceAttacker(attacker);
                CReactionDef* chosen = 0;
                if (ChooseReaction((CCountedPointer<CReactionDef>*)&chosen, type))
                    RecoveryTimer = (long)(g_FramesPerSecond * chosen->Timing->Duration);
                if (attacker->m_presentBits[1] & 0x10000)
                    attacker->GetTC<CTCCombatOther>(0x30)->Wake(true);
            }
        }
        struck.Reacted = true;
        struck.SetReaction(reaction);
        if (stun)
            struck.Stunned = true;
    }
}
