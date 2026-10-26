// CTCWife::GetGreetDialogueGroupName(CCharString& groupName) const -- the dialogue group a greeting from the wife uses: married, the sex offer (stamping its frame) or the attitude greet line; courting, the marriage line, the need-house / need-ring line, or by chance one of four courting-advice lines; false when she has nothing to say
// Leaf: GetGreetDialogueGroupName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D0410, length 370 bytes (`ret 4`, no frame pointer; Ego_r.pdb
//   `?GetGreetDialogueGroupName@CTCWife@@QBE_NAAVCCharString@@@Z`).
// Callees: CWorld::GetWorldFrame() 0x0049D870 (static; the inline IsMarriedToThePlayer as the landed
//   InitWifeNonpersistentState 0x006D03E0); CTCWife::IsOfferingSex() const 0x006CEEA0, IsCourtingHero() const 0x006CF890,
//   IsWillingToMarry() const 0x006CFEC0 (manifest); CCharString::operator=(const char*) 0x0099EFE0, operator=(const
//   CCharString&) 0x0099EFB0, ~CCharString 0x0099EAE0, operator+(const char*, const CCharString&) 0x0099F690 (__fastcall,
//   hidden result in ecx, as landed); CTCBase::GetWorld() const 0x00686A70 and PeekWorld() const 0x00686A80 (both read the
//   world from 0x013B8A1C +0x24; manifest "GetDisplayEngine" propagated; PDB order GetWorld, PeekWorld);
//   COpinionReactionManager::GetThingAttitudeToPlayer(CThing*) 0x00701840 (world +0x74); CTCBase::GetPWorldSeed()
//   0x00686DA0 (as landed); GFFloatRandom(float, unsigned long*) 0x004978A0 and GFROR13(unsigned long) 0x00497890
//   (__fastcall, as landed).
// Data: "TEXT_AI_WIFE_SEX_OFFER_TO_GO_TO_BED" 0x0125EEB0, "TEXT_AI_WIFE_OPINION_ACT_GREET_TO_HERO_" 0x0125EE88,
//   "TEXT_AI_COMMENT_COURTSHIP_MARRIAGE" 0x0125EE64, "TEXT_AI_COMMENT_COURTSHIP_NEED_HOUSE" 0x0125EE3C,
//   "TEXT_AI_COMMENT_COURTSHIP_NEED_WEDDING_RING" 0x0125EE10; the attitude-name table 0x013BABD0 (as landed); the
//   four-entry courting-advice table 0x013BAB30 (descriptive name).
// Layout: engine/CTCWife.h holds the PDB layout, but retail differs past GiftsReceived (+0x78: a 12-byte STLport map in
//   retail, 16 in the PDB), so this is a local model: PHeroMarriageDef +0x0c, OpinionAttitude +0x10,
//   FrameGotMarriedToThePlayer +0x20, FrameLastGaveSexOffer +0x64 (PDB = retail below +0x78), ReceivedWeddingRing +0x91
//   (PDB +0x95 - 4; retail reads +0x91 here). CHeroMarriageDef ProbabilityOfCourtingAdviceOnGreet +0x88 (PDB). The parent
//   thing's +0x1C9 bit 4 is CThingAICreature::CanBeMarried (bitfield run as the landed CThingAICreature() 0x00831F80).
// Shape: the sex-offer frame stamp writes a mutable member and still calls GetWorld() for the static frame read; the
//   greet line is `prefix + table[attitude]` assigned from the temporary, the attitude coming through an inline helper
//   taking the parent thing as a parameter (retail loads it into edi before the PeekWorld call) and an inline CWorld
//   getter for the reaction manager (eax, then `mov ecx, eax` after the push); the advice chance is written
//   `def->Probability > GFFloatRandom(..)` (def loaded into edi before the seed call); the advice pick is the engine's
//   inline LCG (`seed*0x24A1 + 0x24DF`, GFROR13, `% 4`).
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const char* text);               // 0x0099EFE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690

extern CCharString g_OpinionAttitudeNames[];                // 0x013BABD0
extern CCharString g_CourtingAdviceGroupNames[4];           // 0x013BAB30

unsigned long __fastcall GFROR13(unsigned long value);      // 0x00497890
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
__forceinline unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

class CThing;
enum EOpinionAttitudeType { OPINION_ATTITUDE_IN_LOVE_COURTING = 12 };
class COpinionReactionManager { public:
    EOpinionAttitudeType GetThingAttitudeToPlayer(CThing* thing);   // 0x00701840
};
class CWorld {
public:
    unsigned char            _pad00[0x74];
    COpinionReactionManager* OpinionReactionManager;        // +0x74
    static long GetWorldFrame();                            // 0x0049D870
    COpinionReactionManager* GetOpinionReactionManager() const { return OpinionReactionManager; }
};
struct CThingAICreatureFlags {                              // CThingAICreature bitfield run at +0x1C8
    unsigned char _pad00[0x1c8];
    bool PerformingVillageTaskFlag : 1;                     // +0x1c8
    bool AISuspended : 1;
    bool HasInformation : 1;
    bool WanderWithInformation : 1;
    bool WaveWithInformation : 1;
    bool ContinueAIWithInformation : 1;
    bool ImmuneToAttack : 1;
    bool EnableCreatureAutoPlacing : 1;
    bool AllowedToFollowHero : 1;                           // +0x1c9
    bool RegionFollowingOverriddenFromScript : 1;
    bool RespondingToFollowAndWait : 1;
    bool CanBeCourted : 1;
    bool CanBeMarried : 1;                                  // +0x1c9 bit 4
};
class CHeroMarriageDef { public:
    unsigned char _pad00[0x88];
    float         ProbabilityOfCourtingAdviceOnGreet;       // +0x88
};

class CTCBase
{
public:
    virtual void v00();
protected:
    unsigned long* GetPWorldSeed() const;   // 0x00686DA0
    CWorld& GetWorld() const;   // 0x00686A70
    const CWorld& PeekWorld() const;   // 0x00686A80
};

class CTCWife : public CTCBase { public:
    CThing*                  ParentThing;                   // +0x04
    unsigned char            _pad08[4];
    const CHeroMarriageDef*  PHeroMarriageDef;              // +0x0c CDefPointer<const CHeroMarriageDef>
    EOpinionAttitudeType     OpinionAttitude;               // +0x10
    unsigned char            _pad14[0x20 - 0x14];
    long                     FrameGotMarriedToThePlayer;    // +0x20
    unsigned char            _pad24[0x64 - 0x24];
    mutable long             FrameLastGaveSexOffer;         // +0x64
    unsigned char            _pad68[0x91 - 0x68];
    bool                     ReceivedWeddingRing;           // +0x91 (retail)
    bool IsMarriedToThePlayer() const { return FrameGotMarriedToThePlayer <= CWorld::GetWorldFrame(); }
    EOpinionAttitudeType GetAttitudeToPlayer(CThing* thing) const
    {
        return PeekWorld().GetOpinionReactionManager()->GetThingAttitudeToPlayer(thing);
    }
    bool IsOfferingSex() const;                             // 0x006CEEA0
    bool IsCourtingHero() const;                            // 0x006CF890
    bool IsWillingToMarry() const;                          // 0x006CFEC0
    bool GetGreetDialogueGroupName(CCharString& groupName) const;
};

bool CTCWife::GetGreetDialogueGroupName(CCharString& groupName) const
{
    if (IsMarriedToThePlayer()) {
        if (IsOfferingSex()) {
            groupName = "TEXT_AI_WIFE_SEX_OFFER_TO_GO_TO_BED";
            FrameLastGaveSexOffer = GetWorld().GetWorldFrame();
            return true;
        }
        groupName = "TEXT_AI_WIFE_OPINION_ACT_GREET_TO_HERO_" + g_OpinionAttitudeNames[GetAttitudeToPlayer(ParentThing)];
        return true;
    }
    if (!IsCourtingHero())
        return false;
    if (IsWillingToMarry()) {
        groupName = "TEXT_AI_COMMENT_COURTSHIP_MARRIAGE";
        return true;
    }
    if (IsCourtingHero() && OpinionAttitude == OPINION_ATTITUDE_IN_LOVE_COURTING) {
        if (ReceivedWeddingRing)
            groupName = "TEXT_AI_COMMENT_COURTSHIP_NEED_HOUSE";
        else
            groupName = "TEXT_AI_COMMENT_COURTSHIP_NEED_WEDDING_RING";
        return true;
    }
    if (((const CThingAICreatureFlags*)ParentThing)->CanBeMarried &&
        PHeroMarriageDef->ProbabilityOfCourtingAdviceOnGreet > GFFloatRandom(1.0f, GetPWorldSeed()) && IsCourtingHero()) {
        groupName = g_CourtingAdviceGroupNames[GFRandom(4, GetPWorldSeed())];
        return true;
    }
    return false;
}