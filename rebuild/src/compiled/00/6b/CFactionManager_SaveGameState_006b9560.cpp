// CFactionManager::SaveGameState(CPersistContext&) const -- persist the faction count, then each faction under "Faction<index>"
// Leaf: ?SaveGameState@CFactionManager@@QBEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B9560, 168 bytes. Identity: Ego_r.pdb 0x006F5120 ?SaveGameState@CFactionManager@@QBEXAAVCPersistContext@@@Z
//   (engine/CFactionManager.h SAVEGAMESTATE).
// Callees: CPersistContext::Transfer<long>(name, value, default) @ 0x00410BE0, GFIntToCharString @ 0x0099F830, operator+(const char*,
//   const CCharString&) @ 0x0099F690, CCharString(const char*) @ 0x0099E4C0, CPersistContext::Transfer<CFaction>(name, value) @ 0x006BAED0
//   (Ego_r.pdb 0x006F6910 + FableWin.pdb 0x02446A80 ??$Transfer@VCFaction@@@CPersistContext@@QAEXPBDAAVCFaction@@@Z), ~CCharString @ 0x0099EAE0.
// Layout: engine/CFactionManager.h: Factions is the CVectorMap<unsigned long, CCountedPointer<CFaction> > (12-byte records: the CRC key
//   and the counted pointer).
// Name fix (agentNF55, 2026-11-17): moved onto engine/CFactionManager.h, engine/CPersistContext.h and engine/CCountedPointer.h; the local
//   CPersistContext / CFactionManager models with the made-up TransferFaction and the CountedFaction / FactionEntry stand-ins are gone.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CFACTIONMANAGER_SAVEGAMESTATE
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CPersistContext.h"
#include "engine/CFactionManager.h"
CCharString __fastcall GFIntToCharString(long value);                // 0x0099F830 (Ego_r.pdb ?GFIntToCharString@@YI?AVCCharString@@J@Z)

void CFactionManager::SaveGameState(CPersistContext& context) const
{
    long count = Factions.end() - Factions.begin();
    context.Transfer<long>("NumberOfFactions", count, 0);
    for (long i = 0; i < count; ++i)
        context.Transfer("Faction" + GFIntToCharString(i), *Factions.begin()[i].second);
}
