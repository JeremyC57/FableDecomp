// CTCBulletTime::CancelForCutscene() -- flags the bullet-time effect as cancelled by a cutscene and removes it from its thing
// Leaf: CancelForCutscene@CTCBulletTime_Methods
// Retail: Fable.exe @ 0x0079C660, 22 bytes; Ego_r.pdb ?CancelForCutscene@CTCBulletTime@@QAEXXZ. Fields per
// engine/CTCBulletTime.h (CutSceneCancel +0x56, Pausable +0x54). +0x04 is CTCBase::ParentThing (engine/CTCBase.h; the
// CTCBulletTime header calls it f4). Callee 0x004C9840 looks the type up in CThing::Components (+0x44, LowerBound 0x0040F020)
// and matches ?RemoveTC@CThing@@QAEXW4ETCInterfaceType@@@Z; 0x92 is CTCBulletTime's own interface type (slot-21 getter).
#include "engine/CTCBulletTime.h"
enum ETCInterfaceType { TCI_BULLET_TIME = 0x92 };
class CThing {
public:
    void RemoveTC(ETCInterfaceType type);                // 0x004C9840
};
struct CTCBulletTime_Methods : CTCBulletTime {
    CThing* GetParentThing() const { return (CThing*)f4; }
    void CancelForCutscene();
};
void CTCBulletTime_Methods::CancelForCutscene()
{
    CutSceneCancel = true;
    Pausable = false;
    GetParentThing()->RemoveTC(TCI_BULLET_TIME);
}