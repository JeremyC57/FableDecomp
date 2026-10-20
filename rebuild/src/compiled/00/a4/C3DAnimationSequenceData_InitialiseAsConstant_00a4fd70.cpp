// C3DAnimationSequenceData::InitialiseAsConstant(const CMatrix3x4& matrix, unsigned short frameCount, float samplesPerSecond, long mode) -- build a one-matrix sequence chunk on the stack, initialise from it, then set the frame count, rate and bone type
// Grade: DIFFER(132v130) -- /GS lane gap: the lane caches the matrix reference argument in edi at entry (push edi), retail re-reads its slot right before AddMatrix; the rest (inline chunk destructor without vtable store, bitfield clear) lines up
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4FD70, 130 bytes (`ret 10h`). Callees: C3DAnimFileSequenceChunk::
// C3DAnimFileSequenceChunk 0x00AAF730, C3DAnimFileSequenceChunk::AddMatrix 0x00AAF800 (Matrices.push_back),
// C3DAnimationSequenceData::InitialiseFromChunk 0x00A4EFC0, free 0x00BFEA14, ~CCharString 0x0099EAE0,
// ~CChunkedFileSuperChunk 0x00AA8490, __security_check_cookie 0x00BFE9F9 (cookie 0x0139C8A8).
// Layout from the PDB (C3DAnimationSequenceData: SamplesPerSecond +4, FrameCount +8, BoneType = the low 5 bits
// of the byte at +0xA). The 0x4C-byte chunk local (retail layout of the landed WriteToFile 0x00AAF650: name
// +0x34, Matrices +0x40) has no user destructor: its implicit destructor is inlined without a vtable store
// (the CArray frees its block, then the name, then the out-of-line super-chunk destructor). The three member
// stores follow the call in source order (the bitfield clear is `and dl,0E0h`).
extern "C" void __cdecl free(void* block);           // 0x00BFEA14

struct CCharString {
    void* m_pStringData;
    ~CCharString();                                  // 0x0099EAE0
};

struct CMatrix3x4 { float m_row[3][4]; };           // 48 bytes

template <class T>
struct CArray {                                     // retail CArray: begin/end/capacity
    T* m_pBegin;
    T* m_pEnd;
    T* m_pEndOfStorage;
    ~CArray() { if (m_pBegin) free(m_pBegin); }
};

struct CChunkedFileSuperChunk {
    virtual ~CChunkedFileSuperChunk();              // 0x00AA8490
    unsigned char _body[0x28];                      // +0x04 .. +0x2B
};

struct C3DAnimFileSequenceChunk : CChunkedFileSuperChunk {
    long BoneIndex;                                 // +0x2C
    long ParentIndex;                               // +0x30
    CCharString BoneName;                           // +0x34
    float SamplesPerSecond;                         // +0x38
    unsigned char BoneType;                         // +0x3C
    CArray<CMatrix3x4> Matrices;                    // +0x40
    C3DAnimFileSequenceChunk();                     // 0x00AAF730
    void AddMatrix(const CMatrix3x4& matrix);       // 0x00AAF800
};

struct C3DAnimationSequenceData {
    char* BoneName;                                 // +0x00
    float SamplesPerSecond;                         // +0x04
    unsigned short FrameCount;                      // +0x08
    unsigned char BoneType : 5;                     // +0x0A
    unsigned char EnabledFlag : 1;
    unsigned char AnimatedFlag : 1;
    unsigned char IdentityFlag : 1;
    void InitialiseFromChunk(const C3DAnimFileSequenceChunk& chunk, long mode);   // 0x00A4EFC0
    void InitialiseAsConstant(const CMatrix3x4& matrix, unsigned short frameCount, float samplesPerSecond, long mode);
};

void C3DAnimationSequenceData::InitialiseAsConstant(const CMatrix3x4& matrix, unsigned short frameCount, float samplesPerSecond, long mode)
{
    C3DAnimFileSequenceChunk chunk;
    chunk.AddMatrix(matrix);
    InitialiseFromChunk(chunk, mode);
    FrameCount = frameCount;
    SamplesPerSecond = samplesPerSecond;
    BoneType = 0;
}