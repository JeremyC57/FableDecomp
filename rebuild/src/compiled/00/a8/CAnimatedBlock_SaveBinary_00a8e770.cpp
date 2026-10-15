// CAnimatedBlock::SaveBinary(CDataOutputStream* stream) const -- writes the skinned mesh block: the base block (two words, two flags, a byte), the vertex count, bones per vertex, the paletted flag, the group count and the raw group bytes
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8E770, 140 bytes, __fastcall, ret 4.
// Callees (CDataOutputStream, landed): WriteULong 0x00993EA0, WriteBool 0x00993E30, WriteUChar 0x00993E50,
// WriteUShort 0x00993E80, and the stream's virtual Write(const void*, long) (slot 4, `call [eax+10h]`).
// Layout: Ego_r.pdb CAnimatedBlock -- the unnamed 12-byte base block (as in the landed sibling
// CStaticBlock::SaveBinary 0x00A8E710: two words, flags +8 / +9, byte +0xA), VertexCount +0x10,
// BonesPerVertex +0x14 (ushort), PalettedFlag +0x16, GroupCount +0x17, Groups[18] +0x18.
// Shape: identical to the CStaticBlock twin for the base block (each byte argument zero-extended into a
// fresh register); the group bytes go out through the stream's own virtual Write with the count widened
// to `long` (`movzx`), the Groups address formed last with `add esi,18h`.
class CDataOutputStream {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long size);   // slot 4
    void WriteULong(unsigned long value);       // 0x00993EA0
    void WriteBool(bool value);                 // 0x00993E30
    void WriteUChar(unsigned char value);       // 0x00993E50
    void WriteUShort(unsigned short value);     // 0x00993E80
};

struct CAnimatedBlock {
    unsigned long  m_firstIndex;                // +0x00 (base block)
    unsigned long  m_count;                     // +0x04
    bool           m_flagA;                     // +0x08
    bool           m_flagB;                     // +0x09
    unsigned char  m_type;                      // +0x0A
    unsigned long  BoneConstantsFixupOffset;    // +0x0C
    unsigned long  VertexCount;                 // +0x10
    unsigned short BonesPerVertex;              // +0x14
    bool           PalettedFlag;                // +0x16
    unsigned char  GroupCount;                  // +0x17
    unsigned char  Groups[18];                  // +0x18
    void SaveBinary(CDataOutputStream* stream) const;
};

void CAnimatedBlock::SaveBinary(CDataOutputStream* stream) const
{
    stream->WriteULong(m_firstIndex);
    stream->WriteULong(m_count);
    stream->WriteBool(m_flagA);
    stream->WriteUChar(m_type);
    stream->WriteBool(m_flagB);
    stream->WriteULong(VertexCount);
    stream->WriteUShort(BonesPerVertex);
    stream->WriteBool(PalettedFlag);
    stream->WriteUChar(GroupCount);
    stream->Write(Groups, GroupCount);
}