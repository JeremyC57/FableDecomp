// CTCCreatureGeneratorCreator::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCCreatureGeneratorCreator@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C9E90, thiscall, `ret`.
// Identity: slot 8 of CTCCreatureGeneratorCreator's vtable 0x0126EB3C (defined here: the slot holds this body in 1 vtable(s), all of CTCCreatureGeneratorCreator or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCreatureGeneratorCreator@@UBE_NXZ; `ret` matches the signature.
class CTCCreatureGeneratorCreator {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCCreatureGeneratorCreator::HasFrameUpdate() const
{
    return true;
}
