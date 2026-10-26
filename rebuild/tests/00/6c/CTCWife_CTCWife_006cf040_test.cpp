#include <cstdio>
#include <cstdlib>
// Behaviour test for CTCWife::CTCWife (0x006CF040): the constructor of src/ over stand-ins -- the base gets the thing,
// the flags are false, the "never" frames LONG_MAX, gift price 500, and both trees start empty with self-linked headers.
static unsigned long g_invalidSampleID = 0xdeadbeef;
class CThing { public: int dummy; };
class CTCBase { public:
    CThing* ParentThing;
    bool ToBeRemoved;
    bool IsInGlobalUpdateTCs;
    CTCBase(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {}
};
template <class TNode> struct CTreeBase {
    TNode* m_pHeader;
    CTreeBase() : m_pHeader(0) { m_pHeader = (TNode*)malloc(sizeof(TNode)); }
};
template <class TNode> struct CTree : CTreeBase<TNode> {
    unsigned long m_nodeCount;
    CTree() : m_nodeCount(0)
    {
        this->m_pHeader->m_color = 0;
        this->m_pHeader->m_pParent = 0;
        this->m_pHeader->m_pLeft = this->m_pHeader;
        this->m_pHeader->m_pRight = this->m_pHeader;
    }
};
struct SLongLongNode { unsigned char m_color; SLongLongNode* m_pParent; SLongLongNode* m_pLeft; SLongLongNode* m_pRight; long m_key; long m_value; };
struct SLongNode { unsigned char m_color; SLongNode* m_pParent; SLongNode* m_pLeft; SLongNode* m_pRight; long m_key; };
class CHeroMarriageDef;
struct CDefPointerHeroMarriage { const CHeroMarriageDef* Object; CDefPointerHeroMarriage() : Object(0) {} };
class CTCWife : public CTCBase { public:
    CDefPointerHeroMarriage PHeroMarriageDef;
    long OpinionAttitude;
    unsigned char AttitudeUpdateCounter;
    bool CourtingBlocked;
    long CurrentRegionNumber;
    bool PermittedToRegionFollow;
    long FrameGotMarriedToThePlayer;
    bool DivorcedHero, JustMarried, NeedsToChangeBrain, WaitingForFadeOut, WaitingForFadeIn, WaitingForDivorce,
         WaitingForDivorceRenting, WaitingForDowry, WaitingForSex, WaitingForSexCutscene;
    unsigned long CurrentSexCutsceneSampleID;
    long FrameStartedSexCutscene, FrameEndedSoundInSexCutscene;
    float CurrentProportionOfSoundGap;
    long FrameLastAwareOfHusband, FrameToCheckAppearanceChanges, FrameLastReducedOpinion, FrameLastEvaluatedGiftOpinion,
         FrameLastConsideredGivingGift, FrameLastEvaluatedLoveAttitude, FrameEnteredAttitudeHate, FrameLastGaveDivorceWarning,
         FrameEnteredLoveWithHusbandPresentAtHome, FrameLastGaveSexOffer;
    bool DivorceDeedPending, DivorceWarningPending;
    float GiftGivingOpinionDistanceFromMax;
    long GiftGivingPriceValue;
    long GiftToGiveDef;
    CTree<SLongLongNode> GiftsReceived;
    long FrameLastReceivedNiceGift, FrameLastCulledGiftsReceived;
    float LoveAttitudeValue;
    bool HasBeenInLoveWithPlayer, ReceivedWeddingRing;
    long HouseDressingLevelLastCommentedOn;
    CTree<SLongNode> ThingsSeenThePlayerWear;
    long FrameNoticedChangeInAppearance[16];
    float LastFatnessChangePoint;
    bool BooleanHusbandAppearances[4];
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

static int fail(int line) { printf("006cf040_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CThing thing;
    CTCWife* w = new CTCWife(&thing);
    CHECK(w->ParentThing == &thing && w->PHeroMarriageDef.Object == 0);
    CHECK(w->OpinionAttitude == 0 && w->AttitudeUpdateCounter == 0 && !w->CourtingBlocked && !w->PermittedToRegionFollow);
    CHECK(w->FrameGotMarriedToThePlayer == 0x7fffffff && w->FrameStartedSexCutscene == 0x7fffffff);
    CHECK(w->FrameEndedSoundInSexCutscene == 0x7fffffff && w->FrameEnteredAttitudeHate == 0x7fffffff);
    CHECK(w->FrameEnteredLoveWithHusbandPresentAtHome == 0x7fffffff);
    CHECK(!w->DivorcedHero && !w->JustMarried && !w->NeedsToChangeBrain && !w->WaitingForFadeOut && !w->WaitingForFadeIn);
    CHECK(!w->WaitingForDivorce && !w->WaitingForDivorceRenting && !w->WaitingForDowry && !w->WaitingForSex);
    CHECK(!w->WaitingForSexCutscene && w->CurrentSexCutsceneSampleID == 0xdeadbeef && w->CurrentProportionOfSoundGap == 1.0f);
    CHECK(w->FrameLastAwareOfHusband == 0 && w->FrameLastGaveSexOffer == 0 && w->FrameLastGaveDivorceWarning == 0);
    CHECK(!w->DivorceDeedPending && !w->DivorceWarningPending && w->GiftGivingOpinionDistanceFromMax == 0.0f);
    CHECK(w->GiftGivingPriceValue == 500 && w->GiftToGiveDef == 0 && w->HouseDressingLevelLastCommentedOn == -1);
    CHECK(w->GiftsReceived.m_nodeCount == 0 && w->GiftsReceived.m_pHeader->m_pLeft == w->GiftsReceived.m_pHeader);
    CHECK(w->GiftsReceived.m_pHeader->m_pRight == w->GiftsReceived.m_pHeader && w->GiftsReceived.m_pHeader->m_pParent == 0);
    CHECK(w->ThingsSeenThePlayerWear.m_nodeCount == 0 && w->ThingsSeenThePlayerWear.m_pHeader->m_color == 0);
    CHECK(w->ThingsSeenThePlayerWear.m_pHeader->m_pLeft == w->ThingsSeenThePlayerWear.m_pHeader);
    for (int i = 0; i < 16; ++i) CHECK(w->FrameNoticedChangeInAppearance[i] == 0);
    for (int j = 0; j < 4; ++j) CHECK(!w->BooleanHusbandAppearances[j]);
    CHECK(w->LoveAttitudeValue == 0.0f && w->LastFatnessChangePoint == 0.0f && !w->HasBeenInLoveWithPlayer && !w->ReceivedWeddingRing);
    printf("006cf040_TEST PASS\n");
    return 0;
}