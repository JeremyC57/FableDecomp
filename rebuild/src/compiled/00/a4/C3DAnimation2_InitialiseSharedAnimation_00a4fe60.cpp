// C3DAnimation2::InitialiseSharedAnimation(C3DAnimFile const &,EAnimationType) -- initialize shared animation data from the supplied animation file.
// Leaf: ?InitialiseSharedAnimation@C3DAnimation2@@QAE_NABVC3DAnimFile@@W4EAnimationType@@@Z
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_C3DANIMATION2
#define FABLE_RETAIL_C3DANIMFILEBONEMASKCHUNK
#define FABLE_RETAIL_C3DANIMFILEHELPERSCHUNK
#define FABLE_RETAIL_C3DANIMFILEMOVEMENTVECTORCHUNK
#define FABLE_RETAIL_C3DANIMFILEXALLOCATIONSIZECHUNK
#define FABLE_RETAIL_C3DANIMFILEXSEQUENCECHUNK
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CDATAINPUTSTREAM
#include "stlport/fable_containers.h"
#include "engine/C3DAnimation2.h"
#include "engine/C3DAnimFile.h"
#include "engine/C3DAnimFileRootChunk.h"
#include "engine/C3DAnimFileHelpersChunk.h"
#include "engine/C3DAnimFileObjectChunk.h"
#include "engine/C3DAnimFileXAllocationSizeChunk.h"
#include "engine/C3DAnimFileXSequenceChunk.h"
#include "engine/C3DAnimFileBoneMaskChunk.h"
#include "engine/C3DAnimFileMovementVectorChunk.h"
#include "engine/C3DAnimFileSequenceChunk.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/CMovableResourceMemoryPool.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CScopedLockFlagBool.h"

inline bool C3DAnimationSequence::InitialiseFromFileData(const C3DAnimFileSequenceChunk& sequence,C3DAnimationSequenceData* data,CSimpleMemoryPool& allocator)
{
 ParentIndex=(short)sequence.GetParentIndex();Data=data;Shared=false;
 return Data->InitialiseFromFileData(sequence,allocator);
}

bool C3DAnimation2::InitialiseSharedAnimation(const C3DAnimFile& file_data,EAnimationType anim_type)
{
 CScopedLockFlagBool lock(LockedFromMemMemoryRelocation);
 InitialiseStats(file_data,anim_type);
 Allocator->SetCapacity(16384);
 if(file_data.GetRoot()->HasHelpers()) {
  CCountedPointer<C3DAnimFileXAllocationSizeChunk> alloc_size_chunk;
  alloc_size_chunk.SetStaticCast(file_data.GetRoot()->GetHelpers()->FindFirstSubChunkOfType(C3DAnimFileXAllocationSizeChunk::GetStaticChunkType()));
  if(!alloc_size_chunk.IsNull()) {
   unsigned long capacity=alloc_size_chunk->GetAllocSize();
   if(capacity) {
    if(MemoryPool) {
     if(!MemoryPool->AllocateResource(&MemoryHandle,this,capacity,0)) {return false;}
     Allocator->SetMemoryPool(MemoryPool->GetPointerFromHandle(MemoryHandle),capacity);
    }
   }else Allocator->SetCapacity(0);
  }
 }
 CCountedPointer<const C3DAnimFileObjectChunk> anim_data=file_data.GetRoot()->FindFirstObject();
 ReadCompressedFileData(*anim_data,*Allocator);
 FrameCount=Sequences[0].GetFrameCount();SamplesPerSecond=Sequences[0].GetSamplesPerSecond();
 if(anim_type&2) {
  CCountedPointer<const C3DAnimFileBoneMaskChunk> bone_mask_data=anim_data->PeekBoneMask();
  BoneMask=Allocator->AllocateType<bool>(SequenceCount);
  for(unsigned long c_bone=0;c_bone<SequenceCount;++c_bone)BoneMask[c_bone]=bone_mask_data->GetBoneEnable(c_bone);
 }
 MovementTrack.ReleaseData();
 if(file_data.GetRoot()->HasHelpers()&&file_data.GetRoot()->GetHelpers()->HasMovementVector()) {
  CCountedPointer<const C3DAnimFileMovementVectorChunk> vector_chunk=file_data.GetRoot()->GetHelpers()->GetMovementVector();
  CCountedPointer<const C3DAnimFileXSequenceChunk> compressed_track;
  compressed_track.SetStaticCast(vector_chunk->FindFirstSubChunkOfType(C3DAnimFileXSequenceChunk::GetStaticChunkType()));
  if(!compressed_track.IsNull()) {
   MovementTrack.Reset(new C3DAnimationSequence);
   MovementTrackData.Reset(new C3DAnimationSequenceData);
   CMemoryDataInputStream datastream(compressed_track->PeekData(),compressed_track->GetDataSize());
   MovementTrack->ReadCompressedFromFile(datastream,compressed_track->GetDataSize(),MovementTrackData.Get(),*Allocator);
  }else if(vector_chunk->HasMovementTrack()) {
   MovementTrack.Reset(new C3DAnimationSequence);
   MovementTrackData.Reset(new C3DAnimationSequenceData);
   MovementTrack->InitialiseFromFileData(*vector_chunk->GetMovementTrack(),MovementTrackData.Get(),*Allocator);
  }
 }
 return true;
}