#ifndef FABLE_ENGINE_C3DANIMFILESEQUENCECHUNK_H
#define FABLE_ENGINE_C3DANIMFILESEQUENCECHUNK_H
#include "rebuild_abi.h"
#include "engine/CChunkedFileSuperChunk.h"
#include "engine/EBoneType.h"
#ifdef FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_MATRICES
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
virtual ~C3DAnimFileSequenceChunk();
public:long GetParentIndex()const{return ParentIndex;}
private:long BoneIndex,ParentIndex;CCharString BoneName;float SamplesPerSecond;EBoneType BoneType;
#ifdef FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_MATRICES
 // Retail opt-in (chunk 860, row 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData): Ego_r.pdb types the member
 // CArray<CMatrix3x4> Matrices (+0x40; 12 bytes, the 0x30-byte element stride divided at 0x00A4F054..0x00A4F06E), and
 // InitialiseFromFileData reads BoneName / SamplesPerSecond / BoneType / Matrices directly (0x00A4EFD1, 0x00A4F03A, 0x00A4F04E,
 // 0x00A4F054): its class is a friend here.
 friend class C3DAnimationSequenceData;
 CArray<CMatrix3x4> Matrices;
#else
 unsigned char Matrices[12];
#endif
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimFileSequenceChunk)==0x4c);
#endif
