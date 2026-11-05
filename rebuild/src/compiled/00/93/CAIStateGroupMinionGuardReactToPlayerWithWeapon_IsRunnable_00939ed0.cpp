// CAIStateGroup_MinionGuardReactToPlayerWithWeapon::IsRunnable() -- runnable for a village guard when the (re-acquired) living player carries a weapon, is in extended line of sight and within 15 units in XY; also resets or decays the warning count every two minutes
// Leaf: ?IsRunnable@CAIStateGroup_MinionGuardReactToPlayerWithWeapon@@QAE_NXZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00939ED0, length 430 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined lookups: 0x23 = CTCVillager, 0x34 = CTCPerceiveThing),
// GFGetPVillageInterface @ 0x007EE540 (manifest "IS_THING_ALIVE<..>" propagated), CBaseIntelligentPointer ctor @ 0x00A01B10 /
// GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10, CAIStateGroupBase::GetPPlayerManager @ 0x008FCF90,
// CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, CAIStateGroupBase::GetPWorld @ 0x008FCFC0,
// CAIStateGroupBase::GetPThingWorld @ 0x008FCFA0 (both only feed the static CWorld::GetWorldFrame @ 0x0049D870; the manifest's
// "CPlayer::GetCurrentMode" on both is propagated), IsPlayerCarryingWeapon @ 0x00939E70 (this class),
// CTCPerceiveThing::IsPlayerExtendedLineOfSight @ 0x00744830, CThing::GetPos @ 0x004C73D0 (twice).
// Globals: CMainGameComponent::GetConstantFPS() @ 0x01375550; 225.0f @ 0x012922C0; CIntelligentPointer<CThingPlayerCreature> vtable 0x01231C4C.
// Shape: nested ifs with one shared false exit; the world pointer and the frames-per-second value are named locals ahead of
// the static frame read (fps is loaded before the call, into edi); the hero goes through the implicit
// CIntelligentPointer<CThingPlayerCreature> temporary and an inline operator=.
// Layout: Ego_r.pdb CAIStateGroup_MinionGuardReactToPlayerWithWeapon (NumberOfTimesPlayerTold +0x14, FrameLastToldPlayer
// +0x18, LastMessageHeard +0x1c, Player +0x20); the engine header flattens the base, so the model is local.
#include "engine/C3DVector.h"
class CTCBase;

#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CThingPlayerCreature;

class CBaseObject;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CBaseObject*a0);          // 0x00A01B90
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingPlayerCreature>, vtable 0x01231C4C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
};

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CWorld { public:
    static long __fastcall GetFrame();     // 0x0049D870
};
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CTCVillage;
struct CTCVillager;
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)

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
    protected:
    CPlayerManager& GetPlayerManager() const;        // 0x008FCF90
    public:
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
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
    if (creature->HasTC(TCI_VILLAGE_MEMBER) && ((CTCVillageMember*)(((CTCVillager*)&creature->GetTC(TCI_VILLAGE_MEMBER))))->GetPVillage() != 0) {
        if (((CThing*)Player.GetPItem()) == 0) {
            NumberOfTimesPlayerTold = 0;
            FrameLastToldPlayer = 0;
            Player = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        } else if (NumberOfTimesPlayerTold > 0) {
            const CWorld& world = GetWorld();
            long fps = CMainGameComponent::GetConstantFPS();
            if (world.GetFrame() - FrameLastToldPlayer > (unsigned long)(fps * 120)) {
                --NumberOfTimesPlayerTold;
                FrameLastToldPlayer = PeekWorld().GetFrame();
            }
        }
        if (((CThing*)Player.GetPItem()) != 0 && !((CThing*)Player.GetPItem())->IsDead() && IsPlayerCarryingWeapon()) {
            CThing* self = PBrain->PCreature;
            if (self->HasTC(TCI_PERCEIVE_THING) && ((CTCPerceiveThing*)&self->GetTC(TCI_PERCEIVE_THING))->IsPlayerExtendedLineOfSight()) {
                CThing* me = PBrain->PCreature;
                const C3DVector& player = ((CThing*)Player.GetPItem())->GetPos();
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