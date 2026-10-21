// CTCWife::CTCWife(CThing* thing) -- CTCBase(thing), the wife vtable, every marriage/courting flag false, the "never" frames at LONG_MAX, no sex-cutscene sample, gift price 500, empty gift and seen-clothing trees, no appearance changes noticed
// Leaf: ??0CTCWife
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CF040, 394 bytes (`ret 4`); CTCWife vtable 0x0125ED5C (the catalog label
//          "CTCOpinionSource::CTCOpinionSource" is propagated: that constructor is 0x00771360).
// Callees: CTCBase::CTCBase(CThing*) 0x00686800; malloc 0x00BFEA0E (the two tree header nodes, 0x18 and 0x14 bytes).
// Global: the invalid sound-sample id 0x0129D25C.
// Layout: rebuild/include/engine/CTCWife.h is the PDB layout (16-byte map/set); retail STLport trees are 12 bytes (header,
//          node count, empty compare), so FrameLastReceivedNiceGift sits at +0x84 (PDB +0x88) and FrameNoticedChangeInAppearance
//          at +0xa4 (PDB +0xac); a local model with the PDB member names. CurrentRegionNumber (+0x18) is left unset.
// Shape: the members are initialiser-list stores in declaration order; the appearance-frame array is zeroed by a loop in the
//          body (VC7.1 unrolls it through its own zero register) and the four husband-appearance flags by one dword store.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CThing;
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    CTCBase(CThing* thing);                     // 0x00686800
};
template <class TNode> struct CTreeBase {       // STLport _Rb_tree_base: the header node pointer
    TNode* m_pHeader;
    CTreeBase() : m_pHeader(0)
    {
        m_pHeader = (TNode*)malloc(sizeof(TNode));
    }
};
template <class TNode> struct CTree : CTreeBase<TNode> {   // 12 bytes
    unsigned long m_nodeCount;
    struct SLess { } m_compare;
    CTree() : m_nodeCount(0)
    {
        this->m_pHeader->m_color = 0;
        this->m_pHeader->m_pParent = 0;
        this->m_pHeader->m_pLeft = this->m_pHeader;
        this->m_pHeader->m_pRight = this->m_pHeader;
    }
};
struct SLongLongNode {                          // map<long, long> node, 0x18 bytes
    unsigned char m_color;
    SLongLongNode* m_pParent;
    SLongLongNode* m_pLeft;
    SLongLongNode* m_pRight;
    long m_key;
    long m_value;
};
struct SLongNode {                              // set<long> node, 0x14 bytes
    unsigned char m_color;
    SLongNode* m_pParent;
    SLongNode* m_pLeft;
    SLongNode* m_pRight;
    long m_key;
};
struct CHeroMarriageDef;
struct CDefPointerHeroMarriage {
    const CHeroMarriageDef* Object;
    CDefPointerHeroMarriage() : Object(0) { }
};
extern const unsigned long g_invalidSampleID;   // 0x0129D25C
struct CTCWife : CTCBase {
    CDefPointerHeroMarriage PHeroMarriageDef;   // +0x0c
    long OpinionAttitude;                       // +0x10 EOpinionAttitudeType
    unsigned char AttitudeUpdateCounter;        // +0x14
    bool CourtingBlocked;                       // +0x15
    long CurrentRegionNumber;                   // +0x18
    bool PermittedToRegionFollow;               // +0x1c
    long FrameGotMarriedToThePlayer;            // +0x20
    bool DivorcedHero;                          // +0x24
    bool JustMarried;                           // +0x25
    bool NeedsToChangeBrain;                    // +0x26
    bool WaitingForFadeOut;                     // +0x27
    bool WaitingForFadeIn;                      // +0x28
    bool WaitingForDivorce;                     // +0x29
    bool WaitingForDivorceRenting;              // +0x2a
    bool WaitingForDowry;                       // +0x2b
    bool WaitingForSex;                         // +0x2c
    bool WaitingForSexCutscene;                 // +0x2d
    unsigned long CurrentSexCutsceneSampleID;   // +0x30
    long FrameStartedSexCutscene;               // +0x34
    long FrameEndedSoundInSexCutscene;          // +0x38
    float CurrentProportionOfSoundGap;          // +0x3c
    long FrameLastAwareOfHusband;               // +0x40
    long FrameToCheckAppearanceChanges;         // +0x44
    long FrameLastReducedOpinion;               // +0x48
    long FrameLastEvaluatedGiftOpinion;         // +0x4c
    long FrameLastConsideredGivingGift;         // +0x50
    long FrameLastEvaluatedLoveAttitude;        // +0x54
    long FrameEnteredAttitudeHate;              // +0x58
    long FrameLastGaveDivorceWarning;           // +0x5c
    long FrameEnteredLoveWithHusbandPresentAtHome;  // +0x60
    long FrameLastGaveSexOffer;                 // +0x64
    bool DivorceDeedPending;                    // +0x68
    bool DivorceWarningPending;                 // +0x69
    float GiftGivingOpinionDistanceFromMax;     // +0x6c
    long GiftGivingPriceValue;                  // +0x70
    long GiftToGiveDef;                         // +0x74
    CTree<SLongLongNode> GiftsReceived;         // +0x78 map<long, long>
    long FrameLastReceivedNiceGift;             // +0x84
    long FrameLastCulledGiftsReceived;          // +0x88
    float LoveAttitudeValue;                    // +0x8c
    bool HasBeenInLoveWithPlayer;               // +0x90
    bool ReceivedWeddingRing;                   // +0x91
    long HouseDressingLevelLastCommentedOn;     // +0x94
    CTree<SLongNode> ThingsSeenThePlayerWear;   // +0x98 set<long>
    long FrameNoticedChangeInAppearance[16];    // +0xa4
    float LastFatnessChangePoint;               // +0xe4
    bool BooleanHusbandAppearances[4];          // +0xe8
    CTCWife(CThing* thing);
};

