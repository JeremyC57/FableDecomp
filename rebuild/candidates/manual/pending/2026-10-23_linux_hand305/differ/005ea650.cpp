// CTCTavernGame::OnInitialActivate() -- reset the game state, drop the game master and load the three fonts named by the tavern-game def
// Leaf: OnInitialActivate
// Grade: DIFFER(638v654) -- frame 0x14 v 0x10: the GameMaster temporary takes the name slot (+4) and the font temporary moves to +0xc, so its two zero stores before ~CCharString are dropped; a named font local fixes the frame but reads the temp from the frame instead of eax
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EA650, length 654 bytes; called non-virtually from the derived games'
// OnInitialActivate (landed CTCTavernGameCardBase 0x008F1250, CTCTavernGameSpotTheAddition 0x007D83C0).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10,
// CDefClassBase::GetSubDef<CTavernGameDef> @ 0x005ED4A0, CDefStringTable::GetString(long) @ 0x009D49B0 on the global string
// table @ 0x013CA828, the font manager @ 0x013B838C GetFont(const CCharString&) @ 0x009E2C80 (catalog label propagated),
// ~CCharString @ 0x0099EAE0, operator delete @ 0x00BFE9BC.
// Layout: retail CTCTavernGame is Ego_r.pdb's with 12-byte STLport vectors (engine/CTCTavernGame.h keeps the 16-byte PDB
// vectors, so it is 0x1c off from Handles on): IsBeingPlayed +0x20, GameInSession +0x29, BaseBet +0x34, the fonts
// PScoreFont +0x80 / PTargetFont +0x88 / PStatsFont +0x90, Draw +0xa1, TavernGameState +0xc0, PGameMaster +0xd0,
// HasPrizeBeenGiven +0xe9 (the stores here prove each of these offsets).
// Shape: `PGameMaster = CIntelligentPointer<T>(0)` (temporary with its vptr store), the CDefPointer released by the def's
// inline Release() without a null test, and the font assignments through the inline CCountedPointer operator= / dtor.
struct CCharString {
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};

struct CCPPointerInfo {
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};

template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T*              data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct CFontBank;
struct CFontManager {
    CCountedPointer<CFontBank> GetFont(const CCharString& name);   // 0x009E2C80
};
extern CFontManager* g_pFontManager;                    // 0x013B838C

struct CDefStringTable {
    CCharString GetString(long id);                     // 0x009D49B0
};
extern CDefStringTable g_StringTable;                   // 0x013CA828

struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();                                 // +0x00 vfptr (Ego_r.pdb `_padding_`)
    CBaseObject* PData;                                 // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();                                 // own vtable 0x0123E7E0
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};
struct CThingAICreature;

struct CTavernGameDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long          RefCount;                             // +0x04
    unsigned char _pad08[0x11c - 0x08];
    long          ScoreFont;                            // +0x11c
    long          TargetFont;                           // +0x120
    long          StatsFont;                            // +0x124
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Data;
    CDefPointer() : Data(0) { }
    ~CDefPointer() { Data->Release(); }
    T* operator->() const { return Data; }
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer<CTavernGameDef>* def);  // 0x005ED4A0
};
struct CThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* Def;                                 // +0x70
};

struct CTCTavernGame {
    void*                           __vftable;          // +0x00
    CThing*                         ParentThing;        // +0x04
    unsigned char                   _pad08[0x20 - 0x08];
    bool                            IsBeingPlayed;      // +0x20
    bool                            GameWon;            // +0x21
    unsigned char                   _pad22[0x29 - 0x22];
    bool                            GameInSession;      // +0x29
    unsigned char                   _pad2a[0x34 - 0x2a];
    long                            BaseBet;            // +0x34
    unsigned char                   _pad38[0x80 - 0x38];
    CCountedPointer<CFontBank>      PScoreFont;         // +0x80
    CCountedPointer<CFontBank>      PTargetFont;        // +0x88
    CCountedPointer<CFontBank>      PStatsFont;         // +0x90
    unsigned char                   _pad98[0xa1 - 0x98];
    bool                            Draw;               // +0xa1
    unsigned char                   _pada2[0xc0 - 0xa2];
    long                            TavernGameState;    // +0xc0
    unsigned char                   _padc4[0xd0 - 0xc4];
    CIntelligentPointer<CThingAICreature> PGameMaster;  // +0xd0
    unsigned char                   _padd8[0xe9 - 0xd8];
    bool                            HasPrizeBeenGiven;  // +0xe9
    void OnInitialActivate();
};

void CTCTavernGame::OnInitialActivate()
{
    IsBeingPlayed = false;
    TavernGameState = 0;
    PGameMaster = CIntelligentPointer<CThingAICreature>(0);
    GameInSession = false;
    CDefPointer<CTavernGameDef> def;
    ParentThing->Def->GetSubDef(&def);
    BaseBet = 0;
    {
        CCharString name = g_StringTable.GetString(def->ScoreFont);
        PScoreFont = g_pFontManager->GetFont(name);
    }
    {
        CCharString name = g_StringTable.GetString(def->TargetFont);
        PTargetFont = g_pFontManager->GetFont(name);
    }
    {
        CCharString name = g_StringTable.GetString(def->StatsFont);
        PStatsFont = g_pFontManager->GetFont(name);
    }
    HasPrizeBeenGiven = false;
    Draw = false;
}
