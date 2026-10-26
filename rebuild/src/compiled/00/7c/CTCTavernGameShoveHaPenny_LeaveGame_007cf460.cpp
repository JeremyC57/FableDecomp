// CTCTavernGameShoveHaPenny::LeaveGame() -- LeaveGame override: base LeaveGame, then fetches the camera manager from the world, releases scripted camera control and resets the camera
// Retail: Fable.exe @ 0x007CF460, 50 bytes (vtable 0x0126F2F4 slot 30 = the LeaveGame slot).
// Callees: CTCTavernGameCoinBase::LeaveGame 0x008EF110 (non-virtual base call), CTCBase::GetWorld 0x00686A70 (manifest
//          "CViewBase::GetDisplayEngine" is propagated), the world's +0x34 object's virtual slot 9
//          (+0x24, bool out-parameter getter; its result is ignored here),
//          CGameCameraManager::ReleaseScriptedControl 0x00698950 and ::ResetCamera 0x0069A990.
// Shape: the out pointer is an UNinitialised local (no zero store; the CoinGolf override 0x007D4A90
//        initialises it and tests the result) living in the `push ecx` slot and re-read for each
//        call; the +0x34 object comes through an inline accessor so it loads straight into ecx.
class CGameCameraManager { public:
    void ReleaseScriptedControl();                  // 0x00698950
    void ResetCamera();                             // 0x0069A990
};

struct CCameraManagerSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);     // slot 9 (+0x24)
};

class CWorld { public:
    unsigned char _pad[0x34];
    CCameraManagerSource* m_pCameraSource;          // +0x34
    CCameraManagerSource* GetCameraSource() { return m_pCameraSource; }
};

class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
class CTCTavernGame : public CTCBase
{
public:
};

struct CTCTavernGameCoinBase : public CTCTavernGame {
    unsigned char _pad[0x108];
    void LeaveGame();                               // 0x008EF110
};

struct CTCTavernGameShoveHaPenny : CTCTavernGameCoinBase {
    void vfunc_30();
};

void CTCTavernGameShoveHaPenny::vfunc_30()
{
    CTCTavernGameCoinBase::LeaveGame();
    CGameCameraManager* camera;
    GetWorld().GetCameraSource()->GetCameraManager(&camera);
    camera->ReleaseScriptedControl();
    camera->ResetCamera();
}