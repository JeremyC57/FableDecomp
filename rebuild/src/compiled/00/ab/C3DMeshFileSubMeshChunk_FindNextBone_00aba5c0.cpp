// C3DMeshFileSubMeshChunk::FindNextBone(const CCountedPointer<const C3DMeshFileBoneChunk>& previous) -- the next C3DMeshFileBoneChunk after `previous` (same chunk type) as a counted pointer: `previous` converted to a CCountedPointer<const CChunkedFileChunk> temporary, FindNextSubChunkOfType's by-value result static-cast into a fresh pointer, returned by value
// Leaf: ?FindNextBone@C3DMeshFileSubMeshChunk@@QAE?AV?$CCountedPointer@VC3DMeshFileBoneChunk@@@@ABV?$CCountedPointer@$$CBVC3DMeshFileBoneChunk@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABA5C0, 224 bytes, thiscall `ret 8` (hidden result + previous); `this` stays in ecx for the call.
// Identity: no literal in the body (the type comes from `previous`); it sits right after its FindFirst twin 0x00ABA530 of the same constness in the sub-mesh TU (Ego_r order FindFirstX, FindNextX per constness), and the callee is the non-const (QAE) FindNextSubChunkOfType. The PDB name sets hold the mangled name above.
//   Family F006 (33 byte-identical bodies): the manifest label (C3DMeshFileSubMeshChunk::FindNextBone) is propagated.
// Callees: CChunkedFileSuperChunk::FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>&) @ 0x00AA8190 (QAE, non-const; the Ego_r order QBE FindFirst, QBE FindNext,
//   QAE FindFirst, QAE FindNext matches retail 0x00AA7F40 / 0x00AA7FF0 / 0x00AA80E0 / 0x00AA8190), the count block's DeleteFunc
//   (`call [Info+4]`, Data in ecx), operator delete @ 0x00BFE9BC.
// Layout: CCountedPointer<T> {Data +0, Info +4}; count block {RefCount +0, DeleteFunc +4, Data +8}.
// Shape: `chunk; chunk.SetStaticCast(FindNextSubChunkOfType(previous)); return chunk;` (the landed C3DMeshFile::GetRoot 0x00AD1E80 shape):
//   the converted `previous` temporary (template converting constructor, add-ref), the by-value temporary from the lookup, ShareData's `Info != info` test against the empty local (`xor`ed register pair), its add-ref, the temporary's
//   release, the copy into the hidden result (no NRVO in VC7.1) and the local's release (the add-ref and release fold into
//   `inc ; mov ; dec`). The counted pointer's destructor zeroes both words after the release (as the out-of-line
//   0x00CE1000 does); VC7.1 drops those stores as dead except for the lookup temporary, whose zero stores survive because the converted argument's release (a call) follows them.
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
class C3DMeshFileBoneChunk : public CChunkedFileChunk {             // its own base does not enter this function
public:
    static const char* __fastcall GetStaticChunkType() { return "BONE"; }   // literal @ 0x(not referenced here)
};
class C3DMeshFileSubMeshChunk : public CChunkedFileSuperChunk {
public:
    CCountedPointer<C3DMeshFileBoneChunk> FindNextBone(const CCountedPointer<const C3DMeshFileBoneChunk>& previous);
};
CCountedPointer<C3DMeshFileBoneChunk> C3DMeshFileSubMeshChunk::FindNextBone(const CCountedPointer<const C3DMeshFileBoneChunk>& previous)
{
    CCountedPointer<C3DMeshFileBoneChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}