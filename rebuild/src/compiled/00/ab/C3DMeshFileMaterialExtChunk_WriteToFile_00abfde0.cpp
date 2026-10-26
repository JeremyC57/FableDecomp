// C3DMeshFileMaterialExtChunk::WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const -- version 1 then SelfIllumination; the saver is unused and there is no base call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABFDE0, length 33 bytes.
// Callees: 0x00993EA0 CDataOutputStream::WriteULONG x2
//
#include "engine/C3DMeshFileMaterialExtChunk.h"   // PDB layout agrees with retail here (SelfIllumination +0x10)
class CChunkedFileSaver;
class CDataOutputStream { public:
public:
    void WriteULONG(unsigned long);   // 0x00993EA0
};
struct C3DMeshFileMaterialExtChunk_Methods : C3DMeshFileMaterialExtChunk {
    void WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const;
};
void C3DMeshFileMaterialExtChunk_Methods::WriteToFile(CDataOutputStream& out, CChunkedFileSaver& saver) const
{
    out.WriteULONG(1);
    out.WriteULONG(SelfIllumination);
}