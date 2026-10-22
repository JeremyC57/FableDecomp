// CTCTavernGameCoinGolf::LeaveGame() -- LeaveGame override: coin-base LeaveGame, fade the table occupants back in, then (when the world hands out a camera manager) restore automatic camera control and reset the camera
// Retail: Fable.exe @ 0x007D4A90, 69 bytes (vtable 0x0126F49C slot 30 = the LeaveGame slot).
// Callees: CTCTavernGameCoinBase::LeaveGame 0x008EF110 (non-virtual base call),
//          CTCTavernGameCoinGolf::FadeOutTableOccupants(bool) 0x007D46D0, CTCBase::GetWorld 0x00686A70
//          (manifest "CViewBase::GetDisplayEngine" is propagated; returns the global's +0x24), the
//          world's +0x34 object's virtual slot 9 (+0x24, bool out-parameter getter),
//          CGameCameraManager::SetAutomaticCameraControl 0x00699090 and ::ResetCamera 0x0069A990.
// Shape: the camera-manager out pointer is a null-initialised local declared after the fade call
//        (`mov [esp+4],0` scheduled after `mov ecx,esi`); being address-taken it is re-read from its
//        `push ecx` slot for each call. The bool result is tested with `test al,al`. The +0x34 object
//        goes through an inline accessor so it is loaded straight into ecx (`mov ecx,[eax+34h]`; the
//        plain member read loads eax and copies).
struct CGameCameraManager {
    void SetAutomaticCameraControl();               // 0x00699090
    void ResetCamera();                             // 0x0069A990
};

struct CCameraManagerSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);     // slot 9 (+0x24)
};

struct CWorld {
    unsigned char _pad[0x34];
    CCameraManagerSource* m_pCameraSource;          // +0x34
    CCameraManagerSource* GetCameraSource() { return m_pCameraSource; }
};

struct CTCTavernGameCoinBase {
    unsigned char _pad[0x108];
    CWorld* GetWorld();                             // 0x00686A70 (CTCBase)
    void LeaveGame();                               // 0x008EF110
};

struct CTCTavernGameCoinGolf : CTCTavernGameCoinBase {
    void FadeOutTableOccupants(bool fadeOut);       // 0x007D46D0
    void vfunc_30();
};

void CTCTavernGameCoinGolf::vfunc_30()
{
    CTCTavernGameCoinBase::LeaveGame();
    FadeOutTableOccupants(true);
    CGameCameraManager* camera = 0;
    if (GetWorld()->GetCameraSource()->GetCameraManager(&camera)) {
        camera->SetAutomaticCameraControl();
        camera->ResetCamera();
    }
}