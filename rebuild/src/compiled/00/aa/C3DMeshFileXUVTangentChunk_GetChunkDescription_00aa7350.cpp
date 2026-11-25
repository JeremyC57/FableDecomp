// C3DMeshFileXUVTangentChunk::GetChunkDescription
// Retail 0x00aa7350 : struct-return __fastcall virtual method.
// Builds a CCharString from a literal (const char*, -1) into the hidden
// return buffer and returns it by value.

class CCharString {
public:
    // ctor(const char*, long) at 0x0099ebf0
    CCharString(const char* s, long n);
private:
    char* m_data;
};

// The string literal lives at 0x0129e3ec in the retail image.

class C3DMeshFileXUVTangentChunk {
public:
    virtual const CCharString GetChunkDescription() const;
};

const CCharString C3DMeshFileXUVTangentChunk::GetChunkDescription() const
{
    return CCharString("UV Tangent Vectors", -1);
}