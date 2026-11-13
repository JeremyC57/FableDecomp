#ifndef FABLE_ENGINE_CFACTIONMANAGER_H
#define FABLE_ENGINE_CFACTIONMANAGER_H
// Original PDB complete member set; native CVectorMap16-byte layout.
#include "engine/CVectorMap.h"
#include "engine/CCountedPointer.h"
class CFaction;
class CFactionManager {
public:
#ifdef FABLE_RETAIL_CFACTIONMANAGER_BYCRC
    // Opt-in (w22 agent2007, row 0x006DEEE0 CTCTrophy::GlobalUpdate): Ego_r.pdb / FableWin.pdb ?GetPFaction@CFactionManager@@QBEPAVCFaction@@K@Z
    // (out of line, 0x006B93A0; its only caller GlobalUpdate passes `name.GetCRC()`: `call CCharString::GetCRC 0x00404310 ; push eax ;
    // call 0x006B93A0` at 0x006DF14E). Declaration only.
    CFaction* GetPFaction(unsigned long crc) const;                      // 0x006B93A0
#endif
    CFaction* GetPFaction(const CCharString& name) const;
private:
    typedef CVectorMap<unsigned long,CCountedPointer<CFaction>,CKeyPairCompareLess<unsigned long,CCountedPointer<CFaction> > > TFactionMap;
    TFactionMap Factions;
};
FABLE_STATIC_ASSERT(sizeof(CFactionManager)==16);
#endif
