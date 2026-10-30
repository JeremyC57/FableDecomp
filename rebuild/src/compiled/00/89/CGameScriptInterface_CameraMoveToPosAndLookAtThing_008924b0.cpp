// CGameScriptInterface::CameraMoveToPosAndLookAtThing(const C3DVector& pos, const CScriptThing& thing, float) const -- hand the camera manager a scripted go-to-and-look-at camera mode for a valid thing
// Leaf: ?CameraMoveToPosAndLookAtThing@CGameScriptInterface@@UBEXABVC3DVector@@ABVCScriptThing@@M@Z
// Retail: Fable.exe @ 0x008924B0, 116 bytes, `ret 0xc`; Ego_r.pdb same symbol. The float argument is unused.
// Body: if thing.IsValid() (slot 75) and bit 2 of the thing's ThingTypeFlags (+0x6c): t = thing.GetThing() (slot 11, called
// again); World (+0x04)->PCurrentGameCamera (+0x34)->GetCameraManager(&manager) (slot 9, +0x24, Ego_r.pdb
// ?GetCameraManager@CAIGameCameraBase@@UAE_NPAPAVCGameCameraManager@@@Z; `manager` lives in the dead `thing` slot);
// manager->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(new NCameraModes::CScriptModuleGoToLookAt(
// pos, *t))) -- operator new 0x00BFEA1A (0x6c bytes), ctor 0x00852D10, CCountedPointer(T*) 0x005ED270 (folded label),
// SetScriptedCameraMode 0x00699E60 (by-value CCountedPointer).
// Layout: CThing::ThingTypeFlags +0x6c (Ego_r.pdb +0x7c under the retail -0x10 shift); CWorld::PCurrentGameCamera +0x34
// (Ego_r.pdb). CCountedPointer is 8 bytes (object, count). The meaning of type-flag bit 2 is not recovered.
// Shape: /O2 (16-aligned entry). CCountedPointer needs its declared copy constructor: without it VC7.1 builds the temporary
// in the frame and copies it to the argument area (137v116); with it the pointer is constructed in place (`sub esp,8; mov ecx,esp`).
class C3DVector;
class CThing {
public:
    unsigned char _pad00[0x6c];
    unsigned long ThingTypeFlags;                        // +0x6c
};
namespace NCameraModes {
class CScriptModuleBase {
public:
    virtual ~CScriptModuleBase();
};
class CScriptModuleGoToLookAt : public CScriptModuleBase {
public:
    unsigned char _pad04[0x6c - 4];
    CScriptModuleGoToLookAt(const C3DVector& camPos, const CThing& target);   // 0x00852D10
};
}
template <class T> class CCountedPointer {
public:
    T*    Object;
    long* Count;
    CCountedPointer(T* object);                          // 0x005ED270 (instance shared by folding)
    CCountedPointer(const CCountedPointer& other);
    ~CCountedPointer();
};
class CGameCameraManager {
public:
    void SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase> mode);   // 0x00699E60
};
class CAIGameCameraBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** manager);   // slot 9 (+0x24)
};
class CWorld {
public:
    unsigned char      _pad00[0x34];
    CAIGameCameraBase* PCurrentGameCamera;               // +0x34
};
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                    // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                        // slot 75 (+0x12c)
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::CameraMoveToPosAndLookAtThing(const C3DVector& camPos, const CScriptThing& targetThing, float time) const
{
    if (targetThing.IsValid() && (targetThing.GetThing()->ThingTypeFlags & 4))
    {
        CThing* t = targetThing.GetThing();
        CGameCameraManager* manager;
        World->PCurrentGameCamera->GetCameraManager(&manager);
        manager->SetScriptedCameraMode(CCountedPointer<NCameraModes::CScriptModuleBase>(new NCameraModes::CScriptModuleGoToLookAt(camPos, *t)));
    }
}