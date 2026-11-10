// NUISystem::CManager::PlaySample(CCharString& symbol, long bankId) -- play a UI sound sample by symbol from the given sound bank; returns the sound handle, 0 when nothing played
// Leaf: ?PlaySample@CManager@NUISystem@@UAEKAAVCCharString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041CEB3, length 108 bytes, `ret 8` after `leave`.
// Sound manager (agentSND2, 2026-11-10): PSoundManager of engine/CASoundManager.h -- GetPSoundBank(long) const 0x00991840
//   (FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP, Anniversary name; landed as CASoundManager::GetBank), slot 9 Get2DEmitter and the
//   emitter's PlaySample (slot 3) / PlaySoundEvent (slot 4); engine/CSoundInit.h; was a local CASoundManager / CSoundPlayer / CSoundInit model.
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
public:
    long GetSoundIndexFromSymbol(const CCharString&) const;   // 0x00A01920
};
#define FABLE_RETAIL_CASOUNDMANAGER_BANKLOOKUP
#include "engine/CSoundInit.h"
#include "engine/CASoundManager.h"
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
    if (PSoundManager && !m_pDefinitionManager)
    {
        CASoundBank* bank = PSoundManager->GetPSoundBank(bankId);
        if (bank)
        {
            long index = bank->GetSoundIndexFromSymbol(symbol);
            if (index)
            {
                CSoundInit init;
                init.GainFaderBits = 0x40;
                return PSoundManager->Get2DEmitter().PlaySample(*bank, index, true, init, false);
            }
        }
    }
    return 0;
}
}