// CTCKrakenTentacle::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCKrakenTentacle@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB363, thiscall, `ret`.
// Identity: slot 8 of CTCKrakenTentacle's vtable 0x012407BC (defined here: the slot holds this body in 1 vtable(s), all of CTCKrakenTentacle or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCKrakenTentacle@@UBE_NXZ; `ret` matches the signature.
class CTCKrakenTentacle {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCKrakenTentacle::HasFrameUpdate() const
{
    return true;
}
