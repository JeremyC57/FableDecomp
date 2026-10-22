// CTCWife::ReceivedNiceGiftIsOpinionReactionHappening(long objectDefIndex) -- react to a gift: a nice gift before the wedding frame posts the wedding-ring (0x3F, while courting with attitude 12) or 0x4A deed; otherwise a repeat gift inside the marriage def's interval posts deed 0x52, else the gift's receive frame is recorded
// Grade: DIFFER(472v459) -- block order and registers: retail lays the not-nice branch (gift-map find/insert) after the shared post-deed/return block and keeps this in esi, the def index in ebp; ours emits the branches in source order with this in edi; ring store uses bl (constant 1 reused) where retail stores an immediate
// Leaf: ReceivedNiceGiftIsOpinionReactionHappening
// Callees: CTCBase world getter 0x686A70 (world +0x10 = definition manager; byte-identical to GetWorld 0x686A80), CDefinitionManager::GetDef 0x42AF3C,
//   CThingObjectDef GetSubDef 0x6D3EF0, GetWorldFrame 0x49D870, CTCWife::IsCourtingHero 0x6CF890, GetCurrentPlayer 0x49D9D0, GetCharacterThing 0x487DC0,
//   LowerBound 0x40F020, GetPosition 0x4C73D0, CTCHeroOpinionDeedLog::PostDeed 0x6C6990, gift map find 0x5937E1 (label SetMiniMapRegionExitTextOffsetX is wrong), insert_unique 0x4E539F.
struct C3DVector { float X, Y, Z; };
struct CDefBase {
    virtual void v00();
    virtual void DeleteThis() const;                        // slot 1
    mutable long RefCount;                                  // +0x04
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->DeleteThis(); } }
    T* operator->() const { return Object; }
};
struct CGiftDef : CDefBase {
    unsigned char _pad08[0x2c - 0x08];
    bool IsNiceGift;                                        // +0x2c
};
struct CThingObjectDef : CDefBase {
    bool GetGiftDef(CDefPointer<const CGiftDef>* out) const;   // 0x006D3EF0
};
struct CDefinitionManager {
    bool GetObjectDef(long index, CDefPointer<const CThingObjectDef>* out) const;   // 0x0042AF3C
};
struct CWorld { unsigned char _pad00[0x10]; CDefinitionManager* PDefManager; };
struct CHeroMarriageDef : CDefBase {
    unsigned char _pad08[0x48 - 0x08];
    long SecondsBetweenNiceGiftReactions;                   // +0x48
};
struct CBitset { unsigned long Bits[9]; };
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long& type);                   // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    CBitset       m_tcBits;                                 // +0x20
    CInterfaceMap m_tcs;                                    // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char Dead : 1;                                 // +0x91
    const C3DVector& GetPosition() const;                   // 0x004C73D0
    __forceinline bool HasTC(long type) const { return (m_tcBits.Bits[type >> 5] & (1UL << (type & 31))) != 0; }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_tcs;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CThingAICreature;
