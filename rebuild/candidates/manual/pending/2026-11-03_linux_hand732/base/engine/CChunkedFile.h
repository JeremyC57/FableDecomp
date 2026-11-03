#ifndef FABLE_ENGINE_CCHUNKEDFILE_H
#define FABLE_ENGINE_CCHUNKEDFILE_H
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class CChunkedFile:public CBaseClassNonCopyable{public:
CChunkedFile(const CChunkedFile&);
CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>&);
virtual ~CChunkedFile();
private:CCountedPointer<CChunkedFileSuperChunk> RootChunk;unsigned long Version;CCharString FileComment;};
FABLE_STATIC_ASSERT(sizeof(CChunkedFile)==0x14);
#endif
