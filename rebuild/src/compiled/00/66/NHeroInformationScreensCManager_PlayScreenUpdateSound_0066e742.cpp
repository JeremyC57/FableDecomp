// NHeroInformationScreens::CManager::PlayScreenUpdateSound(EScreenSoundTypes type) const -- play the hero-information screen's update sample (the GUI def's inventory no-movement sound for type 2, else sample 0) through the game state's sound player
// Leaf: ?PlayScreenUpdateSound@CManager@NHeroInformationScreens
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066E742, 87 bytes, thiscall `ret 4` (`this` unused). Name: Ego_r.pdb
//   `?PlayScreenUpdateSound@CManager@NHeroInformationScreens@@QBEXW4EScreenSoundTypes@2@@Z` (the only one-argument void
//   const sound member of the class; it sits in the NHeroInformationScreens TU next to CBase::CBase 0x0066E734). No call or
//   address reference reaches it in retail.
// Callees: CPlayerGui::PeekGuiDef() @ 0x00438570 (static `__fastcall`, `const CPlayerGuiDef&`), the CSoundInit default
//   ctor @ 0x0041C930 (out of line in this TU; landed as SSettings_0041c930), the game state's virtual slot 9
//   GetSoundPlayer (+0x24) and the player's slot 3 PlaySample(bank, sample, loop, init, attached) (+0x0c; the landed
//   CThingSoundEmitter 0x00836840 model). Globals: the game state @ 0x013B8394, the main game component @ 0x013B86A0
//   (+0x10 the sample bank, as the landed CGameScriptInterface::PlaySoundAtPos 0x00891170).
// Label correction: the catalog "FrontEnd_PlaySoundEffect" is descriptive.
// Layout: CPlayerGuiDef InventoryNoMovementSound +0x3c (Ego_r.pdb +0x40; the retail CParentDefClassBase base is 0x3c).
// Shape: `long sample = 0; switch (type) { case 2: ... }` (`dec eax ; dec eax ; jne`); the bank read before the
//   GetSoundPlayer call; the CSoundInit() temporary built after `attached` is pushed. Opt: size TU (`optimize("s", on)`;
//   /Os gives the ebp frame and `leave` for the stack locals).
#pragma optimize("s", on)
class CCharString;
class CASoundBank;
class CSoundInit {
public:
    float         Volume, Pitch, Pan;
    long          NoPlays, PlaybackType;
    unsigned long GainFaderBits, PitchFaderBits;
    long          OverrideID;
    CSoundInit();                               // 0x0041C930
};
class CSoundPlayer {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual unsigned long PlaySample(CASoundBank& bank, long sample, bool loop, const CSoundInit& init, bool attached);   // slot 3 (+0x0c)
};
class CGameState {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual CSoundPlayer* GetSoundPlayer();     // slot 9 (+0x24)
};
class CMainGameComponent {
public:
    unsigned char _pad00[0x10];
    CASoundBank*  PSampleBank;                  // +0x10
    CASoundBank& GetSampleBank() const { return *PSampleBank; }
};
extern CGameState*         g_pGameState;            // 0x013B8394
extern CMainGameComponent* g_pMainGameComponent;    // 0x013B86A0
class CPlayerGuiDef {
public:
    unsigned char _pad00[0x3c];
    long          InventoryNoMovementSound;     // +0x3c
};
class CPlayerGui {
public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();   // 0x00438570
};

namespace NHeroInformationScreens {
enum EScreenSoundTypes {
    SCREEN_SOUND_TYPE_0,
    SCREEN_SOUND_TYPE_1,
    SCREEN_SOUND_INVENTORY_NO_MOVEMENT          // 2 (model name)
};
class CManager {
public:
    void PlayScreenUpdateSound(EScreenSoundTypes type) const;
};

void CManager::PlayScreenUpdateSound(EScreenSoundTypes type) const
{
    long sample = 0;
    switch (type) {
    case SCREEN_SOUND_INVENTORY_NO_MOVEMENT:
        sample = CPlayerGui::PeekGuiDef().InventoryNoMovementSound;
        break;
    }
    if (g_pGameState) {
        CASoundBank& bank = g_pMainGameComponent->GetSampleBank();
        g_pGameState->GetSoundPlayer()->PlaySample(bank, sample, true, CSoundInit(), false);
    }
}
}