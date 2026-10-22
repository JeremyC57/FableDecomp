// CGameScriptInterface::EntityTeleportToPosition(const CScriptThing& thing, const C3DVector& pos, float angle, bool keepOnGround, bool fade) const -- teleport a scripted thing: the hero across maps through a move-hero-to-region game event, otherwise place it directly (snapping a walker onto walkable ground, resetting perception and the camera) and set its heading
// Leaf: ?EntityTeleportToPosition@CGameScriptInterface
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089B780, 943 bytes, /GS (cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9), `ret 0x14`.
// Callees: CWorldMap::GetRegionNumberMapIsIn 0x004FC190 / StartMovePlayerPos 0x00502500 / 0x005064B0 (manifest label propagated), CMemoryBuffer::Copy
//   0x009F1760 (CGameEvent adds inlined), CWorld::HandleMoveHeroToRegionGameEvent 0x0049EAF0, component-map LowerBound 0x0040F020, walk component
//   0x006CB6B0 / 0x006CB8E0 (manifest "CScriptThing::operator==" propagated), CThing::SetAsGameStatePersisted 0x00661EA0 / SetInLimbo 0x004C8CF0,
//   CTCPerceiveThing::ResetPerceptionVariables 0x00744DB0, CGameCameraManager::ResetCamera 0x0069A990, CTCCreatureGenerator::InitialiseCreatureGenerator
//   0x0074B770, CTCCarrying::UpdateCarriedObjects 0x006A1D60; game-interface slots 418 (+0x688) and 474 (+0x768).
// Layout: the landed MakeHeroCarryItemInHand / SetHeroHandLampAsLit thing model (component mask +0x20, map +0x44, dead bit +0x91); CGameEvent per Ego_r.pdb (landed 0x00690C70).
// Shape: the walk-component lookup (kind 7) uses the ternary entry pick so the component loads straight into ecx; the other lookups use the eax join.
extern "C" int __cdecl puts(const char* text);
extern "C" __declspec(noreturn) void __cdecl abort();
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);         // 0x0040F020 (CVectorMap<ETCInterfaceType,...>::LowerBound; the key by address)
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase_Retail* Find(long kind)          // the inlined map `find`: lower bound (the key in a stack temporary), end when the key is absent
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
    __forceinline CTCBase_Retail* FindPick(long kind)      // the ternary entry pick (component loaded straight into the use register)
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        return (it == m_pLast || kind < it->m_kind ? m_pLast : it)->m_pTC;
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CThing_Retail {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                         // +0x10 (1 = a creature)
    unsigned char  _pad11[0xf];
    unsigned long  m_tcMask[9];                    // +0x20: which component kinds the thing holds (bitset<274>)
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x3c];
    unsigned short m_id;                           // +0x8c
    unsigned char  _pad8e[3];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    void SetInLimbo(bool limbo);                   // 0x004C8CF0
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }   // the masked word
    __forceinline bool TestComponent(unsigned long position) const    // STLport bitset<274>::test with its range check
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (m_tcMask[position >> 5] & (1UL << (position & 31))) != 0;
    }
};
__declspec(noinline) STCEntry_Retail* CTCMap_Retail::LowerBound(const unsigned long& kind)
{
    STCEntry_Retail* it = m_pFirst;
    while (it != m_pLast && (unsigned long)it->m_kind < kind)
        ++it;
    return it;
}
__declspec(noinline) STCEntry_Retail* CTCMap_Retail::LowerBound(const unsigned long& kind) const
{
    STCEntry_Retail* it = m_pFirst;
    while (it != m_pLast && (unsigned long)it->m_kind < kind)
        ++it;
    return it;
}
__forceinline CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
__forceinline CTCBase_Retail* GetTCConst(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }

struct C3DVector { float X, Y, Z; C3DVector() { } };
#define VPAD1(a) virtual void a();
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
struct CScriptThing {
    VPAD4(a) VPAD2(b)                              // 0..5
    virtual const C3DVector& GetPosition() const;  // slot 6 (+0x18)
    VPAD4(c)                                       // 7..10
    virtual CThing_Retail* GetThing() const;       // slot 11 (+0x2c)
    VPAD32(d) VPAD16(e) VPAD8(f) VPAD4(g) VPAD2(h) VPAD1(i)   // 12..74
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
};
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
struct CWorldMap {
    VPAD16(a)                                      // 0..15
    virtual long GetRegionAt(const C3DVector& pos);   // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long map);         // 0x004FC190
    void StartMovePlayerPos(long map, const C3DVector& pos, long loadType);   // 0x00502500
    void FinishMovePlayer();                       // 0x005064B0
};
struct CPhysicsObject {
    VPAD16(a) VPAD8(b) VPAD4(c) VPAD2(d) VPAD1(e)  // 0..30
    virtual void SetPosition(const C3DVector& pos);   // slot 31 (+0x7c)
};
struct CTCWalk {
    void Reset();                                  // 0x006CB6B0
    bool FindNearestWalkablePos(const C3DVector& pos, float radius, C3DVector* out);   // 0x006CB8E0
};
struct CTCPerceiveThing { void ResetPerceptionVariables(); };   // 0x00744DB0
struct CTCCreatureGenerator {
    unsigned char _pad[0xa1];
    bool          Teleported;                      // +0xa1
    void InitialiseCreatureGenerator(bool flag);   // 0x0074B770
};
struct CTCCarrying { void UpdateCarriedObjects(); };   // 0x006A1D60
struct CGameCameraManager { void ResetCamera(); };    // 0x0069A990
struct CCameraOwner {
    VPAD8(a) VPAD1(b)                              // 0..8
    virtual bool GetCameraManager(CGameCameraManager** out);   // slot 9 (+0x24)
};
struct CTeleportThing {
    unsigned char   _pad[0xc];
    unsigned char   Type;                          // +0x10
    unsigned char   _pad11[0xf];
    signed char     TCMask20;                      // +0x20
    unsigned char   _pad21[3];
    unsigned long   Flags24;                       // +0x24
    unsigned long   Flags28;                       // +0x28
    unsigned char   _pad2c[0x34];
    CPhysicsObject* Physics;                       // +0x60
    unsigned char   _pad64[8];
    unsigned char   Flags6c;                       // +0x6c
    unsigned char   _pad6d[0x23];
    signed char     PlayerNumber;                  // +0x90
    VPAD64(a) VPAD8(b) VPAD4(c) VPAD1(d)           // 0..76
    virtual bool IsCreature();                     // slot 77 (+0x134)
    void SetAsGameStatePersisted(bool persisted);  // 0x00661EA0
    void SetInLimbo(bool limbo);                   // 0x004C8CF0
};
struct CWorld {
    VPAD8(a) VPAD4(b)                              // 0..11
    virtual CWorldMap* GetWorldMap();              // slot 12 (+0x30)
    unsigned char   _pad04[0x30];
    CCameraOwner*   CameraOwner;                   // +0x34
    unsigned char   _pad38[0xf7];
    bool            AllowRegionLoad;               // +0x12f
    void HandleMoveHeroToRegionGameEvent(const CGameEvent& event);   // 0x0049EAF0
};
struct CGameScriptInterface {
    VPAD1(a)
    CWorld* World;                                 // +0x04
    VPAD256(b) VPAD128(c) VPAD32(d) VPAD1(e)        // 1..417
    virtual bool IsCameraInScriptedMode();         // slot 418 (+0x688)
    VPAD32(f) VPAD16(g) VPAD4(h) VPAD2(i) VPAD1(j) // 419..473
    virtual void SetThingHeading(const CScriptThing& thing, float angle, bool immediate) const;   // slot 474 (+0x768)
    virtual void EntityTeleportToPosition(const CScriptThing& thing, const C3DVector& pos, float angle, bool keepOnGround, bool fade) const;
};
#define T(x) ((CTeleportThing*)(x))

void CGameScriptInterface::EntityTeleportToPosition(const CScriptThing& thing, const C3DVector& pos, float angle, bool keepOnGround, bool fade) const
{
    if (!thing.IsValid())
        return;
    CThing_Retail* object = thing.GetThing();
    CWorldMap* map = World->GetWorldMap();
    long fromRegion = map->GetRegionAt(thing.GetPosition());
    long toRegion = World->GetWorldMap()->GetRegionAt(pos);
    long fromMap = World->GetWorldMap()->GetRegionNumberMapIsIn(fromRegion);
    long toMap = World->GetWorldMap()->GetRegionNumberMapIsIn(toRegion);
    if (toMap <= 0)
        return;
    if (T(object)->Type == 2 && fromMap != toMap) {
        World->AllowRegionLoad = false;
        CGameEvent event(0x2c);
        event.AddFloat(pos.X);
        event.AddFloat(pos.Y);
        event.AddFloat(pos.Z);
        event.AddFloat(angle);
        event.AddBool(fade);
        event.AddBool(true);
        event.AddBool(false);
        event.Player = T(thing.GetThing())->PlayerNumber;
        World->HandleMoveHeroToRegionGameEvent(event);
    } else if (T(object)->Flags6c & 8) {
        CTCWalk* walk = 0;
        if (object->HasTC(7)) {
            walk = (CTCWalk*)object->m_tcs.FindPick(7);
            walk->Reset();
        }
        T(object)->SetAsGameStatePersisted(false);
        bool creature = T(object)->IsCreature();
        if (creature) {
            T(object)->SetInLimbo(true);
            World->GetWorldMap()->StartMovePlayerPos(T(object)->PlayerNumber, pos, 0);
        }
        CPhysicsObject* physics = T(object)->Physics;
        physics->SetPosition(pos);
        if (keepOnGround && walk) {
            C3DVector ground;
            if (walk->FindNearestWalkablePos(pos, 0.8f, &ground))
                physics->SetPosition(ground);
        }
        if (creature) {
            World->GetWorldMap()->FinishMovePlayer();
            T(object)->SetInLimbo(false);
        }
        if (T(object)->Flags24 & 0x100000)
            ((CTCPerceiveThing*)GetTC(object, 0x34))->ResetPerceptionVariables();
        if (creature && !const_cast<CGameScriptInterface*>(this)->IsCameraInScriptedMode()) {
            CGameCameraManager* camera = 0;
            if (World->CameraOwner->GetCameraManager(&camera))
                camera->ResetCamera();
        }
        SetThingHeading(thing, angle, true);
    } else {
        if (T(object)->Flags24 & 0x20000000) {
            CTCCreatureGenerator* generator = (CTCCreatureGenerator*)GetTC(object, 0x3d);
            generator->InitialiseCreatureGenerator(true);
            generator->Teleported = true;
        }
        T(object)->Physics->SetPosition(pos);
        SetThingHeading(thing, angle, true);
    }
    if (T(object)->Flags28 & 0x40)
        ((CTCCarrying*)GetTC(object, 0x46))->UpdateCarriedObjects();
}