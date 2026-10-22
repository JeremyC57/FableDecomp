// CCreatureAction_BanditKingLunge::FinishAction() -- base FinishAction; when the lunge ran its full time at a living target the bandit king gets stuck: mode 0x53, the stuck hit location on and the normal one off, a 1.5-radius obstruction half a facing ahead (ignoring the king), and a CCreatureAction_BanditKingStuck (boss def's stuck loop count) -- the obstruction is killed if that action is refused; otherwise a CCreatureAction_BanditKingUnstuck with no target
// Leaf: ?FinishAction@CCreatureAction_BanditKingLunge
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C8C30, length 830 bytes (+ int3 padding), virtual FinishAction, plain `ret`.
// Callees: CCombatActionBase::FinishAction @ 0x00859A20, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10,
//   CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 0x31 CTCCreatureModeManager bit 17 of +0x24, 0x7F CTCHitLocations
//   bit 31 of +0x2c, 0xAD CTCBoss bit 13 of +0x34, 0xED CTCDPhysicalObstruction bit 13 of +0x3c), CTCCreatureModeManager::AddMode
//   @ 0x0073AE10, CTCHitLocations::GetPHitLocationFromName @ 0x00815820 ("HIT_LOCATION_TWINBLADE_STUCK" 0x01281ACC,
//   "HIT_LOCATION_TWINBLADE" 0x01281AB4; implicit temporaries), CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, CHitLocation::SetEnabled
//   @ 0x00816390, CThing::GetPosition @ 0x004C73D0, the physics TC's slot 70 GetFacing (+0x118, hidden result), the `__fastcall`
//   CTCDPhysicalObstruction::Create @ 0x007FD4B0 (start and end both the offset position), SetThingToIgnoreObstruction @ 0x007898A3
//   (manifest "CTCTrapBase::SetThingAsImmune" is a folded twin), CCreatureAction_BanditKingStuck ctor @ 0x008C94D0,
//   CThingCreatureBase::SetCurrentAction @ 0x006644F0, CThing::Kill(bool) @ 0x004C9B80, CCombatActionBase(owner, long, const
//   CSoundPair&) @ 0x00858030 (action code 0x96), ~CCombatActionBase @ 0x0062E900. Constants: 0.5f @ 0x0122F59C, 1.5f pushed.
// Layout: retail CCombatActionBase is 0x114 bytes (Ego_r.pdb 0x12c): PCreature +0x08, TotalTime +0x10, FramesRunning +0x1c,
//   PTarget +0xa8. CCreatureAction_BanditKingStuck (0x128): TimesToLoop +0x114, PStuckEffect +0x118, PObstruction +0x120 (PDB -0x18);
//   CCreatureAction_BanditKingUnstuck (vtable 0x01281D9C): Knockdown +0x114, NoTarget +0x115. CTCBoss PBossDef +0x0c (PDB);
//   CBossDef TwinBladeStuckLoops +0x4c (PDB +0x58 less the three 12-byte STLport vectors). CThing: interface bitset +0x20, map
//   +0x44, PhysicsTC +0x60 (PDB +0x70), Dead bit +0x91.
// Shape: the target test is the inline IS_THING_ALIVE; the mode manager and hit-location lookups are the out-parameter
//   GetInterface with the result ignored (miss path reads the uninitialised out word); the boss loop count starts at 0 and is
//   read only on a hit. The offset is `position += GetPhysics()->GetFacing() * 0.5f` -- C3DVector needs a user copy ctor for
//   retail's x87 order (all three products first, Z parked in the frame, each sum consuming its product); the physics TC comes
//   through an inline GetPhysics() accessor (ecx loaded straight from +0x60). The stuck action is built in the SetCurrentAction
//   call after target, owner and self are fetched into named locals (GetPItem order PTarget, PCreature, PCreature), and the
//   refusal is stored as `bool refused = !...` before the obstruction test (neg/sbb/inc; no conditional-temporary flag word).
//   The unstuck branch is the landed WaspQueenDie temporary: inline ctor (base, vtable, NoTarget = true) in the call.
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CTCPhysicsBase {                                 // CThing::PhysicsTC (+0x60)
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing() const;                // slot 70 (+0x118)
};
struct CThing {
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];                   // +0x20
    CInterfaceMap   m_interfaces;                       // +0x44
    unsigned char   _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    CTCPhysicsBase* GetPhysics() const { return PhysicsTC; }
    void Kill(bool flag);                               // 0x004C9B80
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CCreatureActionBase;
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
struct CTCCreatureModeManager {                         // interface 0x31
    bool AddMode(unsigned long mode);                   // 0x0073AE10
};
struct CHitLocation {
    void SetEnabled(bool enabled);                      // 0x00816390
};
struct CTCHitLocations {                                // interface 0x7F
    CHitLocation* GetPHitLocationFromName(const CCharString& name);   // 0x00815820
};
struct CBossDef {
    unsigned char _pad[0x4c];
    long          TwinBladeStuckLoops;                  // +0x4c (Ego_r.pdb +0x58)
};
struct CTCBoss {                                        // interface 0xAD
    unsigned char _pad[0xc];
    CBossDef*     PBossDef;                             // +0x0c (CDefPointer<const CBossDef>)
};
struct CTCDPhysicalObstruction {                        // interface 0xED
    static CThing* __fastcall Create(const C3DVector& start, const C3DVector& end, float radius);   // 0x007FD4B0
    void SetThingToIgnoreObstruction(CThing* thing);    // 0x007898A3
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                                    // +0x04
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CCreaturePointer {                               // CIntelligentPointer<CThingCreatureBase>
    void*               __vftable;
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const;               // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CSoundPair {
    long First;
    long Second;
    CSoundPair() : First(0), Second(0) { }
};
struct CCreatureActionBase {
    virtual ~CCreatureActionBase();
};
struct CCombatActionBase : CCreatureActionBase {        // retail sizeof 0x114
    unsigned char           _pad04[4];
    CCreaturePointer        PCreature;                  // +0x08
    long                    TotalTime;                  // +0x10
    unsigned char           _pad14[0x1c - 0x14];
    long                    FramesRunning;              // +0x1c
    unsigned char           _pad20[0xa8 - 0x20];
    CBaseIntelligentPointer PTarget;                    // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    CCombatActionBase(CThingCreatureBase* owner, long animation, const CSoundPair& sounds = CSoundPair());   // 0x00858030
    ~CCombatActionBase();                               // 0x0062E900
    virtual void FinishAction();                        // 0x00859A20
};
struct CCreatureAction_BanditKingStuck : CCombatActionBase {     // retail sizeof 0x128
    long                    TimesToLoop;                // +0x114
    CBaseIntelligentPointer PStuckEffect;               // +0x118
    CBaseIntelligentPointer PObstruction;               // +0x120
    CCreatureAction_BanditKingStuck(CThingCreatureBase* owner, CThing* target, long timesToLoop, CThing* obstruction, CThing* stuckEffect);   // 0x008C94D0
};
struct CCreatureAction_BanditKingUnstuck : CCombatActionBase {   // vtable 0x01281D9C
    bool Knockdown;                                     // +0x114
    bool NoTarget;                                      // +0x115
    CCreatureAction_BanditKingUnstuck(CThingCreatureBase* owner, bool noTarget)
        : CCombatActionBase(owner, 0x96), NoTarget(noTarget)
    {
    }
    virtual void FinishAction();
};
struct CCreatureAction_BanditKingLunge : CCombatActionBase {
    virtual void FinishAction();
};

void CCreatureAction_BanditKingLunge::FinishAction()
{
    CCombatActionBase::FinishAction();
    if (IS_THING_ALIVE(PTarget) && FramesRunning >= TotalTime) {
        CTCCreatureModeManager* modeManager;
        PCreature.GetPItem()->GetInterface(&modeManager, 0x31);
        modeManager->AddMode(0x53);
        CTCHitLocations* hitLocations;
        PCreature.GetPItem()->GetInterface(&hitLocations, 0x7f);
        CHitLocation* stuck = hitLocations->GetPHitLocationFromName("HIT_LOCATION_TWINBLADE_STUCK");
        CHitLocation* normal = hitLocations->GetPHitLocationFromName("HIT_LOCATION_TWINBLADE");
        if (stuck)
            stuck->SetEnabled(true);
        if (normal)
            normal->SetEnabled(false);
        long stuckLoops = 0;
        CTCBoss* boss;
        if (PCreature.GetPItem()->GetInterface(&boss, 0xad))
            stuckLoops = boss->PBossDef->TwinBladeStuckLoops;
        C3DVector position = PCreature.GetPItem()->GetPosition();
        position += PCreature.GetPItem()->GetPhysics()->GetFacing() * 0.5f;
        CThing* obstruction = CTCDPhysicalObstruction::Create(position, position, 1.5f);
        if (obstruction && !(obstruction->m_flags91 & 1)) {
            CTCDPhysicalObstruction* physicalObstruction;
            obstruction->GetInterface(&physicalObstruction, 0xed);
            physicalObstruction->SetThingToIgnoreObstruction(PCreature.GetPItem());
        }
        CThing* target = PTarget.GetPItem();
        CThingCreatureBase* owner = PCreature.GetPItem();
        CThingCreatureBase* self = PCreature.GetPItem();
        bool refused = !self->SetCurrentAction(CCreatureAction_BanditKingStuck(owner, target, stuckLoops, obstruction, 0));
        if (refused && obstruction && !(obstruction->m_flags91 & 1))
            obstruction->Kill(true);
    }
    else {
        PCreature.GetPItem()->SetCurrentAction(CCreatureAction_BanditKingUnstuck(PCreature.GetPItem(), true));
    }
}