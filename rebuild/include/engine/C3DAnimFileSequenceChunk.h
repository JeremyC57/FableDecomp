#ifndef FABLE_ENGINE_C3DANIMFILESEQUENCECHUNK_H
#define FABLE_ENGINE_C3DANIMFILESEQUENCECHUNK_H
#include "rebuild_abi.h"
#include "engine/CChunkedFileSuperChunk.h"
#include "engine/EBoneType.h"
#ifdef FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_ADDMATRIX
#include "engine/CArray.h"
#include "engine/CMatrix3x4.h"
#endif
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimFileSequenceChunk:public CChunkedFileSuperChunk{public:
C3DAnimFileSequenceChunk(const C3DAnimFileSequenceChunk&);
C3DAnimFileSequenceChunk();
C3DAnimFileSequenceChunk(long,const CCharString&);
virtual const CCharString GetChunkDescription() const;
virtual const CCharString GetChunkInfo() const;
virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
virtual CChunkedFileChunk* CreateNewInstance() const;
#ifdef FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_ADDMATRIX
// agentNF41 opt-in (row 0x00A4FD70 C3DAnimationSequenceData::InitialiseAsConstant): FableWin.pdb / Ego_r.pdb
// ?AddMatrix@C3DAnimFileSequenceChunk@@QAEXABVCMatrix3x4@@@Z (retail 0x00AAF800, out of line: Matrices.push_back) and the
// Matrices member as its Ego_r.pdb type CArray<CMatrix3x4> (12-byte STLport vector at +0x40). No user destructor: retail expands
// the implicit one in place without a vtable store (0x00A4FDC0..: the CArray block freed, ~CCharString on BoneName, then the
// out-of-line ~CChunkedFileSuperChunk 0x00AA8490). No layout change.
void AddMatrix(const CMatrix3x4&);
#else
virtual ~C3DAnimFileSequenceChunk();
#endif
public:long GetParentIndex()const{return ParentIndex;}
#ifdef FABLE_RETAIL_BUILDER_INTERSECTION_PASS29
 void SetParentIndex(long);
#endif
#if defined(FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_GETTERS) && defined(FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_ADDMATRIX)
 // Retail opt-in (p25C, row 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData): FableWin.pdb
 // ?GetBoneName@C3DAnimFileSequenceChunk@@QBE?BVCCharString@@XZ (by value: the row copy-constructs from +0x34, 0x00A4EFDE),
 // ?GetBoneType@C3DAnimFileSequenceChunk@@QBE?AW4EBoneType@@XZ (+0x3C), ?GetSamplesPerSecond@C3DAnimFileSequenceChunk@@QBEMXZ (+0x38),
 // ?GetMatrixCount@C3DAnimFileSequenceChunk@@QBEKXZ (Matrices.size(): the /48 at 0x00A4F05C) and
 // ?GetMatrix@C3DAnimFileSequenceChunk@@QBEPBVCMatrix3x4@@K@Z (&Matrices[index]), all inline in retail. Needs ADDMATRIX (the
 // CArray<CMatrix3x4> member type).
 const CCharString GetBoneName() const { return BoneName; }
 EBoneType GetBoneType() const { return BoneType; }
 float GetSamplesPerSecond() const { return SamplesPerSecond; }
 unsigned long GetMatrixCount() const { return Matrices.size(); }
 const CMatrix3x4* GetMatrix(unsigned long index) const { return &Matrices[index]; }
#endif
private:long BoneIndex,ParentIndex;CCharString BoneName;float SamplesPerSecond;EBoneType BoneType;
#ifdef FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_ADDMATRIX
 CArray<CMatrix3x4> Matrices;
#else
 unsigned char Matrices[12];
#endif
};
FABLE_STATIC_ASSERT_AT(22, sizeof(C3DAnimFileSequenceChunk)==0x4c);
#endif
