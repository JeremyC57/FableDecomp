// CThing::SetToKillOnLevelUnload(bool kill) -- change the kill-on-level-unload flag; when it becomes set on a thing in limbo whose map is no longer loaded, kill the thing now
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

struct CThingManager {
    unsigned char _pad00[0x28];
    CMapManager*  m_pMapManager;                     // +0x28
};

extern CThingManager* g_pThingManager;              // 0x013B8A1C

struct CThing {
    void*         __vftable;                         // +0x00
    unsigned char _pad04[0x8e - 0x04];
    short         CurrentMapNumber;                  // +0x8e (PDB +0x9e)
    signed char   PlayerNumber;                      // +0x90
    unsigned char Dead : 1;                          // +0x91 bit 0
    unsigned char CreatedByOwnerThingCreation : 1;   // bit 1
    unsigned char IncludeInThingScans : 1;           // bit 2
    unsigned char Activated : 1;                     // bit 3
    unsigned char GloballyLoaded : 1;                // bit 4
    unsigned char InLimbo : 1;                       // bit 5
    unsigned char KillLocked : 1;                    // bit 6
    unsigned char CreatedBySerialisation : 1;        // bit 7
    unsigned char _flags92;                          // +0x92
    unsigned char NotPausedDuringCutscenes : 1;      // +0x93 bit 0
    unsigned char DeathPersistent : 1;               // bit 1
    unsigned char KillOnLevelUnload : 1;             // bit 2
    void Kill(bool immediately);                     // 0x004C9B80
    void SetToKillOnLevelUnload(bool kill);
};

void CThing::SetToKillOnLevelUnload(bool kill)
{
    if (KillOnLevelUnload != kill) {
        KillOnLevelUnload = kill;
        if (KillOnLevelUnload && InLimbo && !g_pThingManager->m_pMapManager->IsMapLoaded(CurrentMapNumber))
            Kill(false);
    }
}