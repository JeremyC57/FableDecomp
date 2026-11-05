// CAIStateGroup_MinionGuardReactToPlayerWithWeapon::Update() -- face the armed player; when another guard is already admonishing, keep watching until the player disarms; otherwise post a numbered warning, follow and watch the player for four seconds, and on the third warning declare the player a criminal
// Leaf: ?Update@CAIStateGroup_MinionGuardReactToPlayerWithWeapon@@QAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLengthSquared respelled GetSquaredMagnitude (the header / FableWin.pdb method, same computation).
// Naming wave 18: the global stand-in g_FramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093A080, length 832 bytes (+ int3 padding), plain `ret`, frame 0xb8.
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90, IsPlayerCarryingWeapon
// @ 0x00939E70, IsAnotherGuardAdmonishingPlayer @ 0x00939D00, CheckForOtherGuardsNewWarning @ 0x00939D70, the own-warning
// removal @ 0x00939DD0 (manifest "CheckAndRemove"), AIAction_LookAtThing @ 0x0097D6F0, AIAction_MoveToThingProximity
// @ 0x0097E380 (its pointer argument built by the out-of-line ctor @ 0x00930D20), CAIStateGroupBase::GetPThingWorld
// @ 0x008FCFA0 / GetPWorld @ 0x008FCFC0 / Terminate @ 0x008FCF40, CWorld::GetFrame @ 0x0049D870,
// CThingCreatureBase::PeekPos @ 0x006A4D00, CThing::PeekPos @ 0x004C73D0, CMessageEventManager::AddMessage
// @ 0x00687540, CCreatureAction_DeclarePlayerACriminal ctor @ 0x00910C30 / dtor @ 0x00910B20, AIAction_PerformActionAndWait
// @ 0x0097DB70.
// Layout: local model (PBrain +4; WarningCount +0x14, FrameOfLastWarning +0x18, PPlayer +0x20; world message manager +0x60;
// the criminal action is CCreatureActionBase (0xa8) + a CIntelligentPointer at +0xa8).
// Shape: the admonishing watch loop is placed at the end; the frame window compare is unsigned. In IsWithinWarningTime
//         the world pointer is fetched into a local BEFORE CMainGameComponent::GetConstantFPS() is read (retail calls GetPThingWorld, then
//         loads the global into ebx; a frames-per-second local read first puts the load ahead of the call).
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
#include "engine/C3DVector.h"
#include "engine/CThing.h"
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;       // 0x006A4D00
};
class CThingCreatureBase : public CThingPhysical { public:
};
class CThingAICreature : public CThingCreatureBase { public: };
class CThingPlayerCreature : public CThingCreatureBase { public: };
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject*a0);          // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    template<class U> __forceinline CIntelligentPointer(const CIntelligentPointer<U>& other) { SetPItem((CBaseObject*)(other.GetPItem())); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {    // the MoveToThingProximity argument type
    virtual void v00();
    CIntelligentPointerThing(const CBaseIntelligentPointer& other);   // 0x00930D20
};
__forceinline bool IsThingAlive(const CBaseIntelligentPointer& thing) { return thing.GetPItem() != 0 && !((CThing*)thing.GetPItem())->IsDead(); }
class CMessageEventManager { public:
    void AddMessage(long type, unsigned long lifetime, const C3DVector* pos, CThing* from, CThing* to,
                    float range, float radius, long data);   // 0x00687540
};
class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageEventManager; // +0x60
    static long __fastcall GetFrame();
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
};
namespace N_00910c30 { struct CCreatureAction_DeclarePlayerACriminal : CCreatureActionBase {
    CBaseIntelligentPointer PPlayer;            // +0xa8
    CCreatureAction_DeclarePlayerACriminal(CThingCreatureBase* creature, CThingPlayerCreature* player);   // 0x00910C30
    ~CCreatureAction_DeclarePlayerACriminal();  // 0x00910B20
}; } using N_00910c30::CCreatureAction_DeclarePlayerACriminal;
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};
enum EAIActionReturn { };
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingAICreature* GetCreature() const { return PBrain->Creature; }
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
protected:
    CWorld& GetWorld() const;    // 0x008FCFC0
public:
    void Terminate();                           protected: // 0x008FCF40
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait); public:   // 0x0097DB70
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
private:
    bool IsPlayerCarryingWeapon() const;        // 0x00939E70 ?IsPlayerCarryingWeapon@...@@ABE_NXZ (private)
public:
    __forceinline bool IsWithinWarningTime()
    {
        const CWorld& world = PeekWorld();
        long framesPerSecond = CMainGameComponent::GetConstantFPS();
        return world.GetFrame() - FrameOfLastWarning < (unsigned long)(framesPerSecond * 4);
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
                CMessageEventManager* manager = GetWorld().PMessageEventManager;
                manager->AddMessage(0x1b, 300, &creature->PeekPos(), creature, player, 0.0f, 0.0f, WarningCount);
                bool justWarning = false;
                if (WarningCount == 1 || WarningCount == 2)
                    justWarning = true;
                FrameOfLastWarning = PeekWorld().GetFrame();
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
                    if ((PPlayer.GetPItem()->GetPos() - GetCreature()->GetPos()).GetSquaredMagnitude() > 25.0f) {
                        bool arrived;
                        AIAction_MoveToThingProximity(1, PPlayer, 5.0f, &arrived, 5, true, false, true, true, 0.0f);
                    }
                    if (AIAction_LookAtThing(1, PPlayer, 0xf, 0.0027777f) != 2)
                        return;
                }
                if (justWarning)
                    return;
                AIAction_PerformActionAndWait(CCreatureAction_DeclarePlayerACriminal(GetCreature(), PPlayer.GetPItem()), false);
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