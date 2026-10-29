// CAIStateGroup_MinionGuardReactToPlayerWithWeapon::IsRunnable() -- runnable for a village guard when the (re-acquired) living player carries a weapon, is in extended line of sight and within 15 units in XY; also resets or decays the warning count every two minutes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00939ED0, length 430 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined lookups: 0x23 = CTCVillager, 0x34 = CTCPerceiveThing),
// GFGetPVillageInterface @ 0x007EE540 (manifest "IS_THING_ALIVE<..>" propagated), CBaseIntelligentPointer ctor @ 0x00A01B10 /
// GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10, CAIStateGroupBase::GetPPlayerManager @ 0x008FCF90,
// CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, CAIStateGroupBase::GetPWorld @ 0x008FCFC0,
// CAIStateGroupBase::GetPThingWorld @ 0x008FCFA0 (both only feed the static CWorld::GetWorldFrame @ 0x0049D870; the manifest's
// "CPlayer::GetCurrentMode" on both is propagated), IsPlayerCarryingWeapon @ 0x00939E70 (this class),
// CTCPerceiveThing::IsPlayerExtendedLineOfSight @ 0x00744830, CThing::GetPos @ 0x004C73D0 (twice).
// Globals: g_framesPerSecond @ 0x01375550; 225.0f @ 0x012922C0; CIntelligentPointer<CThingPlayerCreature> vtable 0x01231C4C.
// Shape: nested ifs with one shared false exit; the world pointer and the frames-per-second value are named locals ahead of
// the static frame read (fps is loaded before the call, into edi); the hero goes through the implicit
// CIntelligentPointer<CThingPlayerCreature> temporary and an inline operator=.
// Layout: Ego_r.pdb CAIStateGroup_MinionGuardReactToPlayerWithWeapon (NumberOfTimesPlayerTold +0x14, FrameLastToldPlayer
// +0x18, LastMessageHeard +0x1c, Player +0x20); the engine header flattens the base, so the model is local.
class C3DVector { public: float X, Y, Z; };
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
class CThingPlayerCreature : public CThing { };
class CThingPlayerCreature;

struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingPlayerCreature>, vtable 0x01231C4C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                   // 0x00449970
};
class CWorld { public:
    static long __fastcall GetFrame();     // 0x0049D870
};
extern long g_framesPerSecond;                  // 0x01375550

class CTCVillage;
struct CTCVillager;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

class CTCPerceiveThing { public:                       // interface 0x34
    bool IsPlayerExtendedLineOfSight() const;   // 0x00744830
};

class CAIBrain { public:
    virtual void v00();
    unsigned char _pad[0x14];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated, Priority, Enabled, _pad13;
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
protected:
    CWorld& GetWorld() const;    // 0x008FCFC0
public:
};

struct CAIStateGroup_MinionGuardReactToPlayerWithWeapon : CAIStateGroupBase {
    long NumberOfTimesPlayerTold;               // +0x14
    unsigned long FrameLastToldPlayer;          // +0x18
    unsigned long LastMessageHeard;             // +0x1c
    CIntelligentPointer Player;                 // +0x20
private:
    bool IsPlayerCarryingWeapon() const;        // 0x00939E70 ?IsPlayerCarryingWeapon@...@@ABE_NXZ (private)
public:
    bool IsRunnable();
};

bool CAIStateGroup_MinionGuardReactToPlayerWithWeapon::IsRunnable()
{
    CThing* creature = PBrain->PCreature;
    if (creature->HasTC(0x23) && GFGetPVillageInterface(creature->GetTC<CTCVillager>(0x23)) != 0) {
        if (Player.GetPItem() == 0) {
            NumberOfTimesPlayerTold = 0;
            FrameLastToldPlayer = 0;
            Player = GetPPlayerManager()->GetMainPlayer()->GetPControlledCreature();
        } else if (NumberOfTimesPlayerTold > 0) {
            const CWorld& world = GetWorld();
            long fps = g_framesPerSecond;
            if (world.GetFrame() - FrameLastToldPlayer > (unsigned long)(fps * 120)) {
                --NumberOfTimesPlayerTold;
                FrameLastToldPlayer = PeekWorld().GetFrame();
            }
        }
        if (Player.GetPItem() != 0 && !(Player.GetPItem()->m_flags91 & 1) && IsPlayerCarryingWeapon()) {
            CThing* self = PBrain->PCreature;
            if (self->HasTC(0x34) && self->GetTC<CTCPerceiveThing>(0x34)->IsPlayerExtendedLineOfSight()) {
                CThing* me = PBrain->PCreature;
                const C3DVector& player = Player.GetPItem()->GetPos();
                const C3DVector& mine = me->GetPos();
                float dx = mine.X - player.X;
                float dy = mine.Y - player.Y;
                if (dx * dx + dy * dy < 225.0f)
                    return true;
            }
        }
    }
    return false;
}