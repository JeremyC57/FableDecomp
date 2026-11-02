#ifndef FABLE_ENGINE_C3DANIMFILE_H
#define FABLE_ENGINE_C3DANIMFILE_H
#include "rebuild_abi.h"
#include "engine/CChunkedFile.h"
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimFile:public CChunkedFile{public:
C3DAnimFile(const C3DAnimFile&);
private:
C3DAnimFile(const CCountedPointer<C3DAnimFileRootChunk>&);
public:
C3DAnimFile();
CCountedPointer<C3DAnimFileRootChunk const > GetRoot() const;
CCountedPointer<C3DAnimFileRootChunk> GetRoot();
virtual ~C3DAnimFile();
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimFile)==0x14);
#endif
