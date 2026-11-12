// CTCAtmosPlayer::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCAtmosPlayer@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E3790, thiscall, `ret`.
// Identity: slot 23 of CTCAtmosPlayer's vtable 0x01270484 (defined here: the slot holds this body in 1 vtable(s), all of CTCAtmosPlayer or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCAtmosPlayer@@UBE_NXZ; `ret` matches the signature.
class CTCAtmosPlayer {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCAtmosPlayer::IsComponentSerialisable() const
{
    return true;
}
