// C3DAnimFileSequenceChunk::ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&) -- read the bone indices, name, frame rate and keyframe matrices, the optional bone type, then the sub-chunks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAF850, length 657 bytes (virtual, chunk reader slot, ret 0Ch).
// Callees: CDataInputStream::GetPosition (vtable slot 2) x4, CDataInputStream::ReadNullTerminatedString (vtable slot 6,
//          hidden CCharString result), CCharString::operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0,
//          CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0, CDataInputStream::ReadMatrix3x4 @ 0x00995340,
//          CArray<CMatrix3x4>::erase @ 0x00AAFD10, CArray<CMatrix3x4>::Reserve @ 0x00AAFD90,
//          CArray<CMatrix3x4>::InsertGrow @ 0x00AAFE50, CChunkedFileSuperChunk::ReadFromFile @ 0x00AA8830.
// Shape: the mirror of the landed WriteToFile 0x00AAF650. end = GetPosition() + chunkSize (kept in the chunkSize slot);
// BoneIndex, ParentIndex, BoneName, SamplesPerSecond; Matrices.Clear() (the inline erase(begin, end) + Reserve(0)
// pair of the landed 0x00AAF770); a counted loop of push_back(ReadMatrix3x4()) (the push_back of the landed
// 0x00AAF800); BoneType defaults to 0 and is read as one byte while bytes remain; the base reader only while bytes
// remain, with end - GetPosition().
// Layout: retail C3DMeshFileChunk base is 0x2c bytes (PDB 0x34): BoneIndex +0x2c, ParentIndex +0x30, BoneName +0x34,
// SamplesPerSecond +0x38, BoneType +0x3c (a 4-byte enum: retail stores a dword), Matrices +0x40 (12-byte vector).
// Leaf: ReadFromFile@C3DAnimFileSequenceChunk
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
inline void* operator new(unsigned int, void* where) { return where; }

class CChunkedFileLoader;
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};
class CMatrix3x4 { public: float m_row[3][4]; };                 // 48 bytes

class CDataInputStream { public:
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();                  // slot 2 (+0x08)
    virtual void v3(); virtual void v4(); virtual void v5();
    virtual CCharString ReadNullTerminatedString();       // slot 6 (+0x18)
    CMatrix3x4 ReadMatrix3x4();                           // 0x00995340 (by value through the hidden pointer)
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC
    long m_10;
    long m_remaining;                      // +0x14
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
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
    __forceinline float ReadFloat() { float value; Read(&value, sizeof(value)); return value; }
    __forceinline unsigned char ReadUByte() { unsigned char value; Read(&value, sizeof(value)); return value; }
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};

struct SScalarTag { SScalarTag() { } };
struct CMatrixArray {
    CMatrix3x4* first;
    CMatrix3x4* last;
    CMatrix3x4* end;
    CMatrix3x4* erase(CMatrix3x4* from, CMatrix3x4* to);  // 0x00AAFD10
    void Reserve(unsigned long count);                    // 0x00AAFD90
    void InsertGrow(CMatrix3x4* where, const CMatrix3x4& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00AAFE50
    void Clear(unsigned long reserve = 0) { erase(first, last); Reserve(reserve); }
    void push_back(const CMatrix3x4& value)
    {
        if (last != end) {
            new (last) CMatrix3x4(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
enum EBoneType { BONE_TYPE_DEFAULT = 0 };

class CChunkedFileSuperChunk { public:
    virtual void v0();
    unsigned char _base_body[0x28];        // +0x04 .. +0x2b (retail C3DMeshFileChunk size 0x2c)
    void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);   // 0x00AA8830
};
class C3DAnimFileSequenceChunk : public CChunkedFileSuperChunk { public:
    long BoneIndex;                        // +0x2c
    long ParentIndex;                      // +0x30
    CCharString BoneName;                  // +0x34
    float SamplesPerSecond;                // +0x38
    EBoneType BoneType;                    // +0x3c
    CMatrixArray Matrices;                 // +0x40
    virtual void ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader);
};

#pragma auto_inline(off)
// Same-TU stand-in for the growth path (reads the value only through its const reference, never the tag).
void CMatrixArray::InsertGrow(CMatrix3x4* where, const CMatrix3x4& value, const SScalarTag& tag, unsigned long count, bool noTail)
{
    if (*(void**)&value != where)
        return;
}
#pragma auto_inline(on)

void C3DAnimFileSequenceChunk::ReadFromFile(CDataInputStream& in, unsigned long chunkSize, CChunkedFileLoader& loader)
{
    unsigned long end = in.GetPosition() + chunkSize;
    BoneIndex = in.ReadLong();
    ParentIndex = in.ReadLong();
    BoneName = in.ReadNullTerminatedString();
    SamplesPerSecond = in.ReadFloat();
    Matrices.Clear();
    unsigned long count = in.ReadULong();
    for (unsigned long i = 0; i < count; ++i) {
        CMatrix3x4 matrix = in.ReadMatrix3x4();
        Matrices.push_back(matrix);
    }
    BoneType = BONE_TYPE_DEFAULT;
    if (in.GetPosition() < end)
        BoneType = (EBoneType)in.ReadUByte();
    if (in.GetPosition() < end)
        CChunkedFileSuperChunk::ReadFromFile(in, end - in.GetPosition(), loader);
}