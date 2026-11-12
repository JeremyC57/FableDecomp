// CTCCreatureGeneratorCreator::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCCreatureGeneratorCreator@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C9EA0, thiscall, `ret`.
// Identity: slot 23 of CTCCreatureGeneratorCreator's vtable 0x0126EB3C (defined here: the slot holds this body in 1 vtable(s), all of CTCCreatureGeneratorCreator or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCCreatureGeneratorCreator@@MBE_NXZ; `ret` matches the signature.
class CTCCreatureGeneratorCreator {
public:
protected:
    virtual bool IsComponentSerialisable() const;
};
bool CTCCreatureGeneratorCreator::IsComponentSerialisable() const
{
    return true;
}
