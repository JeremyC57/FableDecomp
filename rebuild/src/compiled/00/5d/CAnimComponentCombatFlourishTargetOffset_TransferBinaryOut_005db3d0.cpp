// CAnimComponentCombatFlourishTargetOffset::TransferBinaryOut(CDataOutputStream&) const -- write the flag, then the 2D offset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DB3D0, length 37 bytes (+ int3 padding). Vtable slot 7 of
// CAnimComponentCombatFlourishTargetOffset (0x01255760).
// Callees: CDataOutputStream::WriteBool(bool) @ 0x00993E30, CDataOutputStream::Write2DVector(const C2DVector&) @ 0x00993F90.
// Layout (engine/CAnimComponentCombatFlourishTargetOffset.h, PDB): vptr, ref count, bool TargetOffset at +0x08
// and the C2DVector TargetOffsetVector at +0x09 (unaligned: sizeof == 0x11, the class is packed), hence the
// `add esi,9 ; push esi` address of the vector. Modelled here with the real packing because the generated
// header splits the vector into two floats.
// Shape: the bool is widened through a cleared register and pushed; the stream reference stays in edi.
class C2DVector { public: float X, Y; };

class CDataOutputStream { public:
    void WriteBool(bool value);                 // 0x00993E30
    void Write2DVector(const C2DVector& v);     // 0x00993F90
};

#pragma pack(push, 1)
class CAnimComponentCombatFlourishTargetOffset { public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void v5(); virtual void v6();
    virtual void TransferBinaryOut(CDataOutputStream& stream) const;    // slot 7
    long m_refCount;                            // +0x04 CIVCountedPointeeBase
    bool TargetOffset;                          // +0x08
    C2DVector TargetOffsetVector;               // +0x09 (packed)
};
#pragma pack(pop)

void CAnimComponentCombatFlourishTargetOffset::TransferBinaryOut(CDataOutputStream& stream) const
{
    stream.WriteBool(TargetOffset);
    stream.Write2DVector(TargetOffsetVector);
}