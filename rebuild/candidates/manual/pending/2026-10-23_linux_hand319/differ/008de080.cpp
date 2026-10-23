// Grade: DIFFER(5v1437) -- first-pass stub only (one grade); the body is not reconstructed yet, see the header for the decoded shape
// CModeDragonStrafeBurninate::FrameUpdate() -- the dragon's strafing fire run: base mode update, follow the cardinal
//   spline (x87 position / fmod angles / AngleDifference), start and update the beam effect, look at the target, blend the
//   combination animation and remove the mode at the end of the run
// Leaf: FrameUpdate
// Stub: x87-heavy; first-pass skeleton only, not reconstructed yet.
class CModeBase {
public:
    virtual void FrameUpdate();                        // 0x0073AC50
};
class CModeDragonStrafeBurninate : public CModeBase {
public:
    virtual void FrameUpdate();
};
void CModeDragonStrafeBurninate::FrameUpdate()
{
    CModeBase::FrameUpdate();
}
