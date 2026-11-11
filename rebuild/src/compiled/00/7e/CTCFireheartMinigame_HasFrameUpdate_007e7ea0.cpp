// CTCFireheartMinigame::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCFireheartMinigame@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E7EA0, thiscall, `ret`.
// Identity: slot 8 of CTCFireheartMinigame's vtable 0x012709CC (defined here: the slot holds this body in 1 vtable(s), all of CTCFireheartMinigame or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCFireheartMinigame@@UBE_NXZ; `ret` matches the signature.
class CTCFireheartMinigame {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCFireheartMinigame::HasFrameUpdate() const
{
    return true;
}
