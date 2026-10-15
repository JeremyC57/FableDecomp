// CThing::SetForceFrameUpdate(bool force) -- set the force-frame-update flag; when it ends up set on a live thing, put the thing on the thing manager's update list
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7C60, 58 bytes. Callees: CThingManager::AddToUpdateList(CThing*)
// @ 0x0051EC50 on the global thing manager @ 0x013B8A1C (tail jump, `this` stored over the argument slot).
// Shape: ForceFrameUpdate is bit 3 of +0x92 (merge store, then the stored byte re-tested: `mov al,dl ; test al,8`);
// Dead is bit 0 of +0x91.
struct CThing;

struct CThingManager {
    void AddToUpdateList(CThing* thing);             // 0x0051EC50
};

extern CThingManager* g_pThingManager;              // 0x013B8A1C

struct CThing {
    void*         __vftable;                         // +0x00
    unsigned char _pad04[0x91 - 0x04];
    unsigned char Dead : 1;                          // +0x91 bit 0 (PDB +0xa1)
    unsigned char CreatedByOwnerThingCreation : 1;   // bit 1
    unsigned char IncludeInThingScans : 1;           // bit 2
    unsigned char Activated : 1;                     // bit 3
    unsigned char GloballyLoaded : 1;                // bit 4
    unsigned char InLimbo : 1;                       // bit 5
    unsigned char KillLocked : 1;                    // bit 6
    unsigned char CreatedBySerialisation : 1;        // bit 7
    unsigned char DeletedOnOwnerLevelOutOfScope : 1; // +0x92 bit 0
    unsigned char Drawable : 1;                      // bit 1
    unsigned char Dying : 1;                         // bit 2
    unsigned char ForceFrameUpdate : 1;              // bit 3
    void SetForceFrameUpdate(bool force);
};

void CThing::SetForceFrameUpdate(bool force)
{
    ForceFrameUpdate = force;
    if (ForceFrameUpdate && !Dead)
        g_pThingManager->AddToUpdateList(this);
}
