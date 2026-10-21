// CMainGameComponent::ProcessEventPackage(const CGameEventPackage* package) -- process each event of the package in order
// Leaf: ?ProcessEventPackage@CMainGameComponent@@IAEXPBVCGameEventPackage@@@Z
// Retail: Fable.exe @ 0x00416670, 54 bytes, `ret 4`; Ego_r.pdb ?ProcessEventPackage@CMainGameComponent@@IAEXPBVCGameEventPackage@@@Z;
// one caller (0x004172A3). Loops i over CGameEventPackage::GetNoEvents (0x009F16E0) calling
// CMainGameComponent::ProcessEvent(package->GetEvent(i)) (0x00415FE0 / 0x009F16D0), the count re-read every pass.
// Layout: only `this` and the package pointer are used; no members touched.
// Shape: /O2.
class CGameEvent;
class CGameEventPackage {
public:
    const CGameEvent* GetEvent(unsigned long index) const;   // 0x009F16D0
    unsigned long GetNoEvents() const;                   // 0x009F16E0
};
class CMainGameComponent {
protected:
    void ProcessEvent(const CGameEvent* event);          // 0x00415FE0
    void ProcessEventPackage(const CGameEventPackage* package);
    friend struct CMainGameComponent_Test;               // behaviour-test hook (both members are protected)
};
void CMainGameComponent::ProcessEventPackage(const CGameEventPackage* package)
{
    for (unsigned long i = 0; i < package->GetNoEvents(); ++i)
        ProcessEvent(package->GetEvent(i));
}