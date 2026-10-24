// NUISystem::CChangingStateComponent::ChangeState(unsigned long state, float parentUpdateTime) -- record the parent's update time, then change state through the one-argument overload
// Leaf: ?ChangeState@CChangingStateComponent@NUISystem@@UAEXKM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041C5A0, 26 bytes (`ret 8`).
// Callee: own virtual slot 48 (+0xc0), ChangeState(unsigned long).
// Label: the catalog "NUISystem::CSprite::vfunc_47" is the first vtable (0x0122F5D4 slot 47) listing this body;
// Ego_r.pdb names ?ChangeState@CChangingStateComponent@NUISystem@@UAEXKM@Z (beside ?HasState@..@UAE_NK@Z, the
// 0x0041C5C0 row, in the same PDB neighbourhood).
// Layout: retail CChangingStateComponent tail (as the landed 0x0052C8B0): ParentUpdateTime +0x140.
// Shape: the float is copied as a raw dword (eax) before the argument push; the vtable is fetched first.
namespace NUISystem {
class CChangingStateComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046();
    // VC7.1 lays overloaded virtuals out in REVERSE declaration order: declared (K) then (K, float),
    // the vtable holds (K, float) at slot 47 and (K) at slot 48 (declared the other way round the call goes to +0xbc).
    virtual void ChangeState(unsigned long state);                           // slot 48 (+0xc0)
    virtual void ChangeState(unsigned long state, float parentUpdateTime);   // slot 47 (+0xbc)
    unsigned char _pad04[0x140 - 4];
    float ParentUpdateTime;                                                  // +0x140
};

void CChangingStateComponent::ChangeState(unsigned long state, float parentUpdateTime)
{
    ParentUpdateTime = parentUpdateTime;
    ChangeState(state);
}
}