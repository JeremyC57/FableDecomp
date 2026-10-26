// CTCTavernGameCoinGolf::EnterGame() -- coin-base EnterGame, score cleared, camera reset to distance 1 / pitch 0.1275 / yaw 0, the camera manager told to look around, then course 0 enabled
// Leaf: ?EnterGame@CTCTavernGameCoinGolf
// Retail: Fable.exe @ 0x007D34A0, 99 bytes; CTCTavernGameCoinGolf vtable 0x0126F49C slot 29 (EnterGame; the catalog's
//         "vfunc_29", cf. CTCTavernGameCardSorting slot 29 = EnterGame 0x007D4E30).
// Callees: CTCTavernGameCoinBase::EnterGame() 0x008EF390 (non-virtual base call); CTCBase::GetWorld() 0x00686A70 (manifest
//          "CViewBase::GetDisplayEngine" is propagated); the world's +0x34 object's virtual slot 9 (+0x24, bool
//          GetCameraManager(CGameCameraManager**)); CGameCameraManager::PushLookAround() 0x00699A50;
//          CTCTavernGameCoinGolf::EnableCourse(long) 0x007D2CC0.
// Layout: retail = Ego_r.pdb minus 0x18: ZAngle +0x148, XAngle +0x14c, CameraDistance +0x150; CTCTavernGame::Score +0x2c
//         (PDB +0x34).
// Shape: as CTCTavernGameCoinGolf::LeaveGame 0x007D4A90: the out pointer is zero-initialised and the getter's result tested;
//        the world's +0x34 object comes through an inline accessor.
class CGameCameraManager { public:
    void PushLookAround();                          // 0x00699A50
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
    unsigned char _pad[0x2c];
    float Score;                                    // +0x2c (CTCTavernGame)
    unsigned char _pad_0x30[0x118];
    void EnterGame();                               // 0x008EF390
};

class CTCTavernGameCoinGolf : public CTCTavernGameCoinBase { public:
    float ZAngle;                                   // +0x148
    float XAngle;                                   // +0x14c
    float CameraDistance;                           // +0x150
    void EnableCourse(long course);                 // 0x007D2CC0
    void EnterGame();
};

void CTCTavernGameCoinGolf::EnterGame()
{
    CTCTavernGameCoinBase::EnterGame();
    Score = 0.0f;
    CameraDistance = 1.0f;
    XAngle = 0.1275f;
    ZAngle = 0.0f;
    CGameCameraManager* camera = 0;
    if (GetWorld().GetCameraSource()->GetCameraManager(&camera))
        camera->PushLookAround();
    EnableCourse(0);
}