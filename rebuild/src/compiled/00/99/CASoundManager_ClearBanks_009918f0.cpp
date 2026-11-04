// CASoundManager::ClearBanks() -- empty the sound manager's bank map (destroy every {id, bank record} entry; the storage is kept)
// Leaf: ?ClearBanks@CASoundManager@@QAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009918F0, built /Gr.
// Identity: descriptive name (no PDB symbol at this body); Banks CVectorMap<long,CSoundBankRecord> at +0x30.
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
    void ClearBanks();
    unsigned char                           Settings[0x24];  // +0x04 .. +0x27 (the landed ctor 0x00991910's settings block)
    CCountedPointer<CResourceBank>          ResourceBank;    // +0x28
    CVectorMap<long, CSoundBankRecord>      Banks;           // +0x30
};
FABLE_STATIC_ASSERT(sizeof(CASoundManager) == 0x40);

void CASoundManager::ClearBanks()
{
    Banks.clear();
}