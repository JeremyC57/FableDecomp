// behaviour test for 0x0079C660
#include <stdio.h>
#include <string.h>
struct CTCBulletTime { void* vt; long f4; unsigned char _pad[0x54 - 8]; bool Pausable; bool Paused; bool CutSceneCancel; unsigned char _p; };
enum ETCInterfaceType { TCI_BULLET_TIME = 0x92 };
class CThing {
public:
    int removed;
    void RemoveTC(ETCInterfaceType type) { removed = type; }
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
int main()
{
    CThing t; t.removed = 0; CTCBulletTime_Methods b; memset(&b, 0, sizeof(b)); b.f4 = (long)&t; b.Pausable = true;
    b.CancelForCutscene();
    int ok = b.CutSceneCancel && !b.Pausable && !b.Paused && t.removed == 0x92;
    printf(ok ? "0079c660_TEST PASS\n" : "0079c660_TEST FAIL\n");
    return ok ? 0 : 1;
}