#include <new>
#include <stdio.h>
// Behaviour test for CPlayer::SetControlledCreature (0x00487CF0): both intelligent pointers take the creature, a fresh
// third-person view goes to the view manager, InitialPlayerModeSet is cleared and the game camera is reset.
class CBaseObject { public: int tag; };
class CThingPlayerCreature : public CBaseObject { public: };
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* PItem;
    CBaseIntelligentPointer() : PItem(0) { }
    void SetPItem(CBaseObject* item) { PItem = item; }
    CBaseObject* GetPItem() const { return PItem; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
static int g_viewsBuilt = 0;
class CView3DThirdPerson { public: CView3DThirdPerson() { ++g_viewsBuilt; } unsigned char _data[0x9C]; };
class CDisplayViewManager { public: CView3DThirdPerson* Current; void SetView(CView3DThirdPerson* view) { Current = view; } };
class CDisplayEngine { public: unsigned char _pad00[0xE0]; CDisplayViewManager* PViewManager; };
class CMainGameComponent { public: unsigned char _pad00[0x28]; CDisplayEngine* PDisplayEngine; };
CMainGameComponent* g_pMainGameComponent;
class CGameCameraManager { public: int Resets; void ResetCamera() { ++Resets; } };
class CAIGameCameraBase { public:
    CGameCameraManager* Manager;
    virtual void GetCameraManager(CGameCameraManager** out) { *out = Manager; }
};
struct CWorldCameraView { unsigned char _pad00[0x34]; CAIGameCameraBase* PCurrentGameCamera; };
class CPlayer { public:
    unsigned char _pad00[0x1c];
    CWorldCameraView* World;                                    // +0x1c
    unsigned char _pad20[0x0c];
    unsigned char ControlledCreature[8];                        // +0x2c
    unsigned char PlayerCharacter[8];                           // +0x34
    unsigned char _pad3c[0x216 - 0x3c];
    bool InitialPlayerModeSet;                                  // +0x216
};
struct CPlayerRetailFlags { unsigned char _pad00[0x216]; bool InitialPlayerModeSet; };
struct CPlayer_Methods : CPlayer {
    CIntelligentPointer<CThingPlayerCreature>& PlayerCharacterPointer() { return *reinterpret_cast<CIntelligentPointer<CThingPlayerCreature>*>(this->PlayerCharacter); }
    CIntelligentPointer<CThingPlayerCreature>& ControlledCreaturePointer() { return *reinterpret_cast<CIntelligentPointer<CThingPlayerCreature>*>(this->ControlledCreature); }
    void SetControlledCreature(CThingPlayerCreature* creature);
};
void CPlayer_Methods::SetControlledCreature(CThingPlayerCreature* creature)
{
    PlayerCharacterPointer() = CIntelligentPointer<CThingPlayerCreature>(creature);
    ControlledCreaturePointer() = CIntelligentPointer<CThingPlayerCreature>(creature);
    g_pMainGameComponent->PDisplayEngine->PViewManager->SetView(new CView3DThirdPerson);
    CGameCameraManager* manager;
    reinterpret_cast<CPlayerRetailFlags*>(this)->InitialPlayerModeSet = false;
    CAIGameCameraBase* camera = reinterpret_cast<CWorldCameraView*>(World)->PCurrentGameCamera;
    camera->GetCameraManager(&manager);
    manager->ResetCamera();
}
int main()
{
    int ok = 1;
    CDisplayViewManager vm; vm.Current = 0;
    CDisplayEngine de; de.PViewManager = &vm;
    CMainGameComponent mgc; mgc.PDisplayEngine = &de; g_pMainGameComponent = &mgc;
    CGameCameraManager cm; cm.Resets = 0;
    CAIGameCameraBase cam; cam.Manager = &cm;
    CWorldCameraView world; world.PCurrentGameCamera = &cam;
    CPlayer_Methods player; player.World = &world; player.InitialPlayerModeSet = true;
    new (player.PlayerCharacter) CIntelligentPointer<CThingPlayerCreature>();
    new (player.ControlledCreature) CIntelligentPointer<CThingPlayerCreature>();
    CThingPlayerCreature hero; hero.tag = 7;
    player.SetControlledCreature(&hero);
    if (player.PlayerCharacterPointer().GetPItem() != &hero || player.ControlledCreaturePointer().GetPItem() != &hero) ok = 0;
    if (g_viewsBuilt != 1 || vm.Current == 0) ok = 0;
    if (player.InitialPlayerModeSet != false || cm.Resets != 1) ok = 0;
    if (ok) { printf("00487cf0_TEST PASS\n"); return 0; }
    printf("00487cf0_TEST FAIL\n"); return 1;
}