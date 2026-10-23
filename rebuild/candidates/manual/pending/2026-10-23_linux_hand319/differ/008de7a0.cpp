// Grade: DIFFER(5v1239) -- first-pass stub only (one grade); the body is not reconstructed yet, see the header for the decoded shape
// CModeIceTrollGroundSmashIcicleAssault::FrameUpdate() -- the ice troll's icicle assault: base mode update, then on the
//   smash frame pick the best icicle points around the target (CGetBestIciclePointForTarget, a partitioned C3DVector
//   list), create the icicle traps (immune to the troll, manually triggered) with randomised orientation, set the
//   decapitation-response combat action and record the icicles
// Leaf: FrameUpdate
// Stub: first-pass skeleton only (the base update); x87-heavy 0x160 aligned frame, not reconstructed yet.
class CModeBase {
public:
    virtual void FrameUpdate();                        // 0x0073AC50
};
class CModeIceTrollGroundSmashIcicleAssault : public CModeBase {
public:
    virtual void FrameUpdate();
};
void CModeIceTrollGroundSmashIcicleAssault::FrameUpdate()
{
    CModeBase::FrameUpdate();
}