CTCWife::CTCWife(CThing* thing)
    : CTCBase(thing), OpinionAttitude(0), AttitudeUpdateCounter(0), CourtingBlocked(false), PermittedToRegionFollow(false),
      FrameGotMarriedToThePlayer(0x7fffffff), DivorcedHero(false), JustMarried(false), NeedsToChangeBrain(false),
      WaitingForFadeOut(false), WaitingForFadeIn(false), WaitingForDivorce(false), WaitingForDivorceRenting(false),
      WaitingForDowry(false), WaitingForSex(false), WaitingForSexCutscene(false), CurrentSexCutsceneSampleID(g_invalidSampleID),
      FrameStartedSexCutscene(0x7fffffff), FrameEndedSoundInSexCutscene(0x7fffffff), CurrentProportionOfSoundGap(1.0f),
      FrameLastAwareOfHusband(0), FrameToCheckAppearanceChanges(0), FrameLastReducedOpinion(0), FrameLastEvaluatedGiftOpinion(0),
      FrameLastConsideredGivingGift(0), FrameLastEvaluatedLoveAttitude(0), FrameEnteredAttitudeHate(0x7fffffff),
      FrameLastGaveDivorceWarning(0), FrameEnteredLoveWithHusbandPresentAtHome(0x7fffffff), FrameLastGaveSexOffer(0),
      DivorceDeedPending(false), DivorceWarningPending(false), GiftGivingOpinionDistanceFromMax(0.0f), GiftGivingPriceValue(500),
      GiftToGiveDef(0), FrameLastReceivedNiceGift(0), FrameLastCulledGiftsReceived(0), LoveAttitudeValue(0.0f),
      HasBeenInLoveWithPlayer(false), ReceivedWeddingRing(false), HouseDressingLevelLastCommentedOn(-1),
      LastFatnessChangePoint(0.0f)
{
    for (long i = 0; i < 16; ++i)
        FrameNoticedChangeInAppearance[i] = 0;
    for (long j = 0; j < 4; ++j)
        BooleanHusbandAppearances[j] = false;
}
