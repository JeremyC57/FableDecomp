// NCreatureAction::CActionEventBase::CActionEventBase() -- an unfinished action event
// Leaf: ??0CActionEventBase@NCreatureAction@@IAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006920F0, 22 bytes, thiscall, plain `ret`.
// Identity (proven): it stores vtable 0x0125C0C0 (RTTI .?AVCActionEventBase@NCreatureAction@@) after the CBaseClass
//   constructor; FableWin.pdb ??0CActionEventBase@NCreatureAction@@IAE@XZ (protected default constructor); Ego_r.pdb
//   CActionEventBase::Finished is the bool at +4 it clears. The manifest's "CAIStateGroup_MoveGoodsBase" is propagated.
// Callees: CBaseClass::CBaseClass() 0x0099A2D0.
// Shape (family M193, representative 0x006920F0): `CActionEventBase() : Finished(false) { }` over the out-of-line base
//   constructor, then the vtable store and the member. Speed TU.
class CBaseClass {                                         // vtable 0x01231710, RTTI .?AVCBaseClass@@
public:
    CBaseClass();                                         // 0x0099A2D0
    virtual ~CBaseClass();
};
class CThingCreatureBase;
class CGameCameraManager;
namespace NCreatureAction {
class CActionEventBase : public CBaseClass {
protected:
    CActionEventBase();
public:
    virtual ~CActionEventBase();
    bool Finished;                                        // +0x04
    long ActivationFrame;                                 // +0x08
    CThingCreatureBase* PThing;                           // +0x0C
    CGameCameraManager* PCameraManager;                   // +0x10
};
CActionEventBase::CActionEventBase()
    : Finished(false)
{
}
}