// NUISystem::CManager::PlaySample(CCharString& symbol, long bankId) -- play a UI sound sample by symbol from the given sound bank; returns the sound handle, 0 when nothing played
// Leaf: ?PlaySample@CManager@NUISystem@@UAEKAAVCCharString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041CEB3, length 108 bytes, `ret 8` after `leave`.
// Name: Ego_r.pdb / FableWin.pdb `?PlaySample@CManager@NUISystem@@UAEKAAVCCharString@@J@Z`; it is slot 41 (+0xa4) of the
// NUISystem::CManager vtable 0x01230134 (rtti-vtables.tsv), right after PlaySound (slot 40, 0x0041D0A8). The catalog's
// "CASound_PlayFromSymbol" is descriptive.
// Body: nothing (0) without the sound system (0x013B8394) or while the manager holds a definition manager (+0x64);
// else the bank from the sound system's registry (GetBank 0x00991840), the sample index from the bank's symbol table
// (CASoundBank::GetSoundIndexFromSymbol 0x00A01920), and for a valid index a CSoundInit (ctor 0x0041C930) with
// GainFaderBits (+0x14) = 0x40 handed to the sound player's index overload of PlaySoundEvent (slot 3, +0x0c; the player
// is the sound system's slot 9, +0x24) with loop = true and attached = false.
// Layout: retail NUISystem::CManager m_pDefinitionManager +0x64 (as the landed GetIconsDef 0x0041D98A / CManager ctor
// 0x0041E3F6); CSoundInit 0x20 bytes (the landed CWorld::EASkipCutScene 0x00629F50 model).
// Shape: frame-pointer size TU (`optimize("s", on)` + `("y", off)`); the sound-system pointer is tested first and reused
// in eax for GetBank, then re-read for the player after the CSoundInit constructor; one shared `xor eax,eax` exit.
// The player's PlaySoundEvent overloads are declared string-first so the index overload lands in slot 3 (VC lays
// overloaded virtuals out in reverse declaration order).
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
struct CSoundInit {                                      // 0x20 bytes
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
class CDefinitionManager;
namespace NUISystem {
class CManager {
public:
    virtual unsigned long PlaySample(CCharString& symbol, long bankId);   // slot 41 (+0xa4)
    unsigned char       _pad04[0x64 - 4];
    CDefinitionManager* m_pDefinitionManager;            // +0x64
};
unsigned long CManager::PlaySample(CCharString& symbol, long bankId)
{
    if (g_pSoundSystem && !m_pDefinitionManager)
    {
        CASoundBank* bank = g_pSoundSystem->GetBank(bankId);
        if (bank)
        {
            long index = bank->GetSoundIndexFromSymbol(symbol);
            if (index)
            {
                CSoundInit init;
                init.GainFaderBits = 0x40;
                return g_pSoundSystem->GetSoundPlayer()->PlaySoundEvent(*bank, index, true, init, false);
            }
        }
    }
    return 0;
}
}