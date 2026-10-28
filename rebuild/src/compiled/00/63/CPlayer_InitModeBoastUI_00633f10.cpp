// CPlayer::InitModeBoastUI(EPlayerMode, CProcessedInput*) -- attach the boast-UI mode's input processes to the player interface and, with a player character present, switch the display to a third-person view, set the character's controlled movement type and level the controlled creature's facing pitch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00633F10, length 224 bytes (+ int3 padding), `ret 8`.
// Identity: Ego_r.pdb `?InitModeBoastUI@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z` (a CPlayer::PlayerModeInfo
// InitFunc).
// Callees: CAInputProcess::AddToInterface(CGamePlayerInterface*) (virtual slot 1, six processes),
// CBaseIntelligentPointer::GetPItem 0x00A01B50, operator new 0x00BFEA1A (0x9C bytes),
// NDisplayView::CView3DThirdPerson::CView3DThirdPerson 0x0069E940, CDisplayViewManager::SetView(CViewBase*) 0x00640050
// (g_pMainGameComponent 0x013B86A0 -> display engine +0x28 -> view manager +0xE0), CPlayer::PeekPControlledCreature
// 0x00487DD0, CPlayer::GetPControlledCreature 0x00487DC0, the physics component's slot 67 (+0x10C, SetFacingAngleYZ(float):
// CTCPhysicsStandard 0x007237E0 rebuilds the facing from the XY angle (slot 63), this angle and slot 65).
// Layout: engine/CPlayer.h (the scoped_ptr<CInputProcess*> members, PlayerInterface +0x0C, PlayerCharacter +0x34) with
// the lane's CPlayer method carrier (as the landed InitModeBerserk 0x006338D0); the player creature's
// ControlledMovementType at retail +0x17C (PDB +0x1A4), CThing PhysicsTC +0x60, dead bit +0x91 & 1.
// Shape: `&PlayerCharacter` stays in edi for both GetPItem calls; the new-expression's null test selects 0 for
// SetView; the alive test is the inline null + dead-bit check on PeekPControlledCreature(), the physics call goes to
// GetPControlledCreature()'s component through an inline accessor.
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer

typedef long EPlayerMode;
typedef long EControlledMovementType;
class CProcessedInput;
class CBaseObject;

class CAInputProcess { public:
    virtual void VFunc00();
    virtual bool AddToInterface(CGamePlayerInterface* playerInterface);   // slot 1 (+0x04)
};

class CTCPhysicsBase { public:
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

class CThing { public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60 (PDB +0x70)
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   m_stateFlags;                       // +0x91 (bit 0: dead)
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
};
class CThingPlayerCreature : public CThing { public:
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
class CDisplayViewManager { public:
    void SetView(CViewBase* view);                      // 0x00640050
};
class CDisplayEngine { public:
    unsigned char        _pad00[0xe0];
    CDisplayViewManager* ViewManager;                   // +0xe0
};
class CMainGameComponent { public:
    unsigned char   _pad00[0x28];
    CDisplayEngine* DisplayEngine;                      // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

class CPlayer : public CPlayerPdb { public:
    CIntelligentPointer<CThingPlayerCreature>& PlayerCharacterPointer() { return *reinterpret_cast<CIntelligentPointer<CThingPlayerCreature>*>(PlayerCharacter); }
    CThingPlayerCreature* GetPControlledCreature() const;                             // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;                    // 0x00487DD0
    void InitModeBoastUI(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer::InitModeBoastUI(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessBoastUI_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreature_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessRightStickLookAround_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessCycleSpecialCameraModes_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessCreatureMovement_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessWatchForControlAngleChange_ptr)->AddToInterface(PlayerInterface);
    if (PlayerCharacterPointer().GetPItem()) {
        g_pMainGameComponent->DisplayEngine->ViewManager->SetView(new NDisplayView::CView3DThirdPerson);
        PlayerCharacterPointer().GetPItem()->ControlledMovementType = 1;
        if (IsThingAlive(PeekPControlledCreature()))
            GetPControlledCreature()->GetPhysicsTC()->SetFacingAngleYZ(0.0f);
    }
}