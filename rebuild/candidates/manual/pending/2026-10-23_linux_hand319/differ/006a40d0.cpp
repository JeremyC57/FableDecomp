// Grade: DIFFER(13v1579) -- first-pass stub only (one grade); the body is not reconstructed yet, see the header for the decoded shape
// CInputProcessJumpingAndRolling::ProcessRolling(const CInputEvent& event, CProcessedInput& processed) -- the hero's
//   roll: when the roll button is held / queued, project the stick direction onto the camera axes (C2DVector::Dot x3),
//   pick the roll direction event and add the roll game events (Copy of the direction floats) or the exclusive observer
// Leaf: ProcessRolling
// Flags: /GS
// Stub: x87-heavy, two cookie exits; first-pass skeleton only, not reconstructed yet.
class CInputEvent;
class CProcessedInput;
class CInputProcessGameBase {
public:
    bool IsEventButtonHeld(const CInputEvent& event);  // 0x00687BD0
};
class CInputProcessJumpingAndRolling : public CInputProcessGameBase {
public:
    void ProcessRolling(const CInputEvent& event, CProcessedInput& processed);
};
void CInputProcessJumpingAndRolling::ProcessRolling(const CInputEvent& event, CProcessedInput& processed)
{
    if (!IsEventButtonHeld(event))
        return;
}
