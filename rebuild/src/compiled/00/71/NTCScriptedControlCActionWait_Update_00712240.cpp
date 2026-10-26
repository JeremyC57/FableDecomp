// NTCScriptedControl::CActionWait::Update() -- count one more update and terminate the wait once a positive frame budget has been exceeded
// Leaf: ?Update@CActionWait@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00712240, 25 bytes. Virtual slot 1 of CActionWait's vtable; FableWin.pdb lists
// ?Update@CActionWait@NTCScriptedControl@@UAEXXZ.
// Layout (Ego_r.pdb, retail-consistent): CActionBase {vptr, PTCScript +4, Activated +8, Terminated +9};
// CActionWait NoFramesToWait +0x0c, UpdateCounter +0x10. No engine header exists for these classes.
// Shape: the pre-incremented counter is stored first and its value kept in edx for the compare; `NoFramesToWait > 0` tested
// before `UpdateCounter > NoFramesToWait` (both signed `jle`).
class CTCScriptedControl;

namespace NTCScriptedControl {
class CActionBase {
public:
    virtual ~CActionBase();
    virtual void Update();                         // slot 1
    CTCScriptedControl* PTCScript;                 // +0x04
    bool                Activated;                 // +0x08
    bool                Terminated;                // +0x09
};

class CActionWait : public CActionBase {
public:
    virtual void Update();
    long NoFramesToWait;                           // +0x0c
    long UpdateCounter;                            // +0x10
};

void CActionWait::Update()
{
    ++UpdateCounter;
    if (NoFramesToWait > 0 && UpdateCounter > NoFramesToWait)
        Terminated = true;
}
}