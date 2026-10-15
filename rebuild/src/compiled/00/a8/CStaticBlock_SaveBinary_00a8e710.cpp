// CStaticBlock::SaveBinary(CDataOutputStream*) const -- writes the static mesh block: the two base words, the two base flags and the base byte, then the material index
// Leaf: SaveBinary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8E710, 84 bytes (+12 int3 padding). Callees (CDataOutputStream, all
// landed, each a `Write(&value, size)` through vtable slot 4): WriteULong @ 0x00993EA0, WriteBool @ 0x00993E30
// (normalises to 0/1), the 1-byte WriteUChar @ 0x00993E50, WriteLong @ 0x00993EB0.
// Layout: PDB CStaticBlock is 0x20 bytes; its first 12 bytes are an unnamed base block (two words and three bytes:
// +0x0, +0x4, flags +0x8 / +0x9, byte +0xA), MaterialIndex at +0x1C (the texture fix-up offsets and texture
// pointers at +0xC..+0x18 are not saved here). The base members are named descriptively.
// Shape: the stream is kept in edi and `this` in esi; VC7.1 pushes each byte argument (bool / unsigned char) zero-
// extended into a fresh register (`xor reg,reg ; mov reg8,[esi+n]`); the writes follow the source order.
class CDataOutputStream {
public:
    void WriteULong(unsigned long value);       // 0x00993EA0
    void WriteBool(bool value);                 // 0x00993E30
    void WriteUChar(unsigned char value);       // 0x00993E50
    void WriteLong(long value);                 // 0x00993EB0
};

struct CTexture;

struct CStaticBlock {
    unsigned long m_firstIndex;                 // +0x00 (base block)
    unsigned long m_count;                      // +0x04
    bool          m_flagA;                      // +0x08
    bool          m_flagB;                      // +0x09
    unsigned char m_type;                       // +0x0A
    unsigned long DiffuseTextureFixupOffset;    // +0x0C
    unsigned long BumpTextureFixupOffset;       // +0x10
    CTexture*     DiffuseTexture;               // +0x14
    CTexture*     BumpTexture;                  // +0x18
    long          MaterialIndex;                // +0x1C
    void SaveBinary(CDataOutputStream* stream) const;
};

void CStaticBlock::SaveBinary(CDataOutputStream* stream) const
{
    stream->WriteULong(m_firstIndex);
    stream->WriteULong(m_count);
    stream->WriteBool(m_flagA);
    stream->WriteUChar(m_type);
    stream->WriteBool(m_flagB);
    stream->WriteLong(MaterialIndex);
}