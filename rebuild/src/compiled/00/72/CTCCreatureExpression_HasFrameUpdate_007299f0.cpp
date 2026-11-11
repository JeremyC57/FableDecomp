// CTCCreatureExpression::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCCreatureExpression@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007299F0, thiscall, `ret`.
// Identity: slot 8 of CTCCreatureExpression's vtable 0x012665CC (defined here: the slot holds this body in 1 vtable(s), all of CTCCreatureExpression or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCCreatureExpression@@UBE_NXZ; `ret` matches the signature.
class CTCCreatureExpression {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCCreatureExpression::HasFrameUpdate() const
{
    return true;
}
