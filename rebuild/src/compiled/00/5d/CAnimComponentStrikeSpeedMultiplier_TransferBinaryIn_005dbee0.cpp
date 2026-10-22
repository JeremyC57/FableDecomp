// CAnimComponentStrikeSpeedMultiplier::TransferBinaryIn(CDataInputStream& in) -- binary persist read of the 4-byte speed multiplier into the member at +8
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DBEE0, 108 bytes (+4 int3), __fastcall, ret 4.
// Callee: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0.
// Shape: the inlined CDataInputStream::Read(buffer, size) of the landed TransferBinaryIn family
// (0x007AD7F0): `position + size` widened to __int64 and range-checked against [0, 0x7FFFFFFF], a
// dword copy from the chunk pointer when enough bytes remain, else the overflow reader. The value
// local is homed in the dead `in` argument slot ([esp+8]) and then copied to the member through a
// register on every exit (the rejected-position exit copies the uninitialised slot, like retail).
// The multiplier is a float (PDB: CAnimComponentStrikeSpeedMultiplier::AnimationSpeedMultiplier, +8); a float
// copied without arithmetic moves through an integer register.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream {
public:
    void* m_pVtable;                       // +0x00
    unsigned long m_position;              // +0x04
    long m_sourceLength;                   // +0x08
    const unsigned char* m_pData;          // +0x0C current chunk read pointer
    long m_chunkPosition;                  // +0x10
    long m_remaining;                      // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

class CAnimComponentStrikeSpeedMultiplier {
public:
    virtual void v00();
    long IVRefCount;                             // +0x04 (base component data)
    float AnimationSpeedMultiplier;               // +0x08
    void TransferBinaryIn(CDataInputStream& in);
};

void CAnimComponentStrikeSpeedMultiplier::TransferBinaryIn(CDataInputStream& in)
{
    float value;
    in.Read(&value, sizeof(value));
    AnimationSpeedMultiplier = value;
}