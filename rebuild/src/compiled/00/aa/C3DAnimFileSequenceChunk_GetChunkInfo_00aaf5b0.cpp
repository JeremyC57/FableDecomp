// C3DAnimFileSequenceChunk::GetChunkInfo() const -- "Bone Name: %s. Bone Index: %li, Frames: %lu, Framerate: %f" into a 1 KB buffer, returned as a CCharString
// Grade: DIFFER(151v149) -- /GS lane gap: the lane loads the hidden result pointer into esi in the prologue (name pointer goes to edi), retail keeps the name in esi and re-reads the result slot after _snprintf (mov esi,[esp+42Ch])
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAF5B0, 160 bytes. Callees: _snprintf 0x00BFEB96 (format @ 0x0129E878),
// CCharString(const char*, long) 0x0099EBF0, __security_check_cookie 0x00BFE9F9 (cookie @ 0x0139C8A8).
// /GS frame: the 0x400-byte buffer plus the cookie. Retail layout (the PDB's is 8 bytes higher: its
// CChunkedFileChunk base carries 16-byte vectors): BoneIndex +0x2C, ParentIndex +0x30, BoneName +0x34,
// SamplesPerSecond +0x38, BoneType +0x3C, Matrices +0x40 (CArray<CMatrix3x4>, 48-byte elements: the
// 0x2AAAAAAB / sar 3 divide). The name is the inline null-tested chars accessor (empty literal 0x0129AAF4);
// the float is widened to double for the varargs; the buffer is terminated after the call.
extern "C" int __cdecl _snprintf(char* buffer, unsigned int count, const char* format, ...);   // 0x00BFEB96
extern const char g_emptyLiteral_0129aaf4[];        // 0x0129AAF4: ""

class CCharStringData { public:
    const char*   m_pChars;                         // +0x00
    unsigned long m_length;                         // +0x04
};
class CCharString { public:
    CCharStringData* m_pStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    const char* GetChars() const { return m_pStringData == 0 ? g_emptyLiteral_0129aaf4 : m_pStringData->m_pChars; }
};

class CMatrix3x4 { public: float m_row[3][4]; };           // 48 bytes
template <class T> class CArray { public:                  // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long Size() const { return m_end - m_begin; }
};

class C3DAnimFileSequenceChunk { public:
    virtual ~C3DAnimFileSequenceChunk();
    unsigned char _base_body[0x28];                 // +0x04 .. +0x2B (CChunkedFileChunk base, retail size)
    long BoneIndex;                                 // +0x2C
    long ParentIndex;                               // +0x30
    CCharString BoneName;                           // +0x34
    float SamplesPerSecond;                         // +0x38
    unsigned char BoneType;                         // +0x3C EBoneType (written as a byte)
    CArray<CMatrix3x4> Matrices;                    // +0x40
    CCharString GetChunkInfo() const;
};

CCharString C3DAnimFileSequenceChunk::GetChunkInfo() const
{
    char buffer[1024];
    _snprintf(buffer, 1023, "Bone Name: %s. Bone Index: %li, Frames: %lu, Framerate: %f",
              BoneName.GetChars(), BoneIndex, Matrices.Size(), (double)SamplesPerSecond);
    buffer[1023] = 0;
    return CCharString(buffer, -1);
}