#ifndef FABLE_ENGINE_C3DANIMFILEOBJECTCHUNK_H
#define FABLE_ENGINE_C3DANIMFILEOBJECTCHUNK_H
#include "rebuild_abi.h"
#include "engine/CChunkedFileSuperChunk.h"
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimFileObjectChunk:public CChunkedFileSuperChunk{public:
C3DAnimFileObjectChunk(const C3DAnimFileObjectChunk&);
C3DAnimFileObjectChunk();
virtual const CCharString GetChunkDescription() const;
virtual const CCharString GetChunkInfo() const;
CCountedPointer<C3DAnimFileBoneMaskChunk const > PeekBoneMask() const;
virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
virtual CChunkedFileChunk* CreateNewInstance() const;
virtual ~C3DAnimFileObjectChunk();
private:CCharString Name;long SubMeshIndex,ParentIndex,FirstChildIndex,NextSiblingIndex;};
FABLE_STATIC_ASSERT(sizeof(C3DAnimFileObjectChunk)==0x40);
#endif
