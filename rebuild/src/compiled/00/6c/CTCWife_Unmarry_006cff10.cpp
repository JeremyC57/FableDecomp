// CTCWife::Unmarry(bool keepHouseUnrented) -- for a wife married to the player: hand her home back (the house component's occupant cleared, re-rented and optionally un-rented), hide the attraction icon, forget the wedding frame, tell the living hero's stats she is no longer married, and ask for a new brain
// Leaf: ?Unmarry@CTCWife
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CFF10, 269 bytes (+3 int3), `ret 4`.
// Callees: CWorld::GetWorldFrame() @ 0x0049D870 (static), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice, on the
//   creature's HomeBuilding), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (house component type 0xD5 =
//   bit 21 of bitset word +0x38; hero stats type 4 = bit 4 of +0x20), the house component's member @ 0x006D3C40 (manifest
//   "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated; called with 0) and CTCBuyableHouse::SetRented(bool)
//   @ 0x006C1150, CTCWife::SetAttractionIcon(bool, float, unsigned char) @ 0x006CFC90, CPlayerManager::GetMainPlayer
//   @ 0x00449970, CPlayer::GetHeroThing @ 0x00487DC0, CTCHeroStats::SetMarriageState(CThingAICreature*, EMarriageState)
//   @ 0x0057BC6E. Global g_pMainGameComponent 0x013B86A0 (PlayerManager +0x1C).
// Layout: CTCWife ParentThing +0x04 (CTCBase; engine/CTCWife.h leaves it as padding, so the landed CTCWife::Died
//   0x006D0020 local model is used), FrameGotMarriedToThePlayer +0x20, NeedsToChangeBrain +0x26; CThingAICreature
//   HomeBuilding +0x168 (the landed ctor 0x00831F80).
// Shape: the inline IsMarriedToThePlayer guard (`cmp [ebp+20h],eax ; jg`); the house component is an out-parameter lookup
//   into a pointer initialised to 0 and used even when the building has no such component; the hero stats lookup's out
//   pointer is uninitialised (it shares the key word) and is used unconditionally, as retail does.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
class CThingAICreature : public CThing {
public:
    unsigned char _pad95[0x168 - 0x95];
    CBaseIntelligentPointer HomeBuilding;       // +0x168 CIntelligentPointer<CThingBuilding>
};
class CPlayer { public:
    CThing* GetHeroThing() const;               // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                   // 0x00449970
};
class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870
};
class CMainGameComponent { public:
    unsigned char _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;              // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;  // 0x013B86A0
class CTCHeroStats { public:
    void SetMarriageState(CThingAICreature* spouse, long state);   // 0x0057BC6E
};
class CTCBuyableHouse { public:                        // interface type 0xD5
    void SetOccupant(CThing* occupant);         // 0x006D3C40 (name not recovered)
    void SetRented(bool rented);                // 0x006C1150
};
class CTCWife { public:
    virtual void v00();
    CThingAICreature* ParentThing;              // +0x04
    unsigned char _pad_0x08[0x20 - 0x08];
    long FrameGotMarriedToThePlayer;            // +0x20
    bool DivorcedHero;                          // +0x24
    bool JustMarried;                           // +0x25
    bool NeedsToChangeBrain;                    // +0x26
    bool IsMarriedToThePlayer() const { return FrameGotMarriedToThePlayer <= CWorld::GetWorldFrame(); }
    void SetAttractionIcon(bool show, float scale, unsigned char alpha);   // 0x006CFC90
    void Unmarry(bool keepHouseUnrented);
};

void CTCWife::Unmarry(bool keepHouseUnrented)
{
    if (IsMarriedToThePlayer()) {
        CThingAICreature* creature = ParentThing;
        if (creature->HomeBuilding.GetPItem()) {
            CTCBuyableHouse* house = 0;
            creature->HomeBuilding.GetPItem()->GetInterface(&house, 0xd5);
            house->SetOccupant(0);
            house->SetRented(true);
            if (keepHouseUnrented)
                house->SetRented(false);
        }
        SetAttractionIcon(false, 1.0f, 255);
        FrameGotMarriedToThePlayer = 0x7fffffff;
        CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
        if (hero && !(hero->Flags91 & 1)) {
            CTCHeroStats* stats;
            hero->GetInterface(&stats, 4);
            stats->SetMarriageState(creature, 0);
        }
        NeedsToChangeBrain = true;
    }
}