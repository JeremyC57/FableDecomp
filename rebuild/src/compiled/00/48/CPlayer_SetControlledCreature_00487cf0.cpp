// CPlayer::SetControlledCreature(CThingPlayerCreature* creature) -- make the creature both the player character and the controlled creature, switch the display to a fresh third-person view, re-arm the initial player mode and reset the game camera
// Grade: MATCH (retry 9) -- the InitialPlayerModeSet clear is a plain member store (through a retail-offset view of the
//   object) written BEFORE the camera expression: VC7.1 then hoists the World load above the byte store (same base,
//   different offset) and keeps the store ahead of the camera read; a store through a `bool*` computed from the header
//   pad may alias anything, so the lane could not reorder it and it sank below `mov ecx,[edx+34h] ; lea edx,[esp+8]`
// Retail: Fable.exe (TLC, VC7.1) @ 0x00487CF0, 205 bytes (+ int3 padding), `ret 4`.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (twice
//   each), operator new 0x00BFEA1A, CView3DThirdPerson::CView3DThirdPerson() 0x0069E940, CDisplayViewManager::SetView
//   0x00640050, the current game camera's virtual slot 9 (+0x24, fills an out pointer), CGameCameraManager::ResetCamera
//   0x0069A990; global g_pMainGameComponent 0x013B86A0.
// Shape: each assignment is `member = CIntelligentPointer<CThingPlayerCreature>(creature)`: a temporary intelligent
//   pointer (vtable 0x01231C4C) in the frame, the inline operator= (`SetPItem(other.GetPItem())`), then its destructor;
//   PlayerCharacter (+0x34) first, ControlledCreature (+0x2C) second. The view is `new CView3DThirdPerson` (null-tested
//   ctor call) handed to g_pMainGameComponent->PDisplayEngine (+0x28, Ego_r.pdb)->PViewManager (+0xE0, the counted
//   pointer's data); the new-expression is evaluated before the global chain is loaded. InitialPlayerModeSet is retail
//   +0x216 (PDB +0x21E: retail PlayerModes is 8 bytes narrower), stored as a real member of a retail-offset view
//   struct (the header's pad holds that byte) before the world's current game camera (CWorld +0x34, PCurrentGameCamera
//   in the PDB) is read; the camera hands back its manager through an out pointer. Other members (World +0x1C, ControlledCreature +0x2C,
//   PlayerCharacter +0x34) from engine/CPlayer.h.
#include "engine/CPlayer.h"

void* operator new(unsigned int size);                      // 0x00BFEA1A

class CBaseObject;
class CThingPlayerCreature;

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PItem;
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                             // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01231C4C for CThingPlayerCreature
public:
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CView3D;
class CView3DThirdPerson {                                  // 0x9C bytes
public:
    CView3DThirdPerson();                                   // 0x0069E940
    unsigned char _data[0x9C];
};
class CDisplayViewManager { public:
    void SetView(CView3DThirdPerson* view);                 // 0x00640050
};
class CDisplayEngine { public:
    unsigned char        _pad00[0xE0];
    CDisplayViewManager* PViewManager;                      // +0xE0 (CCountedPointer<CDisplayViewManager>::Data)
};
class CMainGameComponent { public:
    unsigned char   _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                         // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0

class CGameCameraManager { public:
    void ResetCamera();                                     // 0x0069A990
};
class CAIGameCameraBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void GetCameraManager(CGameCameraManager** out);   // slot 9 (+0x24)
};
struct CWorldCameraView {
    unsigned char      _pad00[0x34];
    CAIGameCameraBase* PCurrentGameCamera;                  // +0x34
};

struct CPlayerRetailFlags {                                 // retail view: InitialPlayerModeSet at +0x216
    unsigned char _pad00[0x216];
    bool          InitialPlayerModeSet;
};
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
    reinterpret_cast<CPlayerRetailFlags*>(this)->InitialPlayerModeSet = false;   // +0x216 (PDB +0x21E)
    CAIGameCameraBase* camera = reinterpret_cast<CWorldCameraView*>(World)->PCurrentGameCamera;
    camera->GetCameraManager(&manager);
    manager->ResetCamera();
}