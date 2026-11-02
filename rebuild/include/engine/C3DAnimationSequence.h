#ifndef FABLE_ENGINE_C3DANIMATIONSEQUENCE_H
#define FABLE_ENGINE_C3DANIMATIONSEQUENCE_H
#include "rebuild_abi.h"
#include "engine/C3DAnimationSequenceData.h"
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimationSequence{
public:C3DAnimationSequence():Data(0),ParentIndex(-1),Shared(false){}
 unsigned long GetFrameCount()const{return Data->GetFrameCount();}float GetSamplesPerSecond()const{return Data->GetSamplesPerSecond();}
 bool InitialiseFromFileData(const C3DAnimFileSequenceChunk&,C3DAnimationSequenceData*,CSimpleMemoryPool&);
 void ReadCompressedFromFile(CDataInputStream&,unsigned long,C3DAnimationSequenceData*,CSimpleMemoryPool&);
private:C3DAnimationSequenceData* Data;short ParentIndex;bool Shared;
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimationSequence)==0x8);
#endif
