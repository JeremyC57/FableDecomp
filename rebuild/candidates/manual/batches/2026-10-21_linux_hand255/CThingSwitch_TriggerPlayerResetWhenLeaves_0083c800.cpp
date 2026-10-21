// CThingSwitch::TriggerPlayerResetWhenLeaves() -- re-arming area switch: if an initialised player's living hero is inside the trigger square and the switch is not already triggered, count the trigger and queue a CEventTriggered (component 0x42); the triggered flag ends up as "some hero is inside", so the switch re-arms once every hero has left
// Leaf: TriggerPlayerResetWhenLeaves
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083C800, length 388 bytes (plain `ret`).
// Callees: as the sibling SwitchTriggerPlayerOnlyMultipleTimesInArea 0x0083C690 (same TU): CThing::GetPosition() const
//   @ 0x006A4D00, player-manager accessors @ 0x004C79F0 (const) / 0x004C7A00, CPlayerManager::GetPlayer(long) const
//   @ 0x00449910 / non-const @ 0x004498C0, CInitBaseClass::IsInitialised() const @ 0x0099A330, CPlayer::GetHeroCreature
//   @ 0x00487DC0, LowerBound @ 0x0040F020, operator new @ 0x00BFEA1A, NEntityEvents::CEventTriggered(CThing*)
//   @ 0x0083CF20, CTCEntityEvents::AddEvent @ 0x008113C0.
// Layout (retail): as the sibling, plus Triggered +0xdc (Ego_r.pdb +0xf4, CThingPhysical 0x18 lower).
// Shape: `inArea` lives in bl and is written back to Triggered on both exits (the loop end and the break after the
//   event), the loop index in ebp.
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
    __forceinline bool ContainsPoint(const C2DVector& p) const { return p.X >= TLX && p.X < BRX && p.Y >= TLY && p.Y < BRY; }
};

struct CEventBase { virtual void v00(); };
struct CThing;
namespace NEntityEvents {
struct CEventTriggered : CEventBase {
    unsigned char _body[0x10 - 4];
    CEventTriggered(CThing* triggerer);                 // 0x0083CF20
};
}
struct CTCEntityEvents {
    void AddEvent(CEventBase* event);                   // 0x008113C0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};

struct CPlayer {
    bool IsInitialised() const;                         // 0x0099A330 (CInitBaseClass)
    CThing* GetHeroCreature();                          // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetPlayer(long number);                    // 0x004498C0
    const CPlayer* GetPlayer(long number) const;        // 0x00449910
};

struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  Dead : 1;                            // +0x91 bit 0
    const CPlayerManager* GetPlayerManager() const;     // 0x004C79F0
    CPlayerManager* GetPlayerManager();                 // 0x004C7A00
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};

struct CThingPhysical : CThing {
    const C3DVector& GetPosition() const;               // 0x006A4D00
};

struct CThingSwitch : CThingPhysical {
    unsigned char _pad94[0xd0 - 0x94];
    float         TriggerRadius;                        // +0xd0
    unsigned char _padd4[0xdc - 0xd4];
    bool          Triggered;                            // +0xdc
    unsigned char _paddd[0xe4 - 0xdd];
    long          TimesTriggered;                       // +0xe4
    void TriggerPlayerResetWhenLeaves();
};

void CThingSwitch::TriggerPlayerResetWhenLeaves()
{
    C2DBoxF area;
    area.TLX = GetPosition().X - TriggerRadius;
    area.TLY = GetPosition().Y - TriggerRadius;
    area.BRX = GetPosition().X + TriggerRadius;
    area.BRY = GetPosition().Y + TriggerRadius;
    bool inArea = false;
    for (long i = 0; i < 4; ++i) {
        const CThing* self = this;
        if (self->GetPlayerManager()->GetPlayer(i)->IsInitialised()) {
            CThingPhysical* hero = static_cast<CThingPhysical*>(GetPlayerManager()->GetPlayer(i)->GetHeroCreature());
            if (hero && !hero->Dead && area.ContainsPoint(C2DVector(hero->GetPosition().X, hero->GetPosition().Y))) {
                inArea = true;
                if (!Triggered) {
                    ++TimesTriggered;
                    if (HasTC(0x42)) {
                        CTCEntityEvents* events = (CTCEntityEvents*)GetTC(0x42);
                        events->AddEvent(new NEntityEvents::CEventTriggered(hero));
                    }
                    break;
                }
            }
        }
    }
    Triggered = inArea;
}
