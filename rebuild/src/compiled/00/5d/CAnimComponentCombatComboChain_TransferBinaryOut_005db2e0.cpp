// CAnimComponentCombatComboChain::TransferBinaryOut(CDataOutputStream& out) const -- write ComboStage then ComboID to the binary stream
// Leaf: ?TransferBinaryOut@CAnimComponentCombatComboChain@@UBEXAAVCDataOutputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DB2E0, 35 bytes, thiscall, `ret 4`.
// Identity (proven): slot 7 of vtable 0x012556E8 (RTTI .?AVCAnimComponentCombatComboChain@@); Ego_r.pdb ?TransferBinaryOut@CAnimComponentCombatComboChain@@UBEXAAVCDataOutputStream@@@Z;
//   Ego_r.pdb members ComboStage +0x8 / ComboID +0xC (long), the two fields this body writes.
// Callees: CDataOutputStream::WriteSLONG(long) 0x00993EB0 (twice).
// Shape (family M156, representative 0x005DB280): `out.WriteSLONG(ComboStage); out.WriteSLONG(ComboID);` -- the stream kept in edi.
class CDataOutputStream {
public:
    void WriteSLONG(long value);                             // 0x00993EB0
};
class CAnimComponentCombatComboChain {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void TransferBinaryOut(CDataOutputStream& out) const;   // slot 7
    unsigned char _pad04[4];
    long ComboStage;                                  // +0x8
    long ComboID;                                  // +0xC
};
void CAnimComponentCombatComboChain::TransferBinaryOut(CDataOutputStream& out) const
{
    out.WriteSLONG(ComboStage);
    out.WriteSLONG(ComboID);
}