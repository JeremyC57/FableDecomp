#ifndef FABLE_ENGINE_C3DANIMFILESEQUENCECHUNK_H
#define FABLE_ENGINE_C3DANIMFILESEQUENCECHUNK_H
#include "rebuild_abi.h"
#include "engine/CChunkedFileSuperChunk.h"
#include "engine/EBoneType.h"
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
 unsigned char Matrices[12];
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimFileSequenceChunk)==0x4c);
#endif
