// CContainmentVolume::SaveBinary(CDataOutputStream& stream) const -- write the plane count, then
// the plane array as one block
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A75400, length 40 bytes (+ int3 padding).
// Callees: CDataOutputStream::WriteLong(long) @ 0x00993EA0 (forwards 4 bytes of its argument to
// the stream's vtable slot 4; the manifest's "WriteCBYTE" label is propagated),
// CDataOutputStream::WriteBuffer(const void*, long) @ 0x00996610 (the "PersistData_CompressAndVerify"
// label is propagated). engine/CContainmentVolume.h (PDB): CPlane* ContainmentPlanes +0x04,
// unsigned long Size +0x08; the byte count is Size << 4 (sizeof(CPlane) = 16).
// engine/CDataOutputStream.h exists (vptr + NewLine string) but only the two methods are needed.
#include "engine/CContainmentVolume.h"

struct CDataOutputStream_Calls {
    void WriteLong(long value);                        // 0x00993EA0
    void WriteBuffer(const void* data, long size);     // 0x00996610
};

struct CContainmentVolume_Methods : CContainmentVolume {
    void SaveBinary(CDataOutputStream_Calls& stream) const;
};

void CContainmentVolume_Methods::SaveBinary(CDataOutputStream_Calls& stream) const
{
    stream.WriteLong(Size);
    stream.WriteBuffer(ContainmentPlanes, Size * 16);
}