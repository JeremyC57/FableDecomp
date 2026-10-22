// Grade: DIFFER(825v825) -- scheduling only: in both copies of the warning-window test retail calls GetPThingWorld (its result dropped) before loading g_FramesPerSecond into ebx, ours loads the frames-per-second local first; next: `CWorld* world = GetPThingWorld(); long fps = g_FramesPerSecond; return world->GetWorldFrame() - FrameOfLastWarning < (unsigned long)(fps * 4);` in IsWithinWarningTime; 4 grades spent
// CAIStateGroup_MinionGuardReactToPlayerWithWeapon::Update() -- face the armed player; when another guard is already admonishing, keep watching until the player disarms; otherwise post a numbered warning, follow and watch the player for four seconds, and on the third warning declare the player a criminal
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093A080, length 832 bytes (+ int3 padding), plain `ret`, frame 0xb8.
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90, IsPlayerCarryingWeapon
// @ 0x00939E70, IsAnotherGuardAdmonishingPlayer @ 0x00939D00, CheckForOtherGuardsNewWarning @ 0x00939D70, the own-warning
// removal @ 0x00939DD0 (manifest "CheckAndRemove"), AIAction_LookAtThing @ 0x0097D6F0, AIAction_MoveToThingProximity
// @ 0x0097E380 (its pointer argument built by the out-of-line ctor @ 0x00930D20), CAIStateGroupBase::GetPThingWorld
// @ 0x008FCFA0 / GetPWorld @ 0x008FCFC0 / Terminate @ 0x008FCF40, GFGetCurrentFrame @ 0x0049D870,
// CThingCreatureBase::GetPosition @ 0x006A4D00, CThing::GetPosition @ 0x004C73D0, CMessageEventManager::AddMessage
// @ 0x00687540, CCreatureAction_DeclarePlayerACriminal ctor @ 0x00910C30 / dtor @ 0x00910B20, AIAction_PerformActionAndWait
// @ 0x0097DB70.
// Layout: local model (PBrain +4; WarningCount +0x14, FrameOfLastWarning +0x18, PPlayer +0x20; world message manager +0x60;
// the criminal action is CCreatureActionBase (0xa8) + a CIntelligentPointer at +0xa8).
// Shape: the admonishing watch loop is placed at the end; the frame window compare is unsigned.
extern long g_FramesPerSecond;                  // 0x01375550
long __fastcall GFGetCurrentFrame();             // 0x0049D870
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CThing {
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
struct CThingAICreature : CThingCreatureBase { };
struct CThingPlayerCreature : CThingCreatureBase { };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    template<class U> __forceinline CIntelligentPointer(const CIntelligentPointer<U>& other) { SetPItem(other.GetPItem()); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {    // the MoveToThingProximity argument type
    virtual void v00();
    CIntelligentPointerThing(const CBaseIntelligentPointer& other);   // 0x00930D20
};
__forceinline bool IsThingAlive(const CBaseIntelligentPointer& thing) { return thing.GetPItem() != 0 && !(thing.GetPItem()->m_flags91 & 1); }
struct CMessageEventManager {
    void AddMessage(long type, unsigned long lifetime, const C3DVector* pos, CThing* from, CThing* to,
                    float range, float radius, long data);   // 0x00687540
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageEventManager; // +0x60
    static unsigned long GetWorldFrame() { return GFGetCurrentFrame(); }
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
};
struct CCreatureAction_DeclarePlayerACriminal : CCreatureActionBase {
    CBaseIntelligentPointer PPlayer;            // +0xa8
    CCreatureAction_DeclarePlayerACriminal(CThingCreatureBase* creature, CThingPlayerCreature* player);   // 0x00910C30
    ~CCreatureAction_DeclarePlayerACriminal();  // 0x00910B20
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingAICreature* GetCreature() const { return PBrain->Creature; }
    CWorld* GetPThingWorld();                   // 0x008FCFA0
    CWorld* GetPWorld();                        // 0x008FCFC0
    void Terminate();                           // 0x008FCF40
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_LookAtThing(long repeat, CIntelligentPointer<const CThing> thing, long frames, float speed);   // 0x0097D6F0
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerThing thing, float proximity, bool* arrived,
                                       long movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
};
struct CAIStateGroup_MinionGuardReactToPlayerWithWeapon : CAIStateGroupBase {
    long WarningCount;                          // +0x14
    unsigned long FrameOfLastWarning;           // +0x18
    long LastWarningID;                         // +0x1c
    CIntelligentPointer<CThingPlayerCreature> PPlayer;   // +0x20
    bool IsAnotherGuardAdmonishingPlayer();     // 0x00939D00
    bool CheckForOtherGuardsNewWarning();       // 0x00939D70
    void RemoveMyWarningMessage();              // 0x00939DD0
    bool IsPlayerCarryingWeapon();              // 0x00939E70
    __forceinline bool IsWithinWarningTime()
    {
        long framesPerSecond = g_FramesPerSecond;
        return GetPThingWorld()->GetWorldFrame() - FrameOfLastWarning < (unsigned long)(framesPerSecond * 4);
    }
    void Update();
};

void CAIStateGroup_MinionGuardReactToPlayerWithWeapon::Update()
{
    if (IsThingAlive(PPlayer) && IsPlayerCarryingWeapon()) {
        if (AIAction_LookAtThing(1, PPlayer, 0xf, 0.0027777f) != 2)
            return;
        if (IsThingAlive(PPlayer) && IsPlayerCarryingWeapon()) {
            if (!IsAnotherGuardAdmonishingPlayer()) {
                ++WarningCount;
                CThingPlayerCreature* player = PPlayer.GetPItem();
                CThingAICreature* creature = GetCreature();
                CMessageEventManager* manager = GetPWorld()->PMessageEventManager;
                manager->AddMessage(0x1b, 300, &creature->GetPosition(), creature, player, 0.0f, 0.0f, WarningCount);
                bool justWarning = false;
                if (WarningCount == 1 || WarningCount == 2)
                    justWarning = true;
                FrameOfLastWarning = GetPThingWorld()->GetWorldFrame();
                while (IsWithinWarningTime()) {
                    if (!IsThingAlive(PPlayer)) {
                        Terminate();
                        return;
                    }
                    if (!IsPlayerCarryingWeapon()) {
                        RemoveMyWarningMessage();
                        Terminate();
                        return;
                    }
                    if ((PPlayer.GetPItem()->CThing::GetPosition() - GetCreature()->CThing::GetPosition()).GetLengthSquared() > 25.0f) {
                        bool arrived;
                        AIAction_MoveToThingProximity(1, PPlayer, 5.0f, &arrived, 5, true, false, true, true, 0.0f);
                    }
                    if (AIAction_LookAtThing(1, PPlayer, 0xf, 0.0027777f) != 2)
                        return;
                }
                if (justWarning)
                    return;
                AIAction_PerformActionAndWait(&CCreatureAction_DeclarePlayerACriminal(GetCreature(), PPlayer.GetPItem()), false);
            } else {
                long result;
                do {
                    result = AIAction_LookAtThing(2, PPlayer, 0xf, 0.0027777f);
                    if (!IsThingAlive(PPlayer) || !IsPlayerCarryingWeapon()) {
                        Terminate();
                        return;
                    }
                    CheckForOtherGuardsNewWarning();
                } while (result == 4);
                return;
            }
        }
    }
    Terminate();
}
