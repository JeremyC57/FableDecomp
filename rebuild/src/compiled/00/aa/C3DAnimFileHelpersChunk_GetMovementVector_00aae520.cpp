// C3DAnimFileHelpersChunk::GetMovementVector() -- the first sub-chunk of type "MVEC" (C3DAnimFileMovementVectorChunk) as a counted pointer: FindFirstSubChunkOfType's by-value result static-cast into a fresh pointer, returned by value
// Leaf: ?GetMovementVector@C3DAnimFileHelpersChunk@@QAE?AV?$CCountedPointer@VC3DAnimFileMovementVectorChunk@@@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAE520, 144 bytes, thiscall `ret 4` (hidden result); `this` stays in ecx for the call.
// Identity: the pushed literal "MVEC" @ 0x0129E7A4 is C3DAnimFileMovementVectorChunk's chunk type (its constructor passes it); the anim helpers TU places the non-const getter of that sub-chunk in C3DAnimFileHelpersChunk, and the callee is the non-const (QAE) FindFirstSubChunkOfType. The PDB name sets hold the mangled name above.
//   Family F004 (57 byte-identical bodies): the manifest label (C3DMeshFileSubMeshChunk::FindFirstBone) is propagated.
// Callees: CChunkedFileSuperChunk::FindFirstSubChunkOfType(const char*) @ 0x00AA80E0 (QAE, non-const; the Ego_r order QBE FindFirst, QBE FindNext,
//   QAE FindFirst, QAE FindNext matches retail 0x00AA7F40 / 0x00AA7FF0 / 0x00AA80E0 / 0x00AA8190), the count block's DeleteFunc
//   (`call [Info+4]`, Data in ecx), operator delete @ 0x00BFE9BC.
// Layout: CCountedPointer<T> {Data +0, Info +4}; count block {RefCount +0, DeleteFunc +4, Data +8}.
// Shape: `chunk; chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileMovementVectorChunk::GetStaticChunkType())); return chunk;` (the landed C3DMeshFile::GetRoot 0x00AD1E80 shape):
//   the by-value temporary from the lookup, ShareData's `Info != info` test against the empty local (`xor`ed register pair), its add-ref, the temporary's
//   release, the copy into the hidden result (no NRVO in VC7.1) and the local's release (the add-ref and release fold into
//   `inc ; mov ; dec`). The counted pointer's destructor zeroes both words after the release (as the out-of-line
//   0x00CE1000 does); VC7.1 drops those stores as dead.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
class CCPPointerInfo {
public:
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4, Data in ecx
    void* Data;                                         // +8
};
template <class T> class CCountedPointer {
public:
    T* Data;                                            // +0
    CCPPointerInfo* Info;                               // +4
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
    void ReleaseData()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other) { ShareData(static_cast<T*>(other.Data), other.Info); }
};
class CChunkedFileChunk {
public:
    virtual ~CChunkedFileChunk();
};
class CChunkedFileSuperChunk : public CChunkedFileChunk {
public:
    const CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type) const;    // 0x00AA7F40
    const CCountedPointer<CChunkedFileChunk> FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>& previous) const;   // 0x00AA7FF0
    CCountedPointer<CChunkedFileChunk> FindFirstSubChunkOfType(const char* type);                // 0x00AA80E0
    CCountedPointer<CChunkedFileChunk> FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>& previous);   // 0x00AA8190
};
class C3DAnimFileMovementVectorChunk : public CChunkedFileChunk {             // its own base does not enter this function
public:
    static const char* __fastcall GetStaticChunkType() { return "MVEC"; }   // literal @ 0x0129E7A4
};
class C3DAnimFileHelpersChunk : public CChunkedFileSuperChunk {
public:
    CCountedPointer<C3DAnimFileMovementVectorChunk> GetMovementVector();
};
CCountedPointer<C3DAnimFileMovementVectorChunk> C3DAnimFileHelpersChunk::GetMovementVector()
{
    CCountedPointer<C3DAnimFileMovementVectorChunk> chunk;
    chunk.SetStaticCast(FindFirstSubChunkOfType(C3DAnimFileMovementVectorChunk::GetStaticChunkType()));
    return chunk;
}