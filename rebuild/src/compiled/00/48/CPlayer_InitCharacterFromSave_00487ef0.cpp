// CPlayer::InitCharacterFromSave(const NPlayerManager::CPlayerSave& save) -- refresh the cached thing manager / world map, load the saved player character, adopt it as the controlled creature owned by the main player, enter the default player mode and point the game camera at it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00487EF0, length 192 bytes, `ret 4`.
// Identity: Ego_r.pdb `?InitCharacterFromSave@CPlayer@@QAE_NABVCPlayerSave@NPlayerManager@@@Z`.
// Callees: the world's GetThingManager (slot 3, +0xC) and GetWorldMap (slot 12, +0x30),
// CPlayer::LoadPlayerCharacterFromGameState 0x00487C20, CPlayer::SetControlledCreature 0x00487CF0,
// CBaseIntelligentPointer::GetPItem 0x00A01B50, CPlayerManager::GetMainPlayerNumber 0x00449700,
// CThing::SetPlayerNumber(long) 0x004C7440 (stores the byte +0x90), CPlayer::AddMode(EPlayerMode, bool) 0x006345C0,
// the physics component's virtual slot 63 (+0xFC, a float getter whose result is dropped: `fstp st(0)`), the world's
// +0x34 object's slot 9 (+0x24, GetCameraManager out-parameter), CGameCameraManager::SetMainThing (slot 27, +0x6C) and
// CGameCameraManager::ResetCamera 0x0069A990.
// Layout (retail = engine/CPlayer.h up to +0x34): PlayerManager +0x10, World +0x1C, ThingManager +0x20, PWorldMap
// +0x24, PlayerCharacter +0x34; CThing PhysicsTC +0x60 (PDB +0x70).
// Shape: the success path is nested (`if (ok) { ...; return true; } return false;`) so the false epilogue sits last; `&PlayerCharacter` is held in edi for the four GetPItem
// calls; the thing for SetPlayerNumber is fetched before the manager call (object before argument). The physics
// component goes through an inline accessor (loaded straight into ecx). The camera manager is an uninitialised out
// local in its own block (the getter's bool is ignored): it lives in the dead `save` argument slot and is re-read from
// there for each call.
struct CBaseObject;
struct CThingManager;
struct CWorldMap;
struct CThingCreatureBase;

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
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngle();                     // slot 63 (+0xFC; CTCPhysicsStandard 0x00723B30: atan2 of the facing)
};

struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60 (PDB +0x70)
    void SetPlayerNumber(long number);                  // 0x004C7440
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
};
struct CThingPlayerCreature;

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

struct CGameCameraManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26();
    virtual void SetMainThing(const CThingCreatureBase* thing);   // slot 27 (+0x6C, 0x00697C10)
    void ResetCamera();                                 // 0x0069A990
};
struct CCameraManagerSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);   // slot 9 (+0x24)
};

struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager* GetThingManager();           // slot 3 (+0xC)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                   // slot 12 (+0x30)
    unsigned char         _pad04[0x34 - 0x04];
    CCameraManagerSource* CameraSource;                 // +0x34
};

struct CPlayerManager {
    long GetMainPlayerNumber();                         // 0x00449700
};

namespace NPlayerManager { struct CPlayerSave; }
typedef long EPlayerMode;

struct CPlayer {
    void*           __vftable;
    unsigned char   _pad04[0x10 - 0x04];
    CPlayerManager* PlayerManager;                      // +0x10
    unsigned char   _pad14[0x1c - 0x14];
    CWorld*         World;                              // +0x1c
    CThingManager*  ThingManager;                       // +0x20
    CWorldMap*      PWorldMap;                          // +0x24
    unsigned char   _pad28[0x34 - 0x28];
    CIntelligentPointer<CThingPlayerCreature> PlayerCharacter;   // +0x34
    bool LoadPlayerCharacterFromGameState(const NPlayerManager::CPlayerSave& save);   // 0x00487C20
    void SetControlledCreature(CThingPlayerCreature* creature);  // 0x00487CF0
    void AddMode(EPlayerMode mode, bool flag);          // 0x006345C0
    bool InitCharacterFromSave(const NPlayerManager::CPlayerSave& save);
};

struct CThingPlayerCreature : CThing { };

bool CPlayer::InitCharacterFromSave(const NPlayerManager::CPlayerSave& save)
{
    ThingManager = World->GetThingManager();
    PWorldMap = World->GetWorldMap();
    if (LoadPlayerCharacterFromGameState(save)) {
        SetControlledCreature(PlayerCharacter.GetPItem());
        PlayerCharacter.GetPItem()->SetPlayerNumber(PlayerManager->GetMainPlayerNumber());
        AddMode(1, true);
        PlayerCharacter.GetPItem()->GetPhysicsTC()->GetFacingAngle();
        {
            CGameCameraManager* camera;
            World->CameraSource->GetCameraManager(&camera);
            camera->SetMainThing((const CThingCreatureBase*)PlayerCharacter.GetPItem());
            camera->ResetCamera();
        }
        return true;
    }
    return false;
}