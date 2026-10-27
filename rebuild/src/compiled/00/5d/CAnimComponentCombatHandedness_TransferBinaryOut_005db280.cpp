// CAnimComponentCombatHandedness::TransferBinaryOut(CDataOutputStream& out) const -- write StartHandedness then EndHandedness to the binary stream
// Leaf: ?TransferBinaryOut@CAnimComponentCombatHandedness@@UBEXAAVCDataOutputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DB280, 35 bytes, thiscall, `ret 4`.
// Identity (proven): slot 7 of vtable 0x012556C0 (RTTI .?AVCAnimComponentCombatHandedness@@); Ego_r.pdb ?TransferBinaryOut@CAnimComponentCombatHandedness@@UBEXAAVCDataOutputStream@@@Z;
//   Ego_r.pdb members StartHandedness +0x8 / EndHandedness +0xC (ECombatAnimationHandedness), the two fields this body writes.
// Callees: CDataOutputStream::WriteSLONG(long) 0x00993EB0 (twice).
// Shape (family M156, representative 0x005DB280): `out.WriteSLONG(StartHandedness); out.WriteSLONG(EndHandedness);` -- the stream kept in edi.
class CDataOutputStream {
public:
    void WriteSLONG(long value);                             // 0x00993EB0
};
enum ECombatAnimationHandedness { COMBAT_ANIMATION_HANDEDNESS_NONE = 0 };
class CAnimComponentCombatHandedness {
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
    ECombatAnimationHandedness StartHandedness;                                  // +0x8
    ECombatAnimationHandedness EndHandedness;                                  // +0xC
};
void CAnimComponentCombatHandedness::TransferBinaryOut(CDataOutputStream& out) const
{
    out.WriteSLONG(StartHandedness);
    out.WriteSLONG(EndHandedness);
}