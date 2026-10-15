// C3DClothPrimitive::SaveBinary(CDataOutputStream& out) const -- primitive and material index, then the particle program serialised into a memory stream and written as size + bytes
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC3B00, 128 bytes. Callees: CDataOutputStream::WriteULONG 0x00993EA0,
// CMemoryDataOutputStream::CMemoryDataOutputStream 0x00996AF0 / ~CMemoryDataOutputStream 0x00996B30,
// CParticleProgram::WriteToFile 0x00AE2230, CMemoryDataOutputStream::GetSize 0x00994B40,
// CMemoryDataOutputStream::PeekData 0x00995E70, CDataOutputStream::WriteData(data, size) 0x00996610.
// Layout from the PDB (C3DClothPrimitive: ParticleProgram CCountedPointer +4 {object, block}, PrimitiveIndex
// +0xC, MaterialIndex +0x10). The 0x18-byte memory stream is a frame local; the last call's arguments are
// evaluated right to left (the size first, then the data pointer). Manifest labels of 0x00994B40
// ("CCreatureActionBase::GetFramesRemaining") and 0x00996B30 are propagated.
struct CDataOutputStream {
    virtual void v00();
    void WriteULONG(unsigned long value);                       // 0x00993EA0
    void WriteData(const void* data, unsigned long size);       // 0x00996610
};

struct CMemoryDataOutputStream : CDataOutputStream {
    unsigned char _body[0x14];                                  // +0x04 .. +0x17
    CMemoryDataOutputStream();                                  // 0x00996AF0
    ~CMemoryDataOutputStream();                                 // 0x00996B30
    unsigned long GetSize();                                    // 0x00994B40
    void* PeekData();                                           // 0x00995E70
};

struct CParticleProgram {
    void WriteToFile(CDataOutputStream& out);                   // 0x00AE2230
};

struct SCountBlock;
template <class T>
struct CCountedPointer {
    T* m_pObject;                           // +0
    SCountBlock* m_pCount;                  // +4
    T* operator->() const { return m_pObject; }
};

struct C3DClothPrimitive {
    virtual void v00();
    CCountedPointer<CParticleProgram> ParticleProgram;          // +0x04
    unsigned long PrimitiveIndex;                               // +0x0C
    unsigned long MaterialIndex;                                // +0x10
    void SaveBinary(CDataOutputStream& out) const;
};

void C3DClothPrimitive::SaveBinary(CDataOutputStream& out) const
{
    out.WriteULONG(PrimitiveIndex);
    out.WriteULONG(MaterialIndex);
    CMemoryDataOutputStream stream;
    ParticleProgram->WriteToFile(stream);
    out.WriteULONG(stream.GetSize());
    out.WriteData(stream.PeekData(), stream.GetSize());
}