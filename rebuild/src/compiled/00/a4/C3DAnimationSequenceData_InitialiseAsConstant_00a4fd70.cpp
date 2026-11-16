// C3DAnimationSequenceData::InitialiseAsConstant(const CMatrix3x4& matrix, unsigned long frame_count, float samples_per_second, CSimpleMemoryPool& memory_pool) -- build a one-matrix sequence chunk on the stack, initialise from it through the memory pool, then set the frame count, rate and bone type
// Leaf: ?InitialiseAsConstant@C3DAnimationSequenceData@@QAEXABVCMatrix3x4@@KMAAVCSimpleMemoryPool@@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4FD70, 130 bytes (`ret 10h`).
// Identity: Ego_r.pdb + FableWin.pdb ?InitialiseAsConstant@C3DAnimationSequenceData@@QAEXABVCMatrix3x4@@KMAAVCSimpleMemoryPool@@@Z
//   (engine/C3DAnimationSequenceData.h C3DANIMATIONSEQUENCE_INITIALISE); both callers (0x00A4FE50 in C3DAnimationSequence::InitialiseAsConstant
//   0x00A4FE30 and 0x00A5067D in 0x00A50510) push the pool reference.
// Callees: C3DAnimFileSequenceChunk::C3DAnimFileSequenceChunk 0x00AAF730, C3DAnimFileSequenceChunk::AddMatrix 0x00AAF800 (FableWin.pdb
//   ?AddMatrix@C3DAnimFileSequenceChunk@@QAEXABVCMatrix3x4@@@Z), C3DAnimationSequenceData::InitialiseFromFileData 0x00A4EFC0, free 0x00BFEA14,
//   ~CCharString 0x0099EAE0, ~CChunkedFileSuperChunk 0x00AA8490, __security_check_cookie 0x00BFE9F9 (cookie 0x0139C8A8).
// Shape: the chunk's implicit destructor is expanded in place (no vtable store); the three member stores follow the call in source order.
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_C3DANIMATIONSEQUENCE_INITIALISE
#define FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_ADDMATRIX
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CChunkedFileChunk.h"
#include "engine/CChunkedFileSuperChunk.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DAnimFileSequenceChunk.h"
#include "engine/C3DAnimationSequenceData.h"

void C3DAnimationSequenceData::InitialiseAsConstant(const CMatrix3x4& matrix, unsigned long frame_count, float samples_per_second,
    CSimpleMemoryPool& memory_pool)
{
    C3DAnimFileSequenceChunk chunk;
    chunk.AddMatrix(matrix);
    InitialiseFromFileData(chunk, memory_pool);
    FrameCount = (unsigned short)frame_count;
    SamplesPerSecond = samples_per_second;
    BoneType = 0;
}
