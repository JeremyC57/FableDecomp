// Standalone C++03 fixture: the exact static array definition below is the target.
// The string constructor/destructor are traced test boundaries, not game implementations.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
class CDefPointeeBase;
class CThing;
#ifndef FABLE_ENGINE_CCHARSTRING_H
#define FABLE_ENGINE_CCHARSTRING_H
// Retail declarations (Codex pass 11): Ego_r TPI 0x27dc, class key V,
// sizeof 4, protected PStringData +0; native array construction advances by 4.
class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
    ~CCharString();
protected:
    CCharStringData* PStringData;
};
#endif

class CTCWife {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    _pad_0x04[0x4];                         // +0x04
    unsigned char    _pad_0x08[0x4];                         // +0x08
    CDefPointeeBase* PHeroMarriageDef_Object;                // +0x0c CDefPointer<CHeroMarriageDef_const_>::Object
    unsigned char    OpinionAttitude[0x4];                   // +0x10 EOpinionAttitudeType
    unsigned char    AttitudeUpdateCounter;                  // +0x14
    bool             CourtingBlocked;                        // +0x15
    unsigned char    _pad_0x16[0x2];                         // +0x16
    long             CurrentRegionNumber;                    // +0x18
    bool             PermittedToRegionFollow;                // +0x1c
    unsigned char    _pad_0x1d[0x3];                         // +0x1d
    long             FrameGotMarriedToThePlayer;             // +0x20
    bool             DivorcedHero;                           // +0x24
    bool             JustMarried;                            // +0x25
    bool             NeedsToChangeBrain;                     // +0x26
    bool             WaitingForFadeOut;                      // +0x27
    bool             WaitingForFadeIn;                       // +0x28
    bool             WaitingForDivorce;                      // +0x29
    bool             WaitingForDivorceRenting;               // +0x2a
    bool             WaitingForDowry;                        // +0x2b
    bool             WaitingForSex;                          // +0x2c
    bool             WaitingForSexCutscene;                  // +0x2d
    unsigned char    _pad_0x2e[0x2];                         // +0x2e
    unsigned long    CurrentSexCutsceneSampleID;             // +0x30
    long             FrameStartedSexCutscene;                // +0x34
    long             FrameEndedSoundInSexCutscene;           // +0x38
    float            CurrentProportionOfSoundGap;            // +0x3c
    long             FrameLastAwareOfHusband;                // +0x40
    long             FrameToCheckAppearanceChanges;          // +0x44
    long             FrameLastReducedOpinion;                // +0x48
    long             FrameLastEvaluatedGiftOpinion;          // +0x4c
    long             FrameLastConsideredGivingGift;          // +0x50
    long             FrameLastEvaluatedLoveAttitude;         // +0x54
    long             FrameEnteredAttitudeHate;               // +0x58
    long             FrameLastGaveDivorceWarning;            // +0x5c
    long             FrameEnteredLoveWithHusbandPresentAtHome; // +0x60
    long             FrameLastGaveSexOffer;                  // +0x64
    bool             DivorceDeedPending;                     // +0x68
    bool             DivorceWarningPending;                  // +0x69
    unsigned char    _pad_0x6a[0x2];                         // +0x6a
    float            GiftGivingOpinionDistanceFromMax;       // +0x6c
    long             GiftGivingPriceValue;                   // +0x70
    long             GiftToGiveDef;                          // +0x74
    unsigned char    GiftsReceived[0xc];                     // +0x78 map<long,long,std::less<long>,std::allocator<std::pair<long_const_,long>_>_>
    long             FrameLastReceivedNiceGift;              // +0x84
    long             FrameLastCulledGiftsReceived;           // +0x88
    float            LoveAttitudeValue;                      // +0x8c
    bool             HasBeenInLoveWithPlayer;                // +0x90
    bool             ReceivedWeddingRing;                    // +0x91
    unsigned char    _pad_0x92[0x2];                         // +0x92
    long             HouseDressingLevelLastCommentedOn;      // +0x94
    unsigned char    ThingsSeenThePlayerWear[0xc];           // +0x98 set<long,std::less<long>,std::allocator<long>_>
    long             FrameNoticedChangeInAppearance[16];     // +0xa4
    float            LastFatnessChangePoint;                 // +0xe4
    bool             BooleanHusbandAppearances[4];           // +0xe8
// Retail opt-in (Codex pass 11): Ego_r TPI 0x1a9a9 field 0xd74;
// private const four-element array; native initializer writes four pointer words.
private:
    static const CCharString CourtingAdviceGroupNames[4];

};
static const void* constructed[4];
static const char* supplied[4];
static long lengths[4];
static unsigned constructCount;
static unsigned destroyCount;
static int fixtureFailed;
static int mainReached;
static void finishAudit();
class TestAuditRegistration {
public:
    TestAuditRegistration() { if (atexit(finishAudit) != 0) abort(); }
};
static TestAuditRegistration auditRegistration;

const CCharString CTCWife::CourtingAdviceGroupNames[4] = {
    "TEXT_AI_COMMENT_COURTSHIP_SUGGEST_ROMANTIC_EXPRESSION",
    "TEXT_AI_COMMENT_COURTSHIP_SUGGEST_ROMANTIC_GIFT",
    "TEXT_AI_COMMENT_COURTSHIP_SUGGEST_IMPROVE_APPEARANCE",
    "TEXT_AI_COMMENT_COURTSHIP_SUGGEST_INCREASE_RENOWN"
};

CCharString::CCharString(const char* text, long length) : PStringData(0) {
    if (constructCount >= 4) abort();
    constructed[constructCount] = this;
    supplied[constructCount] = text;
    lengths[constructCount] = length;
    ++constructCount;
}
CCharString::CCharString(const CCharString&) : PStringData(0) {
    // Static conversion construction must not rely on elision of a named return value.
    abort();
}
CCharString::~CCharString() {
    if (destroyCount >= 4 || this != constructed[3 - destroyCount]) abort();
    ++destroyCount;
}
int main() {
    const char* expected[4] = {
        "TEXT_AI_COMMENT_COURTSHIP_SUGGEST_ROMANTIC_EXPRESSION",
        "TEXT_AI_COMMENT_COURTSHIP_SUGGEST_ROMANTIC_GIFT",
        "TEXT_AI_COMMENT_COURTSHIP_SUGGEST_IMPROVE_APPEARANCE",
        "TEXT_AI_COMMENT_COURTSHIP_SUGGEST_INCREASE_RENOWN"
    };
    mainReached = 1;
    if (constructCount != 4 || destroyCount != 0) fixtureFailed = 1;
    for (unsigned i = 0; i != 4; ++i) {
        if (lengths[i] != -1 || strcmp(supplied[i], expected[i])) fixtureFailed = 1;
        if (i && (const unsigned char*)constructed[i] -
                 (const unsigned char*)constructed[i-1] != sizeof(CCharString)) fixtureFailed = 1;
    }
    return fixtureFailed;
}
static void finishAudit() {
    if (!mainReached || fixtureFailed || constructCount != 4 || destroyCount != 4) abort();
    puts("0121e210_TEST PASS");
}
