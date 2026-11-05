// CThing::SetToKillOnLevelUnload(bool kill) -- change the kill-on-level-unload flag; when it becomes set on a thing in limbo whose map is no longer loaded, kill the thing now
// Leaf: ?SetToKillOnLevelUnload@CThing@@QAEX_N@Z
// Naming wave 18: the global stand-in g_pThingManager (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline
//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C9C40, 89 bytes. Callees: the map manager's virtual slot 10 (+0x28,
// is-map-loaded query taking the map number) reached through the thing manager @ 0x013B8A1C (+0x28), and
// CThing::Kill(bool) @ 0x004C9B80.
// Shape: KillOnLevelUnload is bit 2 of +0x93 (compared as `shr dl,2 ; and dl,1 ; cmp dl,al` with the argument,
// then merge-stored and re-tested `test al,4`); InLimbo bit 5 of +0x91; CurrentMapNumber the short at +0x8e
// (`movsx`). Retail offsets are 0x10 below the PDB's.
struct CMapManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09();
    virtual bool IsMapLoaded(long mapNumber);        // slot 10 (+0x28)
};

class CThingManager { public:
    unsigned char _pad00[0x28];
    CMapManager*  m_pMapManager;                     // +0x28
};

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHING_MOREACCESS
#include "engine/CThing.h"

void CThing::SetToKillOnLevelUnload(bool kill)
{
    if (KillOnLevelUnload != kill) {
        KillOnLevelUnload = kill;
        if (KillOnLevelUnload && InLimbo && !CThing::GetPCurrentThingManager()->m_pMapManager->IsMapLoaded(CurrentMapNumber))
            Kill(false);
    }
}