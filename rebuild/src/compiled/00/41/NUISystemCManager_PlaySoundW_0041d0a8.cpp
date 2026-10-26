// NUISystem::CManager::PlaySoundW(CCharString& symbol, long bankId) -- play a UI sound by symbol: from the front end's own sample bank while the new front end runs, else from the given registry bank
// Leaf: ?PlaySoundW@CManager@NUISystem@@UAEXAAVCCharString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041D0A8, length 120 bytes, `ret 8` after `leave`.
// Name: Ego_r.pdb / FableWin.pdb `?PlaySoundW@CManager@NUISystem@@UAEXAAVCCharString@@J@Z` (the original method is PlaySound,
// renamed by the <windows.h> `PlaySound` -> `PlaySoundW` macro in its TU); slot 40 (+0xa0) of the NUISystem::CManager
// vtable 0x01230134, right before PlaySample (slot 41, 0x0041CEB3). The catalog's "CASound_PlayMessage" is descriptive.
// Body: nothing without the sound system (0x013B8394); else a CSoundInit (ctor 0x0041C930) with GainFaderBits (+0x14) =
// 0x20 and the string overload of the sound player's PlaySoundEvent (slot 4, +0x10; player = sound system slot 9, +0x24)
// with loop = true, attached = false, on the bank of the new front-end component (0x013B871C, +0x50) when it exists,
// else on the registry bank `bankId` (GetBank 0x00991840).
// Layout: CSoundInit 0x20 bytes (the landed CWorld::EASkipCutScene 0x00629F50 model); CNewFrontendGameComponent
// PSampleBank (a CCountedPointer<CASoundBank>) at +0x50 as in Ego_r.pdb.
// Shape: frame-pointer size TU (`optimize("s", on)` + `("y", off)`); the sound-system pointer is a named local kept in
// esi for the front-end player call and the GetBank call, while the in-game player call re-reads the global.
// Model: local -- engine/CManager.h and CDefinitionManager.h exist, but the generated header is data-only (pack(1), no
// member functions, no virtuals: the vptr and counted pointers are flattened into raw words); this body is a CManager virtual
// and calls virtuals, so both are local.
#pragma optimize("s", on)
#pragma optimize("y", off)
class CCharString {
public:
    void* PStringData;
};
class CASoundBank {
public:
    long GetSoundIndexFromSymbol(const CCharString& symbol);   // 0x00A01920
};
class CSoundInit { public:                                      // 0x20 bytes
    float         Volume;                                // +0x00
    float         Pitch;                                 // +0x04
    float         Pan;                                   // +0x08
    long          NoPlays;                               // +0x0c
    long          PlaybackType;                          // +0x10
    unsigned long GainFaderBits;                         // +0x14
    unsigned long PitchFaderBits;                        // +0x18
    long          OverrideID;                            // +0x1c
    CSoundInit();                                        // 0x0041C930
};
class CSoundPlayer {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual unsigned long PlaySoundEvent(CASoundBank& bank, const CCharString& name, bool loop, CSoundInit& init, bool attached);   // slot 4 (+0x10)
    virtual unsigned long PlaySoundEvent(CASoundBank& bank, long index, bool loop, CSoundInit& init, bool attached);               // slot 3 (+0x0c)
};
class CSoundSystem {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual CSoundPlayer* GetSoundPlayer();              // slot 9 (+0x24)
    CASoundBank* GetBank(long id);                       // 0x00991840 (CSoundBankRegistry::GetBank)
};
extern CSoundSystem* g_pSoundSystem;                     // 0x013B8394
class CNewFrontendGameComponent {
public:
    unsigned char _pad00[0x50];
    CASoundBank*  PSampleBank;                          // +0x50 CCountedPointer<CASoundBank>::Data (Ego_r.pdb PSampleBank)
};
extern CNewFrontendGameComponent* g_pNewFrontendGameComponent;   // 0x013B871C
class CDefinitionManager;
namespace NUISystem {
class CManager {
public:
    virtual void PlaySoundW(CCharString& symbol, long bankId);   // slot 40 (+0xa0)
    unsigned char       _pad04[0x64 - 4];
    CDefinitionManager* m_pDefinitionManager;            // +0x64
};
void CManager::PlaySoundW(CCharString& symbol, long bankId)
{
    CSoundSystem* soundSystem = g_pSoundSystem;
    if (soundSystem)
    {
        CSoundInit init;
        init.GainFaderBits = 0x20;
        if (g_pNewFrontendGameComponent)
        {
            CASoundBank* bank = g_pNewFrontendGameComponent->PSampleBank;
            if (bank)
                soundSystem->GetSoundPlayer()->PlaySoundEvent(*bank, symbol, true, init, false);
        }
        else
        {
            CASoundBank* bank = soundSystem->GetBank(bankId);
            if (bank)
                g_pSoundSystem->GetSoundPlayer()->PlaySoundEvent(*bank, symbol, true, init, false);
        }
    }
}
}