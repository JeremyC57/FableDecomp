// CCreatureAction_GiveMoneyToThing::PerformAction() -- take up to Amount gold from the creature's hero stats, post a received-money event to the receiver and, for a qualifying villager receiver, post the opinion deed (skipped for a married wife whose gift reaction is not running)
// Grade: DIFFER(554v547) -- &PReceivingThing lands in ebp (retail ebx) so VC caches the constant 4 in ebx (key store/cmp/test), and the stats pointer goes through eax (retail stays in ecx)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00845040, length 547 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020,
//          CTCHeroStats::AddMoney(long, bool) @ 0x0057B327, operator new @ 0x00BFEA1A,
//          NEntityEvents::CEventReceivedMoney::CEventReceivedMoney(CThing*, long) @ 0x0084AE00,
//          CTCEntityEvents::AddEvent @ 0x008113C0, CTCWife::IsMarriedToThePlayer @ 0x006CEC80,
//          CTCWife::ReceivedNiceGiftIsOpinionReactionHappening(long) @ 0x006D0630, CThing::GetPosition @ 0x004C73D0,
//          CTCHeroOpinionDeedLog::PostDeed @ 0x006C6990; creature virtual slot 0x134.
// Shape: the amount is std::min(gold, Amount) through a reference pick; the entity-events lookup ignores the
//   presence bit's result (the not-present path reads the never-written out variable).
// Layout: CCreatureActionBase (retail 0xa8) with PCreature at +8; PReceivingThing +0xa8 and Amount +0xb0 as the landed
//   constructor 0x007ED450 shows (no engine header for this class).
struct CTCBase;
struct C3DVector;
struct CFaction;
class CThingAICreature;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};

struct CThing {
    virtual void v00();
    unsigned char _pad04[0x10 - 0x04];
    unsigned char m_thingType;                         // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                           // +0x91 bit 0: dead
    virtual bool IsPlayerControlled();                 // slot 0x134 / 4 (name descriptive)
    const C3DVector& GetPosition() const;              // 0x004C73D0
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline CTCBase* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CThingCreatureBase : CThing { };

struct CTCHeroStats {
    unsigned char _pad[0x3c];
    long m_gold;                                       // +0x3c
    void AddMoney(long amount, bool notify);           // 0x0057B327
};
struct CEventBase { };
struct CEventReceivedMoney : CEventBase {
    unsigned char _pad[0x14];
    CEventReceivedMoney(CThing* giver, long amount);   // 0x0084AE00
};
struct CTCEntityEvents {
    void AddEvent(CEventBase* event);                  // 0x008113C0
};
struct CTCWife {
    bool IsMarriedToThePlayer();                       // 0x006CEC80
    bool ReceivedNiceGiftIsOpinionReactionHappening(long a);   // 0x006D0630
};
struct CTCHeroOpinionDeedLog {
    long PostDeed(long deedType, const C3DVector& position, const CThing* target, CThingAICreature* victim,
                  long postType, bool b1, bool b2, const CFaction* faction, bool b3, long extra);   // 0x006C6990
};

struct CBaseIntelligentPointer {
    void*               __vftable;
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const;              // 0x00A01B50
};

struct CCreatureActionBase {
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
};

struct CCreatureAction_GiveMoneyToThing : CCreatureActionBase {
    CBaseIntelligentPointer PReceivingThing;           // +0xa8
    long Amount;                                       // +0xb0
    virtual void PerformAction();
};

template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }

void CCreatureAction_GiveMoneyToThing::PerformAction()
{
    if (PReceivingThing.GetPItem() != 0 && !(PReceivingThing.GetPItem()->m_flags91 & 1)) {
        CThingCreatureBase* creature = PCreature.GetPItem();
        if (creature->HasTC(4)) {
            CTCHeroStats* stats = (CTCHeroStats*)creature->FindTC(4);
            long gold = stats->m_gold;
            long amount = Min(gold, Amount);
            stats->AddMoney(-amount, true);
            CTCEntityEvents* events;
            PReceivingThing.GetPItem()->GetInterface(&events, 0x42);
            events->AddEvent(new CEventReceivedMoney(PCreature.GetPItem(), amount));
            CThingCreatureBase* giver = PCreature.GetPItem();
            if (giver->HasTC(0x51)) {
                CTCHeroOpinionDeedLog* deeds = (CTCHeroOpinionDeedLog*)giver->FindTC(0x51);
                if (PReceivingThing.GetPItem()->m_thingType == 1 && !PReceivingThing.GetPItem()->HasTC(0xb4)
                    && PReceivingThing.GetPItem()->HasTC(0x4f)) {
                    if (PCreature.GetPItem()->IsPlayerControlled() && PReceivingThing.GetPItem()->HasTC(0xd7)) {
                        CTCWife* wife = (CTCWife*)PReceivingThing.GetPItem()->FindTC(0xd7);
                        if (wife->IsMarriedToThePlayer() && !wife->ReceivedNiceGiftIsOpinionReactionHappening(-1))
                            return;
                    }
                    CThing* receiver = PReceivingThing.GetPItem();
                    CThing* from = PCreature.GetPItem();
                    deeds->PostDeed(0x3d, from->GetPosition(), receiver, (CThingAICreature*)PReceivingThing.GetPItem(), 1, false, false, 0, false, 0);
                }
            }
        }
    }
}
