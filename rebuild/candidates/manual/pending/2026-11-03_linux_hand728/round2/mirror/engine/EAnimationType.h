#ifndef FABLE_ENGINE_EANIMATIONTYPE_H
#define FABLE_ENGINE_EANIMATIONTYPE_H
#include "rebuild_abi.h"
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
enum EAnimationType{ANIMTYPE_BASE=0,ANIMTYPE_BASE_RELATIVE=4,ANIMTYPE_COMBINATION_PARTIAL=3,ANIMTYPE_COMBINATION_DELTA=5};
#endif
