// CTCCreatureItemLevitationBase::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCCreatureItemLevitationBase@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE6E9, thiscall, `ret`.
// Identity: slot 8 of CTCCreatureItemLevitationHero's vtable 0x0124167C (defined here: the slot holds this body in 3 vtable(s), all of CTCCreatureItemLevitationBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCreatureItemLevitationBase@@UBE_NXZ; `ret` matches the signature.
class CTCCreatureItemLevitationBase {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCCreatureItemLevitationBase::HasFrameUpdate() const
{
    return true;
}
