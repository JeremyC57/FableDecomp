// CPlayer::InitModeControlFreeCamera(EPlayerMode mode, CProcessedInput* input) -- entering free-camera control: flag it, attach the free-camera input process (pointing it at the player's current free camera) and start all three free cameras from the world's current camera
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006335E0, length 100 bytes.
// Callees: the input process's virtual AddToInterface(CGamePlayerInterface*) (slot 1), the world's virtual slot 6
// (+0x18), which fills a caller frame camera and returns a reference to the camera to copy.
// Layout: engine/CPlayer.h (PlayerInterface +0x0C, World +0x1C, CurrentFreeCamera +0x40, FreeCamera +0x78,
// OldFreeCamera +0xB0 (CCamera, 0x38 bytes), InputProcessControlFreeCamera_ptr +0x14C); ControllingFreeCamera is
// retail +0x223 (PDB +0x22B: the PlayerModes list is 8 bytes narrower in retail). The process's camera pointer at +0x28
// is descriptive.
// Shape: all three copies read the RETURNED reference (`mov esi,eax ; rep movsd`); a by-value CCamera result (named
// or bound to a const reference) re-forms its frame address for each copy instead.
#include "engine/CPlayer.h"

typedef long EPlayerMode;
class CProcessedInput;

class CCamera { public:                                            // 0x38 bytes, implicit copy
    unsigned long m_words[0xe];
};

struct CInputProcessVirtuals {
    virtual void v00();
    virtual bool AddToInterface(CGamePlayerInterface* playerInterface);   // slot 1 (+0x04)
};

struct CInputProcessControlFreeCameraRetail : CInputProcessVirtuals {
    unsigned char _pad04[0x28 - 0x04];
    CCamera*      m_pCamera;                                // +0x28
};

struct CWorldCameraSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual const CCamera& GetCurrentCamera(CCamera& scratch) const;   // slot 6 (+0x18): fills the scratch camera, returns it
};

struct CPlayer_Methods : CPlayer {
    bool& ControllingFreeCameraRetail() { return *(bool*)((unsigned char*)this + 0x223); }
    void InitModeControlFreeCamera(EPlayerMode mode, CProcessedInput* input);
};

void CPlayer_Methods::InitModeControlFreeCamera(EPlayerMode mode, CProcessedInput* input)
{
    ControllingFreeCameraRetail() = true;
    reinterpret_cast<CInputProcessVirtuals*>(this->InputProcessControlFreeCamera_ptr)->AddToInterface(this->PlayerInterface);
    reinterpret_cast<CInputProcessControlFreeCameraRetail*>(this->InputProcessControlFreeCamera_ptr)->m_pCamera = (CCamera*)this->CurrentFreeCamera;
    CCamera scratch;
    const CCamera& camera = reinterpret_cast<CWorldCameraSource*>(this->World)->GetCurrentCamera(scratch);
    *(CCamera*)this->CurrentFreeCamera = camera;
    *(CCamera*)this->FreeCamera = camera;
    *(CCamera*)this->OldFreeCamera = camera;
}