// CTCTeleporter::TeleportHero() -- flags the region load as a teleport, then hands the world a move-hero game event (0x2c) carrying the teleporter's position, facing and three flags (true, false, false) for the main player
// Leaf: TeleportHero
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DFCF0, 295 bytes (plain `ret`, /GS cookie 0x0139C8A8).
// Callees: CTCBase::GetWorld() 0x00686A70 (x2); CThing::GetPosition() const 0x004C73D0 (x3, one per component);
//          CMemoryBuffer::Copy(unsigned long, const void*) 0x009F1760 (x7); the physics component's GetFacingAngleXY (slot
//          63, `call [edx+0FCh]`); CTCBase::GetPlayerManager() const 0x00686D50; CPlayerManager::GetMainPlayerNumber()
//          0x00449700; CWorld::HandleMoveHeroToRegionGameEvent(CGameEvent*) 0x0049EAF0; __security_check_cookie 0x00BFE9F9.
// Layout (retail): CWorld::RegionLoadViaTeleporter +0x12f (Ego_r.pdb +0x13c: the region-load block sits 0xd below the
//          PDB); CThing physics component +0x60; CGameEvent as the landed CTCHeroCentre::OpenTrainingScreen 0x0077D4B0
//          (Type +0, Player +4, Data +5, EndPos +0x25, Valid +0x26, Replacement +0x27).
// Shape: the payload goes in through inline by-value AddFloat/AddBool helpers (each value homed in its own frame word,
//        the float ones sharing one, the bool ones another); the false bool shares the bl zero with the event's
//        EndPos/Replacement init; the position is re-fetched for each component.
class CThing;
struct C3DVector { float X, Y, Z; };

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
    virtual float GetFacingAngleXY();           // slot 63 (+0xFC)
};
class CThing {
public:
    unsigned char   _pad_0x00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);  // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                         // +0x00
    signed char   Player;                       // +0x04
    unsigned char Data[32];                     // +0x05
    unsigned char EndPos;                       // +0x25
    bool          Valid;                        // +0x26
    bool          Replacement;                  // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddFloat(float value) { Copy(4, &value); }
    void AddBool(bool value) { Copy(1, &value); }
};

struct CWorld {
    unsigned char _pad_0x00[0x12f];
    bool          RegionLoadViaTeleporter;      // +0x12f
    void HandleMoveHeroToRegionGameEvent(CGameEvent* event);   // 0x0049EAF0
};
struct CPlayerManager {
    long GetMainPlayerNumber();                 // 0x00449700
};

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool ToBeRemoved;                           // +0x08
    bool IsInGlobalUpdateTCs;                   // +0x09
    unsigned char _pad0a[2];
    CWorld* GetWorld();                         // 0x00686A70
    CPlayerManager* GetPlayerManager() const;   // 0x00686D50
};
struct CTCTeleporter : CTCBase {
    void TeleportHero();
};

void CTCTeleporter::TeleportHero()
{
    GetWorld()->RegionLoadViaTeleporter = true;
    CGameEvent event(0x2c);
    event.AddFloat(ParentThing->GetPosition().X);
    event.AddFloat(ParentThing->GetPosition().Y);
    event.AddFloat(ParentThing->GetPosition().Z);
    event.AddFloat(ParentThing->PhysicsTC->GetFacingAngleXY());
    event.AddBool(true);
    event.AddBool(false);
    event.AddBool(false);
    event.Player = (signed char)GetPlayerManager()->GetMainPlayerNumber();
    GetWorld()->HandleMoveHeroToRegionGameEvent(&event);
}