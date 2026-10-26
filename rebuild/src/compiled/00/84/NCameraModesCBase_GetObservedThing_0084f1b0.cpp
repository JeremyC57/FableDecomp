// NCameraModes::CBase::GetObservedThing() const -- the thing the camera manager is observing: CameraManager->PObservedThing
// Leaf: ?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084F1B0, 14 bytes, thiscall: `mov ecx,[ecx+40h]; add ecx,100h; jmp GetPItem`.
// Identity (sweep3; FINDINGS chunk 481 "Callees: 0x84F1B0 = GetObservedThing"; the catalog name was already right, the
//   source was a `_Cons_val`/"CParticleMorphsAccessor::GetMorphEntryItem" stand-in): Ego_r.pdb/FableWin.pdb
//   ?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ, and FableWin CGameCameraManager::GetObservedThing with the
//   same return type; Ego_r NCameraModes::CBase::CameraManager is a CGameCameraManager* (retail +0x40) and
//   CGameCameraManager::PObservedThing is a CIntelligentPointer<const CThingGameObject> (retail +0x100; Ego_r +0x110 less the
//   smaller STLport list/vector before it); 66 callers, all in the camera-mode code (0x85xxxx-0x87xxxx).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (tail).
// Layout: local models -- engine/CGameCameraManager.h carries the Ego_r offset (PObservedThing 0x110; header debt).
class CBaseObject;
class CThingGameObject;

class CBaseIntelligentPointer {                             // 8 bytes: vptr, PItem
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    CBaseObject* PItem;                                     // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* Get() const { return (T*)GetPItem(); }
};

class CGameCameraManager {
public:
    unsigned char _before_PObservedThing[0x100];
    CIntelligentPointer<const CThingGameObject> PObservedThing;  // +0x100
    const CThingGameObject* GetObservedThing() const { return PObservedThing.Get(); }
};

namespace NCameraModes {
class CBase {
public:
    virtual ~CBase();
    unsigned char _before_CameraManager[0x3c];
    CGameCameraManager* CameraManager;                      // +0x40
    const CThingGameObject* GetObservedThing() const;
};
}

const CThingGameObject* NCameraModes::CBase::GetObservedThing() const
{
    return CameraManager->GetObservedThing();
}
