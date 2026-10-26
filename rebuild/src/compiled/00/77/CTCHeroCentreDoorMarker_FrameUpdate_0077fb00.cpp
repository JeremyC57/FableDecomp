// CTCHeroCentreDoorMarker::FrameUpdate() -- while the hero centre is alive, raise game event 0x34 (door type + hero-centre UID) the first time the hero walks inside the marker radius, then (for door type 1) update the particle experience
// Leaf: FrameUpdate
// Flags: /GS
//
// Retail 0x0077FB00, 321 bytes (plain `ret`, /GS cookie 0x0139C8A8 checked by __security_check_cookie @ 0x00BFE9F9).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (inline IS_THING_ALIVE pair, then the UID read), CTCBase::GetPlayerManager
// @ 0x00686D40, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroThing @ 0x00487DC0, CThing::GetPosition @ 0x004C73D0
// (own thing first, then the hero), CMemoryBuffer::Copy @ 0x009F1760 (DoorType, then the 8-byte UID), CMainGameComponent::AddGameEvent
// @ 0x0041649C on g_pMainGameComponent 0x013B86A0, CTCHeroCentreDoorMarker::UpdateParticleExperience @ 0x0077F970.
// Shape: the landed CTCHeroCentre::OpenTrainingScreen 0x0077D4B0 event model (CGameEvent over an empty CMemoryBuffer); the planar
// range test keeps both deltas and the radius on the x87 stack and compares Radius*Radius against dX*dX + dY*dY (`test ah,41h`).
// Layout (packed, landed 0x0077F5C0): Radius +0x0c, DoorType +0x10, HeroInsideRadius +0x14, PParticle +0x15, PHeroCentre +0x1d;
// CThing UID +0x18 (unsigned __int64), Player byte +0x90, dead bit 0 of +0x91.
class C3DVector { public: float X, Y, Z; };
class CThing {
public:
    unsigned char _pad00[0x18];
    unsigned __int64 UID;                               // +0x18
    unsigned char _pad20[0x90 - 0x20];
    signed char Player;                                 // +0x90
    unsigned char Dead : 1;                             // +0x91 bit 0
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    virtual void v00();
    void* PData;
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual void v00();
};
template <class P> inline bool IS_THING_ALIVE(const P& p) { return p.GetPItem() && !p.GetPItem()->Dead; }

class CPlayer { public:
    CThing* GetHeroThing();                             // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                           // 0x00449970
};
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);  // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                                 // +0x00
    signed char   Player;                               // +0x04
    unsigned char Data[32];                             // +0x05
    unsigned char EndPos;                               // +0x25
    bool          Valid;                                // +0x26
    bool          Replacement;                          // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLONG(long value) { Copy(4, &value); }
    void AddUID(unsigned __int64 value) { Copy(8, &value); }
};
class CMainGameComponent { public:
    void AddGameEvent(const CGameEvent& event);         // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CPlayerManager* GetPlayerManager();                 // 0x00686D40
};
#pragma pack(push, 1)
class CTCHeroCentreDoorMarker : public CTCBase {        // vtable 0x0126B044
public:
    float Radius;                                       // +0x0c
    long  DoorType;                                     // +0x10
    bool  HeroInsideRadius;                             // +0x14
    CIntelligentPointer<CThing> PParticle;              // +0x15
    CIntelligentPointer<CThing> PHeroCentre;            // +0x1d
    void UpdateParticleExperience();                    // 0x0077F970
    virtual void FrameUpdate();
};
#pragma pack(pop)

void CTCHeroCentreDoorMarker::FrameUpdate()
{
    if (IS_THING_ALIVE(PHeroCentre)) {
        CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
        if (hero && !hero->Dead) {
            const C3DVector& pos = ParentThing->GetPosition();
            const C3DVector& heroPos = hero->GetPosition();
            float dx = heroPos.X - pos.X;
            float dy = heroPos.Y - pos.Y;
            float radius = Radius;
            if (dx * dx + dy * dy < radius * radius) {
                if (!HeroInsideRadius) {
                    CGameEvent event(0x34);
                    event.AddLONG(DoorType);
                    event.AddUID(PHeroCentre.GetPItem()->UID);
                    event.Player = hero->Player;
                    g_pMainGameComponent->AddGameEvent(event);
                    HeroInsideRadius = true;
                }
            } else {
                HeroInsideRadius = false;
            }
        } else {
            HeroInsideRadius = false;
        }
    }
    if (DoorType == 1)
        UpdateParticleExperience();
}