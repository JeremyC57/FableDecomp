// CASoundManager::CloseAllBanks() -- empty the sound manager's registered-bank map (destroy every {id, bank record} entry; the storage is kept)
// Leaf: ?CloseAllBanks@CASoundManager@@QAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009918F0, built /Gr.
// Name (agentNF3, 2026-11-10): Fable Anniversary 360 PDB public ?CloseAllBanks@CASoundManager@@QAAXXZ (lib_sound_manager_base.cpp, after
//   ~CASoundManager in the same TU order as TLC) + the TLC body (erases every record); engine/CASoundManager.h BANKLOOKUP declares it.
//   Was the descriptive ClearBanks. The record keeps its landed descriptive name CSoundBankRecord here: its erase 0x00993030, dtor
//   0x00992510 and operator= 0x00992630 are bound under it by ~20 landed rows (the Anniversary name CASoundManager::CRegisteredBank
//   is a separate respell wave, NOTES). Banks CVectorMap<long,CSoundBankRecord> at +0x30.
// Shape: `Banks.clear();` (engine/CVectorMap.h).
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
#include "engine/CVectorMap.h"

class CSoundBankRecord {                                     // the landed 0x00992510 / 0x00992630 record (0x20 bytes; descriptive)
public:
    unsigned char Data[0x20];
    CSoundBankRecord& operator=(const CSoundBankRecord& other);   // 0x00992630
    ~CSoundBankRecord();                                     // 0x00992510
};
class CResourceBank;

class CASoundManager : public CBaseClassNonCopyable {        // vtable 0x0129A574 (RTTI CASoundManager : CBaseClassNonCopyable)
public:
    virtual ~CASoundManager();                               // 0x00991880
    void CloseAllBanks();
    unsigned char                           Settings[0x24];  // +0x04 .. +0x27 (the landed ctor 0x00991910's settings block)
    CCountedPointer<CResourceBank>          ResourceBank;    // +0x28
    CVectorMap<long, CSoundBankRecord>      Banks;           // +0x30
};
FABLE_STATIC_ASSERT(sizeof(CASoundManager) == 0x40);

void CASoundManager::CloseAllBanks()
{
    Banks.clear();
}