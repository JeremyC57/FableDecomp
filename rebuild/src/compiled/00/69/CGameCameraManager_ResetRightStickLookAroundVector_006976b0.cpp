// CGameCameraManager::ResetRightStickLookAroundVector() -- zero both the current and the target right-stick look-around angles
// Leaf: ResetRightStickLookAroundVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006976B0, length 53 bytes.
// Callees: none.
// Identity: the manifest label CQ_GuildTrainingSkillScript::Init is propagated. The caller
// NCameraModes::CZTargetNew::OnInit 0x00872A20 calls it on its camera manager (ecx = mode->+0x40), and
// Ego_r.pdb has ?ResetRightStickLookAroundVector@CGameCameraManager@@QAEXXZ over the two C2DVector members
// RightStickLookAroundAngles / RightStickTargetLookAroundAngles (PDB +0xd0/+0xd8 -> retail +0xc4/+0xcc).
// The engine header CGameCameraManager.h carries the PDB offsets, so the class is modelled locally.
// Shape: the chained assignment `a = b = C2DVector(0, 0)`: one 8-byte frame temporary written with immediate
// zero stores and read back once into eax/edx, the target stored first.
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};

struct CGameCameraManager {
    unsigned char _pad00[0xc4];
    C2DVector     RightStickLookAroundAngles;               // +0xc4
    C2DVector     RightStickTargetLookAroundAngles;         // +0xcc
    void ResetRightStickLookAroundVector();
};

void CGameCameraManager::ResetRightStickLookAroundVector()
{
    RightStickLookAroundAngles = RightStickTargetLookAroundAngles = C2DVector(0.0f, 0.0f);
}