// CTCFishing::FrameUpdate() -- the fishing mini-game state machine: instructions (tutorial once), waiting for a bite (the
//   "stop" prompt and a random wait), hooking (a press count and a rumble window), reeling (alternating pull / rest periods
//   with the escape / reel sounds, the line distance and strain, the escape / snap / caught checks and the hint texts) and
//   the caught stage; the per-frame press flag is cleared after every stage
// Leaf: ?FrameUpdate@CTCFishing@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B0CD0, 3416 bytes, plain `ret`; five-entry stage switch table at 0x007B1A14.
// Callees: CTCBase::GetPlayerManager 0x00686D40 / GetWorld 0x00686A70, CPlayerManager::GetMainPlayer 0x00449970,
//   CBaseIntelligentPointer::GetPItem 0x00A01B50 (the inline FableWin CPlayer::GetPPlayerCharacter, +0x34),
//   CVectorMap::LowerBound 0x0040F020 / const 0x004365B0, CTCHero::GiveTutorialForCategory 0x006C9FA0,
//   CWorld::GetWorldSeed 0x0049D810, GFRandom 0x004CDAE0, GFROR13 0x00497890, CWorld::GetFrame 0x0049D870,
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0 / operator= 0x0099EFE0, 0x0099EFB0 / operator!= 0x0099E960, 0x0099E900,
//   CSubtitleRenderer::SetButton1OverrideText 0x0048F7C0 / SetButton2OverrideText 0x0048F881,
//   CDrawQuestInfo::DisplayMiniGameInfo 0x00654593, CThing::GetJoystickDeviceNumber 0x004C7CA0, CJoystick::AddRumble (slot 17),
//   CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00 / StopAttachedSound 0x0066ADD0, CTCFishing::GetPFishingRodDef 0x007B0880,
//   NGameText::CDataBank::GetTextBySymbol 0x009C95E0, CTCRegionDisplay::SetCurrentAreaName(const CWideString&, ..) 0x00639A90,
//   ~CWideString 0x0099B510, CTCFishing::FinishFishing 0x007B0950.
// Layout = Ego_r.pdb CTCFishing (no containers, = retail); retail CFishingDef (-4 after the FishTypes vector): PointsPerFishingLevel
//   +0x44, Slow/Normal/Fast/FastestFishingSpeed +0x48..+0x54, BaseWaitPeriodSeconds +0x64, MaxRandomWaitPeriodSeconds +0x68,
//   HookWindowSeconds +0x6C, BaseHookPresses +0x70, MaxRandomHookPresses +0x74; CFishingRodDef StrainReductionPerSecond +0x28,
//   MaxStrain +0x34, MaxLengthMetres +0x38; CTCFish PDef +0x0C, Speed +0x18; CFishDef Base/MaxRandomPullSeconds +0x40/+0x44,
//   Base/MaxRandomPullWaitSeconds +0x48/+0x4C; CTCHeroExperience FishingLevel +0x0C; CPlayerGui SubtitleRenderer +0x18
//   (Button1Alpha +0x78, DisplayingText +0xE5).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CCountedPointer.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
class CTCBase;
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed);   // 0x004CDAE0 ?GFRandom@@YIKKPAK@Z
long __fastcall GFROR13(unsigned long value);           // 0x00497890 ?GFROR13@@YIJK@Z

namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
    unsigned char _pad00[0x14];
    CCountedPointer<NGameText::CDataBank> PTextBank;    // +0x14
    const NGameText::CDataBank& GetTextBank() const { return *PTextBank; }
};
class CJoystick {
public:
    class CInitRumble {
    public:
        CInitRumble(float maxLevel, float smoothness, float attackTime, float decayTime, bool cumulative)
            : MaxLevel(maxLevel), Smoothness(smoothness), AttackTime(attackTime), DecayTime(decayTime), Cumulative(cumulative) { }
        float MaxLevel;
        float Smoothness;
        float AttackTime;
        float DecayTime;
        bool  Cumulative;
    };
    virtual void JoystickVSlot00(); virtual void JoystickVSlot01(); virtual void JoystickVSlot02(); virtual void JoystickVSlot03();
    virtual void JoystickVSlot04(); virtual void JoystickVSlot05(); virtual void JoystickVSlot06(); virtual void JoystickVSlot07();
    virtual void JoystickVSlot08(); virtual void JoystickVSlot09(); virtual void JoystickVSlot10(); virtual void JoystickVSlot11();
    virtual void JoystickVSlot12(); virtual void JoystickVSlot13(); virtual void JoystickVSlot14(); virtual void JoystickVSlot15();
    virtual void JoystickVSlot16();
    virtual void AddRumble(long type, const CInitRumble& rumble, bool replace) = 0;   // slot 17 (+0x44)
};
class CInputManager {
public:
    unsigned char _pad00[0x2C];
    std::vector<CCountedPointer<CJoystick>, std::allocator<CCountedPointer<CJoystick> > > PJoysticks;   // +0x2C
};
extern CInputManager* GPInputManager;                   // 0x013B8388 ?GPInputManager@@3PAVCInputManager@@A

class CWorld {
public:
    unsigned long* GetWorldSeed() const;                // 0x0049D810 ?GetWorldSeed@CWorld@@QBEPAKXZ
    static long __fastcall GetFrame();                  // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    unsigned char _pad00[0x34];
    CIntelligentPointer<CThingPlayerCreature> PlayerCharacter;   // +0x34
    CThingPlayerCreature* GetPPlayerCharacter() const { return PlayerCharacter.Get(); }   // FableWin, inline
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                           // 0x00449970
};
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[4];
protected:
    CPlayerManager& GetPlayerManager();                 // 0x00686D40
    CWorld& GetWorld() const;                           // 0x00686A70
};
enum ETutorialCategory { };
class CTCHero : public CTCBase {
public:
    bool GiveTutorialForCategory(ETutorialCategory category, bool force);   // 0x006C9FA0
};
class CTCSoundPlayer : public CTCBase {
public:
    unsigned long PlayAttachedCriteria(const CCharString& name, bool a, bool b, bool c, long d);   // 0x0066BD00
    void StopAttachedSound(unsigned long id);           // 0x0066ADD0
};
class CTCRegionDisplay : public CTCBase {
public:
    void SetCurrentAreaName(const CWideString& name, float time, bool a, bool b, bool c);   // 0x00639A90
};
class CTCHeroExperience : public CTCBase {
public:
    long FishingLevel;                                  // +0x0C
};
class CFishDef : public CDefPointeeBase {
public:
    unsigned char _pad1c[0x40 - 0x1c];
    float BasePullSeconds;                              // +0x40
    float MaxRandomPullSeconds;                         // +0x44
    float BasePullWaitSeconds;                          // +0x48
    float MaxRandomPullWaitSeconds;                     // +0x4C
};
class CTCFish : public CTCBase {
public:
    CDefPointer<const CFishDef> PDef;                   // +0x0C
    float Length;                                       // +0x10
    float Weight;                                       // +0x14
    float Speed;                                        // +0x18
};
class CFishingRodDef {
public:
    unsigned char _pad00[0x28];
    float StrainReductionPerSecond;                     // +0x28
    float DistanceDecreasePerPress;                     // +0x2C
    float PullingDistanceDecreasePerPress;              // +0x30
    float MaxStrain;                                    // +0x34
    float MaxLengthMetres;                              // +0x38
};
class CFishingDef {
public:
    unsigned char _pad00[0x44];
    long  PointsPerFishingLevel;                        // +0x44
    float SlowFishingSpeed;                             // +0x48
    float NormalFishingSpeed;                           // +0x4C
    float FastFishingSpeed;                             // +0x50
    float FastestFishingSpeed;                          // +0x54
    unsigned char _pad58[0x64 - 0x58];
    float BaseWaitPeriodSeconds;                        // +0x64
    float MaxRandomWaitPeriodSeconds;                   // +0x68
    float HookWindowSeconds;                            // +0x6C
    long  BaseHookPresses;                              // +0x70
    long  MaxRandomHookPresses;                         // +0x74
};
class CSubtitleRenderer {
public:
    unsigned char _pad00[0x78];
    unsigned char Button1Alpha;                         // +0x78
    unsigned char _pad79[0xe5 - 0x79];
    bool DisplayingText;                                // +0xE5
    bool IsDisplayingText() const { return DisplayingText; }
    void SetButton1OverrideText(const CCharString& text);   // 0x0048F7C0
    void SetButton2OverrideText(const CCharString& text);   // 0x0048F881
};
class CDrawQuestInfo {
public:
    void DisplayMiniGameInfo(long show, long type, long value);   // 0x00654593
};
template <class T> class CTBaseSingletonNoDefaultAlloc {
    static CCountedPointer<T> PClass;                   // 0x013B8790
public:
    static T& __fastcall Get() { return *PClass; }
};
class CPlayerGui : public CTBaseSingletonNoDefaultAlloc<CPlayerGui> {
public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                 // +0x18
    unsigned char _pad[0x1c8 - 0x18 - sizeof(CSubtitleRenderer)];
    CDrawQuestInfo* PQuestInfo;                         // +0x1C8
    CSubtitleRenderer& GetSubtitleRenderer() { return SubtitleRenderer; }
};

enum EFishingStage { };

class CTCFishing : public CTCBase {
public:
    CIntelligentPointer<CThing> PFish;                  // +0x0C
    EFishingStage Stage;                                // +0x14
    bool  StageInitialised;                             // +0x18
    long  FrameStageFinishes;                           // +0x1C
    long  HookPresses;                                  // +0x20
    bool  PressedThisFrame;                             // +0x24
    long  LastFramePressed;                             // +0x28
    float Distance;                                     // +0x2C
    float MaxDistance;                                  // +0x30
    float Strain;                                       // +0x34
    long  FrameReelFinishes;                            // +0x38
    bool  Pulling;                                      // +0x3C
    long  Score;                                        // +0x40
    CCharString Text;                                   // +0x44
    long  TextTimer;                                    // +0x48
    bool  InstructionsShown;                            // +0x4C
    long  FrameStarted;                                 // +0x50
    unsigned long ReelingInSoundID;                     // +0x54
    unsigned long FishEscapingSoundID;                  // +0x58
    long  CurrentFishWeight;                            // +0x5C
    const CFishingDef* PDef;                            // +0x60 CDefPointer<const CFishingDef>::Object
    const CFishingRodDef* GetPFishingRodDef() const;    // 0x007B0880
    virtual void FrameUpdate();
protected:
    void FinishFishing(bool caught);                    // 0x007B0950
    void SetStage(EFishingStage stage) { if (Stage != stage) { Stage = stage; StageInitialised = false; } }   // Ego_r 0x007B1BE0, inline
};

void CTCFishing::FrameUpdate()
{
    CSubtitleRenderer& subtitles = CPlayerGui::Get().GetSubtitleRenderer();
    if (!Stage)
        return;
    CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPPlayerCharacter();
    switch (Stage) {
    case 1:
        if (!StageInitialised) {
            if (!InstructionsShown) {
                CTCHero* heroTC;
                if (ParentThing->GetTC(TCI_HERO, &heroTC))
                    heroTC->GiveTutorialForCategory((ETutorialCategory)0x24, false);
                InstructionsShown = true;
            }
            StageInitialised = true;
        } else if (!subtitles.IsDisplayingText())
            SetStage((EFishingStage)2);
        break;
    case 2:
        if (!StageInitialised) {
            const CFishingDef* def = PDef;
            float seconds = def->BaseWaitPeriodSeconds + GFRandom((unsigned long)def->MaxRandomWaitPeriodSeconds, GetWorld().GetWorldSeed())
                - def->HookWindowSeconds * 0.5f;
            long fps = CMainGameComponent::GetConstantFPS();
            FrameStageFinishes = (long)(CWorld::GetFrame() + fps * seconds);
            StageInitialised = true;
            Strain = 0.0f;
            CPlayerGui::Get().GetSubtitleRenderer().SetButton2OverrideText("TEXT_GUI_FISHING_STOP");
            CPlayerGui::Get().PQuestInfo->DisplayMiniGameInfo(1, 1, 0);
        } else if (FrameStageFinishes <= CWorld::GetFrame())
            SetStage((EFishingStage)3);
        break;
    case 3:
        if (!StageInitialised) {
            const CFishingDef* def = PDef;
            HookPresses = GFRandom(def->MaxRandomHookPresses, GetWorld().GetWorldSeed()) + def->BaseHookPresses;
            float hookWindow = PDef->HookWindowSeconds;
            long fps = CMainGameComponent::GetConstantFPS();
            FrameStageFinishes = (long)(CWorld::GetFrame() + fps * hookWindow);
            GPInputManager->PJoysticks[ParentThing->GetJoystickDeviceNumber()]->AddRumble(4,
                CJoystick::CInitRumble(0.35f, 0.75f, 0.0f, hookWindow, false), false);
            StageInitialised = true;
            CPlayerGui::Get().GetSubtitleRenderer().SetButton1OverrideText("TEXT_GUI_FISHING_PRESS");
            CPlayerGui::Get().GetSubtitleRenderer().Button1Alpha = 0xff;
            CTCSoundPlayer* sound;
                if (hero->GetTC(TCI_SOUND_PLAYER, &sound))
                    sound->PlayAttachedCriteria("FISHING_FISH_HOOKED", false, false, false, -1);
        } else if (CWorld::GetFrame() >= FrameStageFinishes || HookPresses == 0) {
            if (HookPresses == 0) {
                FrameStarted = CWorld::GetFrame();
                SetStage((EFishingStage)4);
            } else {
                SetStage((EFishingStage)2);
                CPlayerGui::Get().GetSubtitleRenderer().Button1Alpha = 0x80;
            }
        }
        break;
    case 4:
        if (!StageInitialised) {
            unsigned long* seed = GetWorld().GetWorldSeed();
            *seed = *seed * 0x24a1 + 0x24df;
            *seed = GFROR13(*seed);
            MaxDistance = Distance = *seed % 5 + 5.0f;
            FrameReelFinishes = CWorld::GetFrame();
            Pulling = false;
            ReelingInSoundID = 0;
            FishEscapingSoundID = 0;
            StageInitialised = true;
        } else {
            const CTCFish* fish = 0;
            PFish.Get()->PeekTC(TCI_FISH, &fish);
            CDefPointer<const CFishDef> fishDef = fish->PDef;
            const CFishingRodDef* rod = GetPFishingRodDef();
            if (CWorld::GetFrame() > FrameReelFinishes) {
                Pulling = !Pulling;
                if (Pulling) {
                    float seconds = fishDef->BasePullSeconds + GFRandom((unsigned long)fishDef->MaxRandomPullSeconds, GetWorld().GetWorldSeed());
                    FrameReelFinishes += CMainGameComponent::GetConstantFPS() * seconds;
                    CTCSoundPlayer* sound;
                    if (!FishEscapingSoundID && hero->GetTC(TCI_SOUND_PLAYER, &sound))
                        FishEscapingSoundID = sound->PlayAttachedCriteria("FISHING_FISH_ESCAPING", false, false, false, -1);
                    unsigned long reelingInSoundID = ReelingInSoundID;
                    if (reelingInSoundID && hero->HasTC(TCI_SOUND_PLAYER)) {
                        ((CTCSoundPlayer*)&hero->GetTC(TCI_SOUND_PLAYER))->StopAttachedSound(reelingInSoundID);
                        ReelingInSoundID = 0;
                    }
                } else {
                    float seconds = fishDef->BasePullWaitSeconds + GFRandom((unsigned long)fishDef->MaxRandomPullWaitSeconds, GetWorld().GetWorldSeed());
                    FrameReelFinishes += CMainGameComponent::GetConstantFPS() * seconds;
                    unsigned long fishEscapingSoundID = FishEscapingSoundID;
                    if (fishEscapingSoundID && hero->HasTC(TCI_SOUND_PLAYER)) {
                        ((CTCSoundPlayer*)&hero->GetTC(TCI_SOUND_PLAYER))->StopAttachedSound(fishEscapingSoundID);
                        FishEscapingSoundID = 0;
                    }
                }
            }
            if (PressedThisFrame) {
                CTCSoundPlayer* sound;
                if (!ReelingInSoundID && !FishEscapingSoundID && hero->GetTC(TCI_SOUND_PLAYER, &sound))
                    ReelingInSoundID = sound->PlayAttachedCriteria("FISHING_REEL_IN", false, false, false, -1);
            } else {
                if (Pulling) {
                    CPlayerGui::Get().GetSubtitleRenderer().Button1Alpha = 0x80;
                    Distance += fish->Speed / CMainGameComponent::GetConstantFPS();
                    GPInputManager->PJoysticks[ParentThing->GetJoystickDeviceNumber()]->AddRumble(4,
                        CJoystick::CInitRumble(0.75f, 0.25f, 0.1f, 0.1f, false), false);
                } else {
                    CPlayerGui::Get().GetSubtitleRenderer().Button1Alpha = 0xff;
                    if (Strain > 0.0f) {
                        float reduction = 0.0f;
                        if (rod)
                            reduction = rod->StrainReductionPerSecond;
                        Strain -= reduction;
                        if (Strain < 0.0f)
                            Strain = 0.0f;
                    }
                }
                if (CWorld::GetFrame() - LastFramePressed > 10) {
                    unsigned long reelingInSoundID = ReelingInSoundID;
                    if (reelingInSoundID && hero->HasTC(TCI_SOUND_PLAYER)) {
                        ((CTCSoundPlayer*)&hero->GetTC(TCI_SOUND_PLAYER))->StopAttachedSound(reelingInSoundID);
                        ReelingInSoundID = 0;
                    }
                }
            }
            float maxLength = 0.0f;
            if (rod)
                maxLength = rod->MaxLengthMetres;
            float maxStrain = 0.0f;
            if (rod)
                maxStrain = rod->MaxStrain;
            if (Distance >= maxLength) {
                const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
                CTCRegionDisplay* region = 0;
                hero->GetTC(TCI_REGION_DISPLAY, &region);
                region->SetCurrentAreaName(bank.GetTextBySymbol("TEXT_GUI_FISHING_GOT_AWAY"), -1.0f, false, false, true);
                CTCSoundPlayer* sound;
                    if (hero->GetTC(TCI_SOUND_PLAYER, &sound))
                        sound->PlayAttachedCriteria("FISHING_FISH_ESCAPED", false, false, false, -1);
                FinishFishing(false);
            } else if (Strain >= maxStrain) {
                const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
                CTCRegionDisplay* region = 0;
                hero->GetTC(TCI_REGION_DISPLAY, &region);
                region->SetCurrentAreaName(bank.GetTextBySymbol("TEXT_GUI_FISHING_STRAIN_HIGH"), -1.0f, false, false, true);
                CTCSoundPlayer* sound;
                    if (hero->GetTC(TCI_SOUND_PLAYER, &sound))
                        sound->PlayAttachedCriteria("FISHING_LINE_SNAPPED", false, false, false, -1);
                FinishFishing(false);
            } else if (Distance <= 0.0f) {
                long fps = CMainGameComponent::GetConstantFPS();
                float seconds = (float)(CWorld::GetFrame() - FrameStarted) / fps;
                if (seconds <= PDef->FastestFishingSpeed)
                    Score += 5;
                else if (seconds <= PDef->FastFishingSpeed)
                    Score += 4;
                else if (seconds <= PDef->NormalFishingSpeed)
                    Score += 3;
                else if (seconds <= PDef->SlowFishingSpeed)
                    Score += 2;
                else
                    Score += 1;
                CTCHeroExperience* experience;
                if (ParentThing->GetTC(TCI_HERO_EXPERIENCE, &experience) && Score >= PDef->PointsPerFishingLevel) {
                    ++experience->FishingLevel;
                    Score -= PDef->PointsPerFishingLevel;
                }
                SetStage((EFishingStage)5);
            } else {
                CTCRegionDisplay* region;
                if (hero->GetTC(TCI_REGION_DISPLAY, &region) && region && TextTimer == 0) {
                    CCharString text("");
                    if (Strain >= maxStrain * 0.8f)
                        text = "TEXT_GUI_FISHING_LINE_STRAIN";
                    else if (Distance > maxLength * 0.8f)
                        text = "TEXT_GUI_FISHING_GET_AWAY";
                    else if (Distance < maxLength * 0.2f)
                        text = "TEXT_GUI_FISHING_NEARLY_THERE";
                    if (text != "")
                        TextTimer = (long)(CMainGameComponent::GetConstantFPS() * 0.5f);
                    if (Text != text) {
                        Text = text;
                        const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
                        region->SetCurrentAreaName(bank.GetTextBySymbol(Text), -1.0f, false, false, true);
                    }
                } else
                    --TextTimer;
            }
        }
        break;
    case 5: {
        CTCSoundPlayer* sound;
            if (hero->GetTC(TCI_SOUND_PLAYER, &sound))
                sound->PlayAttachedCriteria("FISHING_FISH_CAUGHT", false, false, false, -1);
        CTCRegionDisplay* region = 0;
        hero->GetTC(TCI_REGION_DISPLAY, &region);
        const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
        region->SetCurrentAreaName(bank.GetTextBySymbol("TEXT_GUI_FISHING_CAUGHT"), -1.0f, false, false, true);
        FinishFishing(true);
        break;
    }
    }
    PressedThisFrame = false;
}