// CTCFishing::FinishFishing(bool caught) -- stops the reel/escape sounds on the hero, closes the fishing mini-game GUI and button prompts, scores a caught fish's weight, posts the finish message and leaves fishing mode
// Leaf: ?FinishFishing@CTCFishing@@QAEX_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Naming wave 18: the global stand-in g_PlayerGui (0x013B8790) is the private static CCountedPointer<CPlayerGui>
//   CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B0950, 584 bytes, `ret 4`.
// Callees: CTCBase::PeekPlayerManager @ 0x00686D50, CPlayerManager::GetMainPlayer const @ 0x00449960,
//          CPlayer::GetPControlledCreature @ 0x00487DC0 (Ego_r.pdb names; the const chain), CVectorMap::LowerBound @ 0x0040F020, sound stop @ 0x0066ADD0
//          (manifest "SetToKillOnLevelUnload", folded), CDrawQuestInfo::DisplayMiniGameInfo @ 0x00654593,
//          CCharString(const char*, long) @ 0x0099EBF0, CSubtitleRenderer::SetButton2OverrideText @ 0x0048F881 /
//          SetButton1OverrideText @ 0x0048F7C0, ~CCharString @ 0x0099EAE0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CTCHeroOnlineScoreboard::UpdateScoreForActivity @ 0x0055F3B0, CTCBase::GetWorld @ 0x00686A70, message add
//          @ 0x00687540, CThing::GetPlayer @ 0x004C7A30, CPlayer::RemoveMode @ 0x00634190. Globals: player GUI @ 0x013B8790,
//          g_framesPerSecond @ 0x01375550, "" @ 0x0122D70E.
// Layout: Ego_r.pdb CTCFishing (matches retail): PFish +0x0C, Stage +0x14, StageInitialised +0x18, ReelingInSoundID +0x54,
//         FishEscapingSoundID +0x58.
// Shape: the sound ids are read into locals (edi); the empty prompts are implicit const char* conversions (default length
//        -1), so the second temporary lands in the dead `caught` slot; the zero vector and the world are named locals,
//        which builds the vector and calls GetWorld before the message arguments are pushed.
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    CBaseObject* PItem;
};
class CThing;
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* Get() const { return (T*)GetPItem(); }
};
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
#include "engine/C3DVector.h"
extern long g_framesPerSecond;                          // 0x01375550
class CTCBase;

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThingPlayerCreature;
class CPlayer {
public:
    void RemoveMode(long mode);                         // 0x00634190
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};

class CThingPlayerCreature : public CThing { };
class CMessageManager {
public:
    void AddMessage(long type, long frames, const C3DVector& pos, long a, long b, long c, long d, const CThing* thing);   // 0x00687540
};
class CWorld {
public:
    unsigned char _pad00[0x60];
    CMessageManager* PMessages;                         // +0x60
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                      // +0x04
    unsigned char _pad08[4];
protected:
    const CPlayerManager& PeekPlayerManager() const;    // 0x00686D50
public:
    __forceinline void PostMessage(long type, const C3DVector& pos, const CThing* thing)
    {
        GetWorld().PMessages->AddMessage(type, g_framesPerSecond, pos, 0, 0, 0, 0, thing);
    }
protected:
    CWorld& GetWorld() const;   // 0x00686A70 (Ego_r.pdb CTCBase, protected)
};
class CTCSoundEmitter : public CTCBase {
public:
    void StopSound(unsigned long id);                   // 0x0066ADD0
};
class CTCHeroOnlineScoreboard : public CTCBase {
public:
    void UpdateScoreForActivity(long activity, float value, bool flag);   // 0x0055F3B0
};
class CTCFish : public CTCBase {
public:
    unsigned char _pad0c[0x14 - 0x0c];
    float Weight;                                       // +0x14
};
class CDrawQuestInfo {
public:
    void DisplayMiniGameInfo(bool show, long type, long value);   // 0x00654593
};
class CSubtitleRenderer {
public:
    void SetButton1OverrideText(const CCharString& text);   // 0x0048F7C0
    void SetButton2OverrideText(const CCharString& text);   // 0x0048F881
};
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CPlayerGui
    static CCountedPointer<T> PClass;                                // 0x013B8790 ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@SIAAVCPlayerGui@@XZ (inline)
};
class CPlayerGui { public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer Subtitles;                        // +0x18
    unsigned char _pad19[0xf8 - 0x19];
    bool ShowFishingMeter;                              // +0xF8
    unsigned char _padf9[0x1c8 - 0xf9];
    CDrawQuestInfo* PQuestInfo;                         // +0x1C8
};
extern long g_framesPerSecond;                          // 0x01375550

class CTCFishing : public CTCBase {
public:
    CIntelligentPointer<CThing> PFish;                  // +0x0C
    long Stage;                                         // +0x14
    bool StageInitialised;                              // +0x18
    unsigned char _pad19[0x54 - 0x19];
    unsigned long ReelingInSoundID;                     // +0x54
    unsigned long FishEscapingSoundID;                  // +0x58
    void FinishFishing(bool caught);
};

void CTCFishing::FinishFishing(bool caught)
{
    CThing* hero = PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (!hero || hero->IsDead())
        return;
    unsigned long reelingInSoundID = ReelingInSoundID;
    if (reelingInSoundID && hero->HasTC(TCI_SOUND_PLAYER)) {
        ((CTCSoundEmitter*)((CTCSoundEmitter*)&hero->GetTC(TCI_SOUND_PLAYER)))->StopSound(reelingInSoundID);
        ReelingInSoundID = 0;
    }
    unsigned long fishEscapingSoundID = FishEscapingSoundID;
    if (fishEscapingSoundID && hero->HasTC(TCI_SOUND_PLAYER)) {
        ((CTCSoundEmitter*)((CTCSoundEmitter*)&hero->GetTC(TCI_SOUND_PLAYER)))->StopSound(fishEscapingSoundID);
        FishEscapingSoundID = 0;
    }
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().PQuestInfo->DisplayMiniGameInfo(false, 1, 0);
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().Subtitles.SetButton2OverrideText("");
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().ShowFishingMeter = false;
    CThing* fish = 0;
    if (caught) {
        fish = PFish.Get();
        CTCHeroOnlineScoreboard* scoreboard;
        if (hero->HasTC(TCI_HERO_ONLINE_SCOREBOARD))
            scoreboard = (CTCHeroOnlineScoreboard*)((CTCHeroOnlineScoreboard*)&hero->GetTC(TCI_HERO_ONLINE_SCOREBOARD));
        CTCFish* fishTC = 0;
        CThing* fishThing = PFish.Get();
        if (fishThing->HasTC(TCI_FISH))
            fishTC = (CTCFish*)((CTCFish*)&fishThing->GetTC(TCI_FISH));
        scoreboard->UpdateScoreForActivity(7, fishTC->Weight, false);
    }
    C3DVector zero(0.0f, 0.0f, 0.0f);
    CWorld* world = &GetWorld();
    world->PMessages->AddMessage(0x3f, g_framesPerSecond, zero, 0, 0, 0, 0, fish);
    if (Stage) {
        Stage = 0;
        StageInitialised = false;
    }
    hero->GetPlayer().RemoveMode(0x1d);
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().Subtitles.SetButton1OverrideText("");
}