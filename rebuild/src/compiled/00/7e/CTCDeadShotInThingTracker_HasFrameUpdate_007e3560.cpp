// CTCDeadShotInThingTracker::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCDeadShotInThingTracker@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E3560, thiscall, `ret`.
// Identity: slot 8 of CTCDeadShotInThingTracker's vtable 0x0127040C (defined here: the slot holds this body in 1 vtable(s), all of CTCDeadShotInThingTracker or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCDeadShotInThingTracker@@UBE_NXZ; `ret` matches the signature.
class CTCDeadShotInThingTracker {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCDeadShotInThingTracker::HasFrameUpdate() const
{
    return true;
}
