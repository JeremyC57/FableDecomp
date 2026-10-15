// CPhysicalPrimitiveCylinder::TransferBinaryOut(CDataOutputStream*) -- writes the base-name def index, the radius and the height to the binary stream
// Leaf: TransferBinaryOut
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00815010, 46 bytes.
// Callees: 0x00993EB0 CDataOutputStream 4-byte writer (long), 0x00993EE0 CDataOutputStream 4-byte
// writer (float), both `this->Write(&value, 4)` through vtable +0x10.
//
// PDB layout: CPhysicalPrimitiveCylinder (16 bytes) = { vptr; CDefString BaseName (+0x4, one long
// TablePos); float Radius (+0x8); float Height (+0xC) }. No engine header exists for the class, so
// it is modelled locally with those names. The stream (argument) is kept in edi, `this` in esi; the
// two floats are pushed as dwords (plain copies). Same shape as the landed sibling 0x00814C10.
struct CDataOutputStream {
    void WriteLong(long value);            // 0x00993EB0
    void WriteFloat(float value);          // 0x00993EE0
};

struct CDefString {
    long TablePos;
};

struct CPhysicalPrimitiveCylinder {
    virtual void TransferBinaryOut(CDataOutputStream* stream);
    CDefString BaseName;                   // +0x4
    float      Radius;                     // +0x8
    float      Height;                     // +0xC
};

void CPhysicalPrimitiveCylinder::TransferBinaryOut(CDataOutputStream* stream)
{
    stream->WriteLong(BaseName.TablePos);
    stream->WriteFloat(Radius);
    stream->WriteFloat(Height);
}
