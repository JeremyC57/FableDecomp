// CReactionFrequencyTraits_LinearTime::TransferBinaryOut(CDataOutputStream& out) const -- write MinWait then WaitRange to the binary stream
// Leaf: ?TransferBinaryOut@CReactionFrequencyTraits_LinearTime@@UBEXAAVCDataOutputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006FED00, 35 bytes, thiscall, `ret 4`.
// Identity (proven): slot 4 of vtable 0x01264B64 (RTTI .?AVCReactionFrequencyTraits_LinearTime@@); Ego_r.pdb ?TransferBinaryOut@CReactionFrequencyTraits_LinearTime@@UBEXAAVCDataOutputStream@@@Z;
//   Ego_r.pdb members MinWait +0x4 / WaitRange +0x8 (float), the two fields this body writes.
// Callees: CDataOutputStream::WriteFloat(float) 0x00993EE0 (twice).
// Shape (family M156, representative 0x005DB280): `out.WriteFloat(MinWait); out.WriteFloat(WaitRange);` -- the stream kept in edi.
class CDataOutputStream {
public:
    void WriteFloat(float value);                             // 0x00993EE0
};
class CReactionFrequencyTraits_LinearTime {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void TransferBinaryOut(CDataOutputStream& out) const;   // slot 4
    float MinWait;                                  // +0x4
    float WaitRange;                                  // +0x8
};
void CReactionFrequencyTraits_LinearTime::TransferBinaryOut(CDataOutputStream& out) const
{
    out.WriteFloat(MinWait);
    out.WriteFloat(WaitRange);
}