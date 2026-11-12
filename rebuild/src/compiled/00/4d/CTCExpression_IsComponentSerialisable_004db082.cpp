// CTCExpression::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCExpression@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB082, thiscall, `ret`.
// Identity: slot 23 of CTCExpression's vtable 0x012401F4 (defined here: the slot holds this body in 1 vtable(s), all of CTCExpression or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCExpression@@UBE_NXZ; `ret` matches the signature.
class CTCExpression {
public:
    virtual bool IsComponentSerialisable() const;
};
bool CTCExpression::IsComponentSerialisable() const
{
    return true;
}
