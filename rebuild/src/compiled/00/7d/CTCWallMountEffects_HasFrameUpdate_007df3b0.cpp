// CTCWallMountEffects::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCWallMountEffects@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DF3B0, thiscall, `ret`.
// Identity: slot 8 of CTCWallMountEffects's vtable 0x0126FC84 (defined here: the slot holds this body in 1 vtable(s), all of CTCWallMountEffects or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCWallMountEffects@@UBE_NXZ; `ret` matches the signature.
class CTCWallMountEffects {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCWallMountEffects::HasFrameUpdate() const
{
    return true;
}
