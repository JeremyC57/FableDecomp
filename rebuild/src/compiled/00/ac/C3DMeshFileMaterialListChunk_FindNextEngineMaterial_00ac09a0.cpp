// C3DMeshFileMaterialListChunk::FindNextEngineMaterial(const CCountedPointer<const C3DMeshFileEngineMaterialChunk>& previous) const -- the engine-material chunk after `previous` as a const counted pointer: `previous` converted to a CCountedPointer<const CChunkedFileChunk> temporary, the const FindNextSubChunkOfType's by-value result static-cast into a fresh pointer, returned by value
// Leaf: ?FindNextEngineMaterial@C3DMeshFileMaterialListChunk@@QBE?AV?$CCountedPointer@$$CBVC3DMeshFileEngineMaterialChunk@@@@ABV2@@Z
// Grade: RELOCATION_MATCH -- identity corrected from the propagated FindNextBone family label to the const
//   FindNextEngineMaterial (Ego_r order/spacing and the const callee); shared engine/CCountedPointer.h SetStaticCast
//   with the real callee FindNextSubChunkOfType const 0x00AA7FF0 (sec5 retry)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC09A0, 209 bytes, thiscall `ret 8` (hidden result + previous).
// Identity: the parked row carried the propagated family label C3DMeshFileSubMeshChunk::FindNextBone (one of the
//   byte-identical F006 bodies). Two sources put this address on the const FindNextEngineMaterial: (1) the material TU
//   order -- landed 0x00AC07A0 FindFirstEngineMaterial (QAE), 0x00AC0830 FindNextEngineMaterial (QAE), 0x00AC0910
//   FindFirstEngineMaterial (QBE), then this body -- repeats Ego_r.pdb's 0x00C3D260 / 0x00C3D2F0 / 0x00C3D3D0 / 0x00C3D460
//   with the same 0x90 / 0xE0 / 0x90 spacing; (2) the callee is the const (QBE) FindNextSubChunkOfType 0x00AA7FF0, so the
//   caller is the const member (ABV2: the parameter is the same CCountedPointer<const ...> as the result).
// Callees: CChunkedFileSuperChunk::FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>&) const @ 0x00AA7FF0,
//   the count block's DeleteFunc, operator delete @ 0x00BFE9BC.
// Layout: engine/CCountedPointer.h (Data +0, Info +4; CCPPointerInfo RefCount +0, DeleteFunc +4, Data +8).
// Shape: `chunk; chunk.SetStaticCast(FindNextSubChunkOfType(previous)); return chunk;` as the landed QBE twin
//   C3DMeshFileMaterialChunk::FindNextMap 0x00AC0EE0: ShareData loads the found Data before the null-count test.
#include "engine/CCountedPointer.h"

class CChunkedFileChunk {
public:
    virtual ~CChunkedFileChunk();
};
class CChunkedFileSuperChunk : public CChunkedFileChunk {
public:
    const CCountedPointer<CChunkedFileChunk> FindNextSubChunkOfType(const CCountedPointer<const CChunkedFileChunk>& previous) const;   // 0x00AA7FF0
};
class C3DMeshFileEngineMaterialChunk : public CChunkedFileChunk {
};
class C3DMeshFileMaterialListChunk : public CChunkedFileSuperChunk {
public:
    CCountedPointer<const C3DMeshFileEngineMaterialChunk> FindNextEngineMaterial(const CCountedPointer<const C3DMeshFileEngineMaterialChunk>& previous) const;
};

CCountedPointer<const C3DMeshFileEngineMaterialChunk> C3DMeshFileMaterialListChunk::FindNextEngineMaterial(const CCountedPointer<const C3DMeshFileEngineMaterialChunk>& previous) const
{
    CCountedPointer<const C3DMeshFileEngineMaterialChunk> chunk;
    chunk.SetStaticCast(FindNextSubChunkOfType(previous));
    return chunk;
}