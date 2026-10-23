// Grade: DIFFER(1140v1144) -- register allocation: retail keeps &PReceivingThing in ebx, &PItemBeingGiven in ebp, &PCreature in edi and spills the deed-log component (frame 0x18); ours frame 0x14 with ebp/edi/ebx rotated
// CCreatureAction_GiveItemToThing::PerformAction() -- hand the item over: post the gift opinion deed, reject a romantic gift a wife will not take, take the item out of the giver's inventory, play HERO_GIVE_ITEM and post a received-item event to the receiver
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00844BC0, length 1144 bytes.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, LowerBound 0x0040F020 / 0x004365B0 (const copy, the gift lookup),
// CTCGift IsGiftOfType wrappers 0x008FA2F0 / 0x008FA2E0 / 0x008FA300, CTCWife::ReceivedNiceGiftIsOpinionReactionHappening 0x006D0630,
// CThing::GetPosition 0x004C73D0, CTCHeroOpinionDeedLog::PostDeed 0x006C6990, CDefClassBase::GetSubDef<CGiftDef> 0x006D3EF0,
// CTCInventoryItem 0x005D8E20 / 0x005D8E50, CThing::SetInLimbo 0x004C8CF0, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
// CTCSound::PlaySound 0x0066BD00, operator new 0x00BFEA1A, CEventReceivedItem ctor 0x0084AE80, CTCEntityEvents::AddEvent 0x008113C0.
// Layout: retail CCreatureActionBase is 0xa8 bytes (PDB 0xb4), so `this` is modelled locally (as in landed 0x00842F70):
// PCreature +0x08, PReceivingThing +0xa8, PItemBeingGiven +0xb0. Retail CThing: UsingComponents bits +0x20, the component
// map +0x44, PBaseDef +0x70, def index word +0x8c, Dead bit 0 of +0x91.
struct CTCBase;
struct C3DVector;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CDefVirtual { virtual void v00(); virtual void Destroy(); };
struct CRefCounted { long m_count; };
struct CDefBase : CDefVirtual, CRefCounted { };
struct CGiftDef : CDefBase {
    unsigned char _pad08[0x24];
    bool Romantic;                                      // +0x2c
};
struct CDefPointer_CGiftDef {
    CGiftDef* m_pDef;
    CDefPointer_CGiftDef() : m_pDef(0) { }
    __forceinline ~CDefPointer_CGiftDef()
    {
        if (m_pDef) {
            if (--m_pDef->m_count == 0)
                m_pDef->Destroy();
        }
    }
    CGiftDef* operator->() const { return m_pDef; }
};
struct CThingBaseDef : CDefBase {
    bool GetSubDef(CDefPointer_CGiftDef* subDef);       // 0x006D3EF0
};
void* FindDef_GiveItem_0();
#pragma auto_inline(off)
__declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer_CGiftDef* out)
{
    void* def = FindDef_GiveItem_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThingBaseDefPointer {
    CThingBaseDef* m_pDef;
    __forceinline CThingBaseDefPointer(CThingBaseDef* p) : m_pDef(p) { if (m_pDef) ++m_pDef->m_count; }
    __forceinline CThingBaseDefPointer(const CThingBaseDefPointer& o) : m_pDef(o.m_pDef) { if (m_pDef) ++m_pDef->m_count; }
    __forceinline ~CThingBaseDefPointer()
    {
        if (m_pDef) {
            if (--m_pDef->m_count == 0)
                m_pDef->Destroy();
        }
    }
    CThingBaseDef* operator->() const { return m_pDef; }
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    SEntry* LowerBound(const long* type) const;         // 0x004365B0
};
struct CThing {
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CThingBaseDefPointer PBaseDef;                      // +0x70
    unsigned char _pad74[0x8c - 0x74];
    unsigned short DefIndex;                            // +0x8c
    unsigned char _pad8e[3];
    unsigned char Flags91;                              // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    __forceinline bool HasTC(long t) const
    {
        if (m_presentBits[t >> 5] & (1UL << (t & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long t)
    {
        long type = t;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || t < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline T* GetTCConst(long t) const
    {
        long type = t;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || t < it->type) ? map->last : it)->component;
    }
    const C3DVector* GetPosition() const;               // 0x004C73D0
    void SetInLimbo(bool limbo);                        // 0x004C8CF0
};
struct CThingAICreature;
struct CThingCreature : CThing { };

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};

struct CTCGift {
    bool IsGiftType0();                                 // 0x008FA2F0
    bool IsGiftType1();                                 // 0x008FA2E0
    bool IsGiftType2();                                 // 0x008FA300
};
struct CTCWife {
    bool ReceivedNiceGiftIsOpinionReactionHappening(long defIndex);   // 0x006D0630
};
struct CTCHeroOpinionDeedLog {
    long PostDeed(long deed, const C3DVector* pos, CThing* thing, CThingAICreature* creature, long postType,
                  bool a, bool b, void* faction, bool c, long defIndex);   // 0x006C6990
};
struct CTCInventoryItem {
    unsigned char _pad00[0x10];
    void* m_pOwner;                                     // +0x10
    void RemoveFromInventory();                         // 0x005D8E20
    void ReturnToOwner();                               // 0x005D8E50
};
struct CTCSound {
    unsigned long PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
    unsigned long PlaySound(const char* name, long a, long b, long c, long d) { return PlaySound(CCharString(name), a, b, c, d); }
};
struct CEventBase { };
struct CEventReceivedItem : CEventBase {
    unsigned char _pad[0x1c];
    CEventReceivedItem(CThing* giver, CThing* item, bool flag);   // 0x0084AE80
};
struct CTCEntityEvents {
    void AddEvent(CEventBase* e);                       // 0x008113C0
};
struct CThingPlayerVirtual {
    // slot 0x134/4 on the creature
};

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                  // +0x08
    unsigned char _pad10[0xa8 - 0x10];
};

struct CCreatureAction_GiveItemToThing : CCreatureActionBase {
    CBaseIntelligentPointer PReceivingThing;            // +0xa8
    CBaseIntelligentPointer PItemBeingGiven;            // +0xb0
    void PerformAction();
};

struct CCreatureVirtual {
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05(); virtual void s06(); virtual void s07();
    virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11(); virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15();
    virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19(); virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29(); virtual void s30(); virtual void s31();
    virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35(); virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39();
    virtual void s40(); virtual void s41(); virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47();
    virtual void s48(); virtual void s49(); virtual void s50(); virtual void s51(); virtual void s52(); virtual void s53(); virtual void s54(); virtual void s55();
    virtual void s56(); virtual void s57(); virtual void s58(); virtual void s59(); virtual void s60(); virtual void s61(); virtual void s62(); virtual void s63();
    virtual void s64(); virtual void s65(); virtual void s66(); virtual void s67(); virtual void s68(); virtual void s69(); virtual void s70(); virtual void s71();
    virtual void s72(); virtual void s73(); virtual void s74(); virtual void s75(); virtual void s76();
    virtual bool IsPlayer();                            // slot 77 (+0x134)
};

void CCreatureAction_GiveItemToThing::PerformAction()
{
    if (PReceivingThing.GetPItem() && !PReceivingThing.GetPItem()->IsDead()
        && PItemBeingGiven.GetPItem() && !PItemBeingGiven.GetPItem()->IsDead()) {
        bool giveItem = true;
        CThing* creature = PCreature.GetPItem();
        if (creature->HasTC(0x51)) {
            CTCHeroOpinionDeedLog* deedLog = creature->GetTC<CTCHeroOpinionDeedLog>(0x51);
            CThing* item = PItemBeingGiven.GetPItem();
            if (item->HasTC(0x59)) {
                CTCGift* gift = item->GetTCConst<CTCGift>(0x59);
                if (PReceivingThing.GetPItem()->HasTC(0x4f)) {
                    long deed = 0;
                    if (gift->IsGiftType0())
                        deed = 0x3d;
                    else if (gift->IsGiftType1())
                        deed = 0x3c;
                    else if (gift->IsGiftType2())
                        deed = 0x3e;
                    bool rejected = false;
                    CThing* receiver;
                    if (((CCreatureVirtual*)PCreature.GetPItem())->IsPlayer() && (receiver = PReceivingThing.GetPItem())->HasTC(0xd7)) {
                        CTCWife* wife = receiver->GetTC<CTCWife>(0xd7);
                        if ((deed == 0x3d || deed == 0x3c)
                            && !wife->ReceivedNiceGiftIsOpinionReactionHappening(PItemBeingGiven.GetPItem()->DefIndex))
                            rejected = true;
                    }
                    if (!rejected) {
                        CThing* given = PItemBeingGiven.GetPItem();
                        CThingAICreature* receiverCreature = (CThingAICreature*)PReceivingThing.GetPItem();
                        CThing* receiverThing = PReceivingThing.GetPItem();
                        deedLog->PostDeed(deed, PCreature.GetPItem()->GetPosition(), receiverThing,
                                          receiverCreature, 1, false, false, 0, false, given->DefIndex);
                    } else {
                        CDefPointer_CGiftDef giftDef;
                        bool romantic = CThingBaseDefPointer(PItemBeingGiven.GetPItem()->PBaseDef.m_pDef)->GetSubDef(&giftDef) && giftDef->Romantic;
                        if (romantic)
                            giveItem = false;
                    }
                }
            }
        }
        if (giveItem) {
        CThing* given = PItemBeingGiven.GetPItem();
        if (given->HasTC(0x1a)) {
            CTCInventoryItem* inv = given->GetTC<CTCInventoryItem>(0x1a);
            if (inv->m_pOwner)
                inv->RemoveFromInventory();
        }
        PItemBeingGiven.GetPItem()->SetInLimbo(true);
        CThing* speaker = PCreature.GetPItem();
        if (speaker->HasTC(0x10)) {
            CTCSound* sound = speaker->GetTC<CTCSound>(0x10);
            sound->PlaySound(CCharString("HERO_GIVE_ITEM"), 0, 0, 0, -1);
        }
        CTCEntityEvents* events = 0;
        CThing* target = PReceivingThing.GetPItem();
        if (target->HasTC(0x42))
            events = target->GetTC<CTCEntityEvents>(0x42);
        events->AddEvent(new CEventReceivedItem(PCreature.GetPItem(), PItemBeingGiven.GetPItem(), true));
        return;
        }
        {
            CThing* given = PItemBeingGiven.GetPItem();
            if (given->HasTC(0x1a)) {
                CTCInventoryItem* inv = given->GetTC<CTCInventoryItem>(0x1a);
                if (inv->m_pOwner)
                    inv->ReturnToOwner();
            }
        }
        return;
    }
    CThing* given;
    if (PItemBeingGiven.GetPItem() && !PItemBeingGiven.GetPItem()->IsDead() && (given = PItemBeingGiven.GetPItem())->HasTC(0x1a)) {
        CTCInventoryItem* inv = given->GetTC<CTCInventoryItem>(0x1a);
        if (inv->m_pOwner)
            inv->ReturnToOwner();
    }
}
