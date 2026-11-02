#ifndef FABLE_ENGINE_C3DANIMFILEROOTCHUNK_H
#define FABLE_ENGINE_C3DANIMFILEROOTCHUNK_H
#include "rebuild_abi.h"
#include "engine/CChunkedFileSuperChunk.h"
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimFileRootChunk:public CChunkedFileSuperChunk{public:
C3DAnimFileRootChunk(const C3DAnimFileRootChunk&);
C3DAnimFileRootChunk();
virtual const CCharString GetChunkDescription() const;
CCountedPointer<C3DAnimFileObjectChunk const > FindFirstObject() const;
CCountedPointer<C3DAnimFileObjectChunk> FindFirstObject();
bool HasHelpers() const;
CCountedPointer<C3DAnimFileHelpersChunk const > GetHelpers() const;
CCountedPointer<C3DAnimFileHelpersChunk> GetHelpers();
virtual void ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&);
virtual void WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const;
virtual CChunkedFileChunk* CreateNewInstance() const;
virtual ~C3DAnimFileRootChunk();
private:bool LoopFlag;float Duration;};
FABLE_STATIC_ASSERT(sizeof(C3DAnimFileRootChunk)==0x34);
#endif