struct CFaction;
enum EOpinionDeedType { OPINION_DEED_NONE };
enum EOpinionPostDeedType { OPINION_POST_DEED_1 = 1 };
struct CTCHeroOpinionDeedLog {
    long PostDeed(EOpinionDeedType deedType, const C3DVector& position, const CThing* target, CThingAICreature* victim,
                  EOpinionPostDeedType postType, bool b1, bool b2, const CFaction* faction, bool b3, long extra);   // 0x006C6990
};
struct CPlayer { CThing* GetCharacterThing(); };            // 0x00487DC0
struct CWorldPlayers { CPlayer* GetCurrentPlayer(); };      // 0x0049D9D0
long GetWorldFrame();                                       // 0x0049D870
extern long g_FramesPerSecond;                              // 0x01375550
struct SGiftNode {
    long Color;
    SGiftNode* Parent;
    SGiftNode* Left;
    SGiftNode* Right;
    long Key;                                               // +0x10
    long FrameReceived;                                     // +0x14
};
struct SGiftPair {
    long first;
    long second;
    SGiftPair(const long& a, const long& b) : first(a), second(b) {}
};
struct CGiftIterator {
    SGiftNode* _M_node;
    CGiftIterator(const CGiftIterator& other) : _M_node(other._M_node) {}
};
struct SGiftInsertResult { CGiftIterator first; bool second; };
struct CGiftMap {
    SGiftNode* Header;
    long NodeCount;
    long Pad;
    CGiftIterator find(const long& key);                    // 0x005937E1
    SGiftInsertResult insert(const SGiftPair& value);       // 0x004E539F
};
struct CTCWife {
    void* __vftable;
    CThing* ParentThing;                                    // +0x04
    unsigned char _pad08[4];
    CHeroMarriageDef* PHeroMarriageDef;                     // +0x0c
    long OpinionAttitude;                                   // +0x10
    unsigned char _pad14[0x20 - 0x14];
    long FrameGotMarriedToThePlayer;                        // +0x20
    unsigned char _pad24[0x78 - 0x24];
    CGiftMap GiftsReceived;                                 // +0x78
    long FrameLastReceivedNiceGift;                         // +0x84
    unsigned char _pad88[0x91 - 0x88];
    bool ReceivedWeddingRing;                               // +0x91
    CWorld* GetDefWorld() const;                            // 0x00686A70
    CWorldPlayers* GetWorld() const;                        // 0x00686A80
    bool IsCourtingHero() const;                            // 0x006CF890
    bool ReceivedNiceGiftIsOpinionReactionHappening(long objectDefIndex);
    __forceinline void PostDeedToHero(EOpinionDeedType deedType)
    {
        CThing* hero = GetWorld()->GetCurrentPlayer()->GetCharacterThing();
        if (hero && !hero->Dead && hero->HasTC(0x51)) {
            CTCHeroOpinionDeedLog* deedLog = (CTCHeroOpinionDeedLog*)hero->GetTC(0x51);
            deedLog->PostDeed(deedType, hero->GetPosition(), ParentThing, (CThingAICreature*)ParentThing, OPINION_POST_DEED_1, false, false, 0, false, 0);
        }
    }
};

bool CTCWife::ReceivedNiceGiftIsOpinionReactionHappening(long objectDefIndex)
{
    CDefPointer<const CThingObjectDef> objectDef;
    CDefPointer<const CGiftDef> giftDef;
    bool isNice = GetDefWorld()->PDefManager->GetObjectDef(objectDefIndex, &objectDef) && objectDef->GetGiftDef(&giftDef) && giftDef->IsNiceGift;
    bool result = true;
    EOpinionDeedType deedType;
    if (isNice && FrameGotMarriedToThePlayer > GetWorldFrame()) {
        if (IsCourtingHero() && OpinionAttitude == 0xc) {
            ReceivedWeddingRing = true;
            deedType = (EOpinionDeedType)0x3f;
            goto post;
        }
        deedType = (EOpinionDeedType)0x4a;
    } else {
        SGiftNode* it = GiftsReceived.find(objectDefIndex)._M_node;
        if (it == GiftsReceived.Header) {
            GiftsReceived.insert(SGiftPair(objectDefIndex, GetWorldFrame()));
            FrameLastReceivedNiceGift = GetWorldFrame();
            goto done;
        }
        if (GetWorldFrame() - it->FrameReceived > PHeroMarriageDef->SecondsBetweenNiceGiftReactions * g_FramesPerSecond) {
            it->FrameReceived = GetWorldFrame();
            FrameLastReceivedNiceGift = GetWorldFrame();
            goto done;
        }
        deedType = (EOpinionDeedType)0x52;
    }
    result = false;
post:
    PostDeedToHero(deedType);
done:
    return result;
}
