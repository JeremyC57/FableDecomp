// CPlayer::InitModeControlCreature(EPlayerMode mode, CProcessedInput* input) -- entering the control-creature mode attaches the eighteen walking/combat input processes to the player interface and, with a player character present, switches the display to a fresh third-person view, sets the character's controlled movement type and levels the controlled creature's facing pitch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00633D80, length 398 bytes (+ int3 padding), `ret 8`.
// Identity: Ego_r.pdb `?InitModeControlCreature@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z` (a
// CPlayer::PlayerModeInfo InitFunc; UninitModeControlCreature 0x00633420 is landed).
// Callees: CAInputProcess::AddToInterface(CGamePlayerInterface*) (virtual slot 1, eighteen processes),
// CBaseIntelligentPointer::GetPItem 0x00A01B50, operator new 0x00BFEA1A (0x9C bytes),
// NDisplayView::CView3DThirdPerson::CView3DThirdPerson 0x0069E940, CDisplayViewManager::SetView(CViewBase*) 0x00640050
// (g_pMainGameComponent 0x013B86A0 -> display engine +0x28 -> view manager +0xE0), CPlayer::GetControlledCreature
// 0x00487DD0, CPlayer::GetHeroThing 0x00487DC0, the physics component's slot 67 (+0x10C, SetFacingAngleYZ(float)).
// Layout: engine/CPlayer.h (the scoped_ptr<CInputProcess*> members, PlayerInterface +0x0C, PlayerCharacter +0x34) through
// the lane's CPlayer_Methods carrier; the player creature's ControlledMovementType at retail +0x17C (PDB +0x1A4),
// CThing PhysicsTC +0x60, dead bit +0x91 & 1 -- the same model as the landed InitModeBoastUI (0x00633F10).
// Shape: one AddToInterface per process in source order with PlayerInterface re-read each time; `&PlayerCharacter`
// stays in edi for both GetPItem calls; the new-expression's null test selects 0 for SetView.
#include "engine/CPlayer.h"

typedef long EPlayerMode;
typedef long EControlledMovementType;
struct CProcessedInput;
struct CBaseObject;

struct CAInputProcess {
    virtual void VFunc00();
    virtual bool AddToInterface(CGamePlayerInterface* playerInterface);   // slot 1 (+0x04)
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
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66();
    virtual void SetFacingAngleYZ(float angle);        // slot 67 (+0x10C)
};

struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60 (PDB +0x70)
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   m_stateFlags;                       // +0x91 (bit 0: dead)
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
};
struct CThingPlayerCreature : CThing {
    unsigned char           _pad94[0x17c - 0x94];
    EControlledMovementType ControlledMovementType;     // +0x17c (PDB +0x1a4)
};

inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PItem;                                 // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct CViewBase { };
namespace NDisplayView {
struct CView3DThirdPerson : CViewBase {                 // 0x9c bytes, vtable 0x0125C7F8
    unsigned char _body[0x9c];
    CView3DThirdPerson();                               // 0x0069E940
};
}
struct CDisplayViewManager {
    void SetView(CViewBase* view);                      // 0x00640050
};
struct CDisplayEngine {
    unsigned char        _pad00[0xe0];
    CDisplayViewManager* ViewManager;                   // +0xe0
};
struct CMainGameComponent {
    unsigned char   _pad00[0x28];
    CDisplayEngine* DisplayEngine;                      // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

struct CPlayer_Methods : CPlayer {
    CIntelligentPointer<CThingPlayerCreature>& PlayerCharacterPointer() { return *reinterpret_cast<CIntelligentPointer<CThingPlayerCreature>*>(PlayerCharacter); }
    CThing* GetHeroThing();                             // 0x00487DC0
    CThing* GetControlledCreature();                    // 0x00487DD0
    void InitModeControlCreature(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer_Methods::InitModeControlCreature(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreature_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureActivateZTarget_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureRightStick_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessRightStickLookAround_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessCycleSpecialCameraModes_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessCreatureMovement_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessUseEnvironment_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessBlock_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessDebugControls_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessQuickAccessItems_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessCombat_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessSpecialAbilities_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessJumpingAndRolling_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessSetRangedWeaponMode_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessSetRangedWeaponThirdPerson_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessQuickAccessMenu_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessWatchForControlAngleChange_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessToggleViewHeroMode_ptr)->AddToInterface(PlayerInterface);
    if (PlayerCharacterPointer().GetPItem()) {
        g_pMainGameComponent->DisplayEngine->ViewManager->SetView(new NDisplayView::CView3DThirdPerson);
        PlayerCharacterPointer().GetPItem()->ControlledMovementType = 1;
        if (IsThingAlive(GetControlledCreature()))
            GetHeroThing()->GetPhysicsTC()->SetFacingAngleYZ(0.0f);
    }
}