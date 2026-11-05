// C3DClothPrimitive::SaveBinary(CDataOutputStream& out) const -- primitive and material index, then the particle program serialised into a memory stream and written as size + bytes
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC3B00, 128 bytes. Callees: CDataOutputStream::WriteULONG 0x00993EA0,
// CMemoryDataOutputStream::CMemoryDataOutputStream 0x00996AF0 / ~CMemoryDataOutputStream 0x00996B30,
// CParticleProgram::WriteToFile 0x00AE2230, CMemoryDataOutputStream::GetLength 0x00994B40,
// CMemoryDataOutputStream::PeekData 0x00995E70, CDataOutputStream::WriteData(data, size) 0x00996610.
// Layout from the PDB (C3DClothPrimitive: ParticleProgram CCountedPointer +4 {object, block}, PrimitiveIndex
// +0xC, MaterialIndex +0x10). The 0x18-byte memory stream is a frame local; the last call's arguments are
// evaluated right to left (the size first, then the data pointer). Manifest labels of 0x00994B40
// ("CCreatureActionBase::GetFramesRemaining") and 0x00996B30 are propagated.
class CDataOutputStream { public:
    virtual void v00();
    void WriteULONG(unsigned long value);                       // 0x00993EA0
    void WriteData(const void* data, unsigned long size);       // 0x00996610
};

class CMemoryDataOutputStream : public CDataOutputStream {
public:
    unsigned long GetLength() const;   // 0x00994B40 (Ego_r.pdb ?GetLength@CMemoryDataOutputStream@@QBEKXZ)
private: public:
    unsigned char _body[0x14];                                  // +0x04 .. +0x17
    CMemoryDataOutputStream();                                  // 0x00996AF0
    virtual ~CMemoryDataOutputStream();                                 // 0x00996B30
    void const* PeekData() const;                                           // 0x00995E70
};

class CParticleProgram { public:
    void WriteToFile(CDataOutputStream& out);                   // 0x00AE2230
};

struct SCountBlock;
#include "engine/CCountedPointer.h"

class C3DClothPrimitive { public:
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
    out.WriteULONG(((CMemoryDataOutputStream*)&stream)->GetLength());
    out.WriteData(((void*)stream.PeekData()), ((CMemoryDataOutputStream*)&stream)->GetLength());
}