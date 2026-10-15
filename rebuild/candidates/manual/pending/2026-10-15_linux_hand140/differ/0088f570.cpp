// CGameScriptInterface::Play2DSound(const CCharString& symbol) const -- plays the engine sound bank's sample for the symbol once, unpositioned, through the game state's sound player; the invalid sound handle when the symbol has no sample
// Grade: DIFFER(150v147) -- retail keeps the constant 1 in edi (`push edi ; mov edi,1`, stored to NoPlays and PlaybackType and pushed as `attached`), stores the CSoundInit words in the order +0,+4,+8,+c,+18,+1c,+10,+14 and loads the engine global before the symbol argument; ours stores immediates in declaration order and pushes `1`; tried the default ctor, an argument-taking ctor, optimize("s")
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088F570, length 145 bytes.
// Callees: CASoundBank::GetSoundIndexFromSymbol @ 0x00A01920 on the engine's sound bank (g_pEngine @ 0x013B86A0, +0x10), the game state's (0x013B8394) virtual slot 9 (+0x24, the sound player),
// the player's virtual slot 3 (+0x0c, PlaySample(bank, sample, loop, init, attached)); the invalid handle global @ 0x0129D25C.
// Shape: a 0x20-byte CSoundInit (Ego_r.pdb layout) built by its inline constructor (Volume 1, Pitch 1, Pan 0, NoPlays 1, PlaybackType 1, GainFaderBits 0x8010, PitchFaderBits 0, OverrideID -1) before the player is fetched; the bank is re-read for the call.
// Leaf: Play2DSound
#include <string.h>
struct CThing_Retail;
struct CScriptThing;
// CCharString (Ego_r.pdb layout, as the landed CCharString::operator!= 0x0099E960): a 4-byte handle on a 17-byte block.
#pragma pack(push, 1)
struct CBasicStringChar {                   // CBasicString<char>, 13 bytes
    char*         PData;                    // +0x00
    unsigned long StringLength;             // +0x04
    unsigned long DataLength : 31;          // +0x08
    bool          UseFastExtend : 1;        // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {                    // 17 bytes
    CBasicStringChar Data;                  // +0x00
    long             NoRefs;                // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;           // +0x00
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
    bool IsEmpty() const { return PStringData == 0 || PStringData->Data.StringLength == 0; }
};
struct CSoundInit {                                 // Ego_r.pdb CSoundInit, 0x20 bytes
    float         Volume;                           // +0x00
    float         Pitch;                            // +0x04
    float         Pan;                              // +0x08
    long          NoPlays;                          // +0x0c
    long          PlaybackType;                     // +0x10 (ESoundPlaybackType)
    unsigned long GainFaderBits;                    // +0x14
    unsigned long PitchFaderBits;                   // +0x18
    long          OverrideID;                       // +0x1c
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), PlaybackType(1), GainFaderBits(0x8010), PitchFaderBits(0), OverrideID(-1) { }
};
struct CASoundBank_Retail {
    long GetSoundIndexFromSymbol(const CCharString& symbol);   // 0x00A01920
};
struct CSoundPlayer_Retail {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual long PlaySample(CASoundBank_Retail& bank, long sample, bool loop, const CSoundInit& init, bool attached);   // slot 3 (+0x0c)
};
struct CGameState_Retail {
    virtual void s000(); virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005(); virtual void s006(); virtual void s007();
    virtual void s008();
    virtual CSoundPlayer_Retail* GetSoundPlayer();  // slot 9 (+0x24)
};
struct CEngine_Retail {
    unsigned char       _pad00[0x10];
    CASoundBank_Retail* m_pSoundBank;               // +0x10
};
extern CEngine_Retail*    g_pEngine;                // 0x013B86A0
extern CGameState_Retail* g_pGameState;             // 0x013B8394
extern long               g_InvalidSoundHandle;     // 0x0129D25C
struct CWorld;
struct CPlayerManager_Retail;
struct CDefinitionManager_Retail;
struct CGameScriptInterface {
    virtual void v000();
    CWorld*                    World;               // +0x04 (engine/CGameScriptInterface.h)
    void*                      Component;           // +0x08
    void*                      DisplayEngine;       // +0x0c
    CDefinitionManager_Retail* DefinitionManager;   // +0x10
    CPlayerManager_Retail*     PlayerManager;       // +0x14
    long Play2DSound(const CCharString& symbol) const;
};

long CGameScriptInterface::Play2DSound(const CCharString& symbol) const
{
    long sample = g_pEngine->m_pSoundBank->GetSoundIndexFromSymbol(symbol);
    if (sample) {
        CSoundInit init;
        return g_pGameState->GetSoundPlayer()->PlaySample(*g_pEngine->m_pSoundBank, sample, false, init, true);
    }
    return g_InvalidSoundHandle;
}
