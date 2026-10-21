// Grade: DIFFER(379v379) -- /GS; the filter temporary and the out pointer swap frame slots (retail: filter +0x10, creature +0xC shared with the float payload temps) and the creature is loaded into esi before the GetWorld call instead of after
// CTCInventoryMap::TeleportHeroToBoastingArea() -- while the hero creature is alive and an AI creature with property 0x72 (the boasting-area marker) exists, show the mini map on the region load and queue a move-hero-to-region game event (0x2C) carrying that creature's position and facing angle, flags all false, from the main player
// Leaf: TeleportHeroToBoastingArea
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FB4B0, 384 bytes, `ret`, esp frame (speed TU), /GS (cookie
//   0x0139C8A8, __security_check_cookie @ 0x00BFE9F9).
// Callees: CTCBase::GetPlayerManager() const @ 0x00686D50, CPlayerManager::GetMainPlayer @ 0x00449960,
//   CPlayer::GetControlledCreature @ 0x00487DD0 (as the landed sibling TeleportHeroToGuild 0x005FBD90), CTCBase::GetWorld
//   @ 0x00686A70 and its const twin @ 0x00686A80, CMemoryBuffer::Copy(unsigned long, const void*) @ 0x009F1760,
//   CPlayerManager::GetMainPlayerID @ 0x00449700 (engine 0x013B86A0 +0x1C), CWorld::HandleMoveHeroToRegionGameEvent
//   @ 0x0049EAF0 (landed; it reads X, Y, Z, angle and three flag bytes in this order),
//   CThingSearchTools::GetPFirstLeafThing<CThingFilter_AICreatureHasProperty, CThingAICreature> @ 0x006156E0,
//   CThing::GetPosition @ 0x004C73D0 (called once per coordinate), the physics component's slot 63 GetAngleXY (+0xFC).
// Shape: the sibling TeleportHeroToGuildSealRecallLocation 0x005FAD40 with the payload read from the found creature;
//   the filter temporary is built before the const GetWorld call; the found creature is read back from its out slot.
// Layout: retail CWorld: ThingSearchTools +0x20, DisplayMiniMapOnRegionLoad +0x12F (Ego_r.pdb order, 16 bytes less);
//   CGameEvent as the landed CInputProcessFishing 0x00690B20; retail CThing PPhysics +0x60, dead bit +0x91.
struct C3DVector { float X, Y, Z; };
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddFloat(float value) { Copy(sizeof(value), &value); }
    void AddBool(bool value) { Copy(1, &value); }
};

struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetAngleXY() const;                 // slot 63 (+0xFC)
};
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PPhysics;                         // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char IsDead : 1;                         // +0x91 bit 0
    const C3DVector& GetPosition() const;             // 0x004C73D0
};
struct CThingAICreature : CThing { };
struct CThingFilter_AICreatureHasProperty {
    long Property;
    CThingFilter_AICreatureHasProperty(long property) : Property(property) { }
};
struct CThingSearchTools {
    template <class F, class T> bool GetPFirstLeafThing(const F& filter, T** out) const;   // 0x006156E0
};
struct CPlayer {
    CThing* GetControlledCreature() const;            // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;                   // 0x00449960
    long GetMainPlayerID() const;                     // 0x00449700
};
struct CEngine {
    unsigned char _pad00[0x1c];
    CPlayerManager* PlayerManager;                    // +0x1C
};
extern CEngine* g_pEngine;                            // 0x013B86A0
struct CWorld {
    unsigned char _pad000[0x20];
    CThingSearchTools* ThingSearchTools;              // +0x20
    unsigned char _pad024[0x12f - 0x24];
    bool DisplayMiniMapOnRegionLoad;                  // +0x12F
    void HandleMoveHeroToRegionGameEvent(const CGameEvent* event);   // 0x0049EAF0
};

struct CTCInventoryMap {
    CPlayerManager* GetPlayerManager() const;         // 0x00686D50 (CTCBase)
    CWorld* GetWorld();                               // 0x00686A70 (CTCBase)
    const CWorld* GetWorld() const;                   // 0x00686A80 (CTCBase)
    const CWorld& GetWorldConst() const { return *GetWorld(); }
    template <class F, class T> __forceinline bool GetPFirstLeafThing(const F& filter, T** out) const
    {
        const CThingSearchTools* tools = GetWorld()->ThingSearchTools;
        return tools->GetPFirstLeafThing(filter, out);
    }
    void TeleportHeroToBoastingArea();
};

void CTCInventoryMap::TeleportHeroToBoastingArea()
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetControlledCreature();
    if (hero && !hero->IsDead) {
        CThingAICreature* marker;
        if (GetPFirstLeafThing(CThingFilter_AICreatureHasProperty(0x72), &marker)) {
            CThingAICreature* boaster = marker;
            GetWorld()->DisplayMiniMapOnRegionLoad = true;
            CGameEvent event(0x2c);
            event.AddFloat(boaster->GetPosition().X);
            event.AddFloat(boaster->GetPosition().Y);
            event.AddFloat(boaster->GetPosition().Z);
            event.AddFloat(boaster->PPhysics->GetAngleXY());
            event.AddBool(false);
            event.AddBool(false);
            event.AddBool(false);
            event.Player = (signed char)g_pEngine->PlayerManager->GetMainPlayerID();
            GetWorld()->HandleMoveHeroToRegionGameEvent(&event);
        }
    }
}
