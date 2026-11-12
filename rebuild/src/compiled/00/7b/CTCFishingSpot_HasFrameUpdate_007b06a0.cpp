// CTCFishingSpot::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCFishingSpot@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B06A0, thiscall, `ret`.
// Identity: slot 8 of CTCFishingSpot's vtable 0x0126D51C (defined here: the slot holds this body in 1 vtable(s), all of CTCFishingSpot or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCFishingSpot@@UBE_NXZ; `ret` matches the signature.
class CTCFishingSpot {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCFishingSpot::HasFrameUpdate() const
{
    return true;
}
