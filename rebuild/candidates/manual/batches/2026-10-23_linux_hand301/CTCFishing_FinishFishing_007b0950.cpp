// CTCFishing::FinishFishing(bool caught) -- stops the reel/escape sounds on the hero, closes the fishing mini-game GUI and button prompts, scores a caught fish's weight, posts the finish message and leaves fishing mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B0950, 584 bytes, `ret 4`.
// Callees: CTCBase::GetPlayerManager @ 0x00686D50, CPlayerManager::GetMainPlayer @ 0x00449960, CMainPlayer::GetCharacterThing
//          @ 0x00487DC0 (labels on these three are folded), CVectorMap::LowerBound @ 0x0040F020, sound stop @ 0x0066ADD0
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
class CCharString {
public:
    void* Data;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
extern long g_framesPerSecond;                          // 0x01375550
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);               // 0x0040F020
};
class CPlayer {
public:
    void RemoveMode(long mode);                         // 0x00634190
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long PresentBits[9];                       // +0x20
    CInterfaceMap Interfaces;                           // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                              // +0x91
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    CPlayer& GetPlayer();                               // 0x004C7A30
};
class CMessageManager {
public:
    void AddMessage(long type, long frames, const C3DVector& pos, long a, long b, long c, long d, const CThing* thing);   // 0x00687540
};
class CWorld {
public:
    unsigned char _pad00[0x60];
    CMessageManager* PMessages;                         // +0x60
};
class CPlayerManager {
public:
    class CMainPlayer* GetMainPlayer();                 // 0x00449960
};
class CMainPlayer {
public:
    CThing* GetCharacterThing();                        // 0x00487DC0
};
class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                      // +0x04
    unsigned char _pad08[4];
    CPlayerManager* GetPlayerManager();                 // 0x00686D50
    CWorld* GetWorld() const;                           // 0x00686A70
    __forceinline void PostMessage(long type, const C3DVector& pos, const CThing* thing)
    {
        GetWorld()->PMessages->AddMessage(type, g_framesPerSecond, pos, 0, 0, 0, 0, thing);
    }
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
struct CPlayerGui {
    unsigned char _pad00[0x18];
    CSubtitleRenderer Subtitles;                        // +0x18
    unsigned char _pad19[0xf8 - 0x19];
    bool ShowFishingMeter;                              // +0xF8
    unsigned char _padf9[0x1c8 - 0xf9];
    CDrawQuestInfo* PQuestInfo;                         // +0x1C8
};
extern CPlayerGui* g_PlayerGui;                         // 0x013B8790
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
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetCharacterThing();
    if (!hero || (hero->Flags91 & 1))
        return;
    unsigned long reelingInSoundID = ReelingInSoundID;
    if (reelingInSoundID && hero->HasTC(0x10)) {
        ((CTCSoundEmitter*)hero->GetTC(0x10))->StopSound(reelingInSoundID);
        ReelingInSoundID = 0;
    }
    unsigned long fishEscapingSoundID = FishEscapingSoundID;
    if (fishEscapingSoundID && hero->HasTC(0x10)) {
        ((CTCSoundEmitter*)hero->GetTC(0x10))->StopSound(fishEscapingSoundID);
        FishEscapingSoundID = 0;
    }
    g_PlayerGui->PQuestInfo->DisplayMiniGameInfo(false, 1, 0);
    g_PlayerGui->Subtitles.SetButton2OverrideText("");
    g_PlayerGui->ShowFishingMeter = false;
    CThing* fish = 0;
    if (caught) {
        fish = PFish.Get();
        CTCHeroOnlineScoreboard* scoreboard;
        if (hero->HasTC(0x10d))
            scoreboard = (CTCHeroOnlineScoreboard*)hero->GetTC(0x10d);
        CTCFish* fishTC = 0;
        CThing* fishThing = PFish.Get();
        if (fishThing->HasTC(0xdc))
            fishTC = (CTCFish*)fishThing->GetTC(0xdc);
        scoreboard->UpdateScoreForActivity(7, fishTC->Weight, false);
    }
    C3DVector zero(0.0f, 0.0f, 0.0f);
    CWorld* world = GetWorld();
    world->PMessages->AddMessage(0x3f, g_framesPerSecond, zero, 0, 0, 0, 0, fish);
    if (Stage) {
        Stage = 0;
        StageInitialised = false;
    }
    hero->GetPlayer().RemoveMode(0x1d);
    g_PlayerGui->Subtitles.SetButton1OverrideText("");
}
