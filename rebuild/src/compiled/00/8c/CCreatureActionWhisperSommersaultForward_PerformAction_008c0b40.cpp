// CCreatureAction_WhisperSommersaultForward::PerformAction() -- if the creature's flag bit 13 at +0x28 is set, notify its property 77 changed; then mark the action finished
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C0B40, length 48 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, fetched twice), CBaseObject::OnPropertyChanged(long) @ 0x004C9840 (property 0x4d = 77).
// Shape: the intelligent pointer's address is kept in edi for both GetPItem calls; `test ch,20h` is bit 13 of the
// creature's flag word at +0x28; the Finished byte at +0x114 (retail CCombatActionBase is 0x114 bytes) is stored
// after the branch, between the two pops. PerformAction has no arguments (`ret`).
class CBaseObject { public:
    unsigned char _pad[0x28];
    unsigned long m_flags28;                   // +0x28: bit 13 tested here
    void OnPropertyChanged(long propertyId);   // 0x004C9840
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CCreatureAction_WhisperSommersaultForward { public:   // CCombatActionBase (0x114 bytes) + Finished
    virtual void v00();
    unsigned char           _pad04[4];         // +0x04
    CBaseIntelligentPointer PCreature;         // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char           _pad10[0x114 - 0x10];
    unsigned char           Finished;          // +0x114
    void PerformAction();
};

void CCreatureAction_WhisperSommersaultForward::PerformAction()
{
    if (PCreature.GetPItem()->m_flags28 & 0x2000)
        PCreature.GetPItem()->OnPropertyChanged(77);
    Finished = 1;
}