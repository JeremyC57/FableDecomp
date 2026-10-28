// CDataInputStream::Read3DAngles() -- read the XY, YZ then XZ angles and return the 3D angles by value
// Leaf: ?Read3DAngles@CDataInputStream@@QAE?AVC3DAngles@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00995210, 299 bytes, `ret 4` (thiscall on the stream, hidden result pointer).
// Identity: Ego_r.pdb reader order Read2DVector / Read3DVector / Read3DAngles / ReadMatrix3x4 is retail's 0x00995030 /
//   0x009950F0 / 0x00995210 / 0x00995340 (the first two landed, the last labelled); C3DAngles is 12 bytes (three floats).
//   The manifest's "CPersistTraits<CBrainUpdateZone>::TransferBinaryIn" is propagated (the PDB one is
//   QAEX(CMemoryDataInputStream&, CBrainUpdateZone&), `ret 8`); no direct callers or absolute references in retail.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (x3, the slow path of the inlined Read).
// Layout: Ego_r.pdb CDataInputStream (StreamPos +4, CurrentSourcePtr +0xC, SourceChunkBytesRemaining +0x14); modelled
//   locally because the method has to mangle on CDataInputStream itself (engine/CDataInputStream.h carries no methods).
// Shape: three inlined Read calls through one 4-byte frame local, each value stored into a C3DAngles local (copied
//   through GPRs) that is returned by value.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class C3DAngles { public:
    float AngleXY;
    float AngleYZ;
    float AngleXZ;
};

class CDataInputStream { public:
    virtual unsigned long GetPosition() const;          // slot 0 (vptr +0x00)
    unsigned long StreamPos;                            // +0x04
    long          StreamSize;                           // +0x08
    unsigned char* CurrentSourcePtr;                    // +0x0C
    unsigned long SourceChunkStreamPos;                 // +0x10
    long          SourceChunkBytesRemaining;            // +0x14
    C3DAngles Read3DAngles();
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
};

C3DAngles CDataInputStream::Read3DAngles()
{
    C3DAngles angles;
    angles.AngleXY = ReadFloat();
    angles.AngleYZ = ReadFloat();
    angles.AngleXZ = ReadFloat();
    return angles;
}