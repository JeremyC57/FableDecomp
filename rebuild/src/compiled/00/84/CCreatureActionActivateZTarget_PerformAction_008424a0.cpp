// CCreatureAction_ActivateZTarget::PerformAction() -- put the action's player into player mode 2 (the Z-target mode), not exclusively
// Leaf: ?PerformAction@CCreatureAction_ActivateZTarget@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008424A0, 17 bytes, thiscall, plain `ret`.
// Identity (proven): RTTI vtable 0x012738EC (.?AVCCreatureAction_ActivateZTarget@@, bases CCreatureActionBase / CBaseObject /
//   CBaseClass) slot 3 is this body; slot 3 is PerformAction in every CCreatureActionBase vtable (22 landed rows); Ego_r.pdb and
//   FableWin.pdb both hold ?PerformAction@CCreatureAction_ActivateZTarget@@UAEXXZ. The catalog label is right.
// Callees: CCreatureActionBase::GetPlayer() 0x00692930 (Ego_r / FableWin ?GetPlayer@CCreatureActionBase@@IAEAAVCPlayer@@XZ:
//   protected, returns CPlayer&; its body is PCreature.GetPItem() then a tail jump), CPlayer::AddMode(EPlayerMode, bool)
//   0x006345C0 (Ego_r / FableWin ?AddMode@CPlayer@@QAEXW4EPlayerMode@@_N@Z).
// Shape: VC pushes both arguments before it evaluates the object expression, then a plain call / ret (same shape as the landed
//   sibling CCreatureAction_LookThroughWindow::PerformAction 0x008424D0, mode 16). The EPlayerMode enumerator names are not in
//   any name set: the value 2 is written as a cast.
// Layout: engine/CPlayer.h and engine/CBaseClass.h are generated flat headers (no member functions / vptr as data); only the
//   AddMode declaration and the polymorphic CBaseClass -> CBaseObject -> CCreatureActionBase chain (RTTI of 0x012738EC) are needed.
enum EPlayerMode { };

class CPlayer {
public:
    void AddMode(EPlayerMode mode, bool exclusive);       // 0x006345C0
};

class CBaseClass {
public:
    virtual ~CBaseClass();                                // slot 0 (the ??_G)
};
class CBaseObject : public CBaseClass {
public:
};
class CCreatureActionBase : public CBaseObject {
public:
    virtual void v01();
    virtual void v02();
    virtual void PerformAction();                         // slot 3
protected:
    CPlayer& GetPlayer();                                 // 0x00692930
};

class CCreatureAction_ActivateZTarget : public CCreatureActionBase {
public:
    virtual void PerformAction();
};

void CCreatureAction_ActivateZTarget::PerformAction()
{
    GetPlayer().AddMode((EPlayerMode)2, false);
}