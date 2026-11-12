// C3DAnimationSequenceData::ReadCompressedFromFile(CDataInputStream&, CSimpleMemoryPool&) -- read one bone's compressed animation track header and its four track arrays from the stream into pool memory
// Leaf: ?ReadCompressedFromFile@C3DAnimationSequenceData@@QAEXAAVCDataInputStream@@AAVCSimpleMemoryPool@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A4C5E0, 1844 bytes (two `ret 8` epilogues, the last at 0x00A4CD11; int3 padding from
//   0x00A4CD14, then an uncatalogued 71-byte function at 0x00A4CD20 -- SPLITS.tsv).
// Name: Ego_r.pdb / FableWin.pdb ?ReadCompressedFromFile@C3DAnimationSequenceData@@QAEXAAVCDataInputStream@@AAVCSimpleMemoryPool@@@Z; the
//   landed caller C3DAnimationSequence::ReadCompressedFromFile 0x00A4CDD0 passes the stream and the pool (`ret 8`).
// Callees: CDataInputStream virtual ReadNullTerminatedString (slot 6, +0x18, hidden CCharString result), CSimpleMemoryPool::Allocate
//   0x00A5C910 (through the inline FableWin AllocateType<char / CQuaternion / unsigned char / C3DVectorWord>), the private
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inline Read's slow path), ~CCharString 0x0099EAE0.
// Layout: engine/C3DAnimationSequenceData.h (retail 0x2C): BoneName +0, SamplesPerSecond +4, FrameCount +8, the bit fields at +0xA
//   (BoneType:5, EnabledFlag, AnimatedFlag, IdentityFlag) and +0xB (RotationTrackMode:2, PositionTrackMode:2, ScalingTrackMode:2),
//   PositionFactor +0xC, ScalingFactor +0x10, RotationTrack +0x14, the four sizes +0x18..+0x1E, PositionTrack +0x20,
//   PalettedRotationTrack +0x24, PalettedPositionTrack +0x28. CDataInputStream: StreamPos +4, CurrentSourcePtr +0xC,
//   SourceChunkBytesRemaining +0x14 (engine/CDataInputStream.h retail branch).
// Shape: the bone name read into a local CCharString (destroyed at both exits), BoneName allocated GetLength() + 1 and filled by the
//   intrinsic strcpy after the bone type byte; the typed reads expand CDataInputStream::ReadType<T> (their value lives in the dead
//   stream-argument word); each track is sized, allocated and read in file order; SetupIdentityFlag expanded at the end.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_READ
#include <string.h>
#include "fable_string.h"
#include "engine/CDataInputStream.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/CQuaternion.h"
#include "engine/C3DAnimationSequenceData.h"

inline void C3DAnimationSequenceData::SetupIdentityFlag()
{
    if (RotationTrackMode == 0 && PositionTrackMode == 0 && ScalingTrackMode == 0)
        IdentityFlag = true;
    else
        IdentityFlag = false;
}

void C3DAnimationSequenceData::ReadCompressedFromFile(CDataInputStream& in, CSimpleMemoryPool& pool)
{
    CCharString bone_name = in.ReadNullTerminatedString();
    BoneName = pool.AllocateType<char>(bone_name.GetLength() + 1);
    BoneType = in.ReadUBYTE();
    strcpy(BoneName, bone_name.GetAsCharArray());
    SamplesPerSecond = in.ReadFloat();
    FrameCount = (unsigned short)in.ReadULONG();
    AnimatedFlag = in.ReadEBOOL();
    RotationTrackMode = in.ReadUBYTE();
    PositionTrackMode = in.ReadUBYTE();
    ScalingTrackMode = in.ReadUBYTE();
    PositionFactor = in.ReadFloat();
    ScalingFactor = in.ReadFloat();

    RotationTrackSize = in.ReadUWORD();
    RotationTrack = pool.AllocateType<CQuaternion>(RotationTrackSize);
    in.Read(RotationTrack, RotationTrackSize * sizeof(CQuaternion));
    PalettedRotationTrackSize = in.ReadUWORD();
    PalettedRotationTrack = pool.AllocateType<unsigned char>(PalettedRotationTrackSize);
    in.Read(PalettedRotationTrack, PalettedRotationTrackSize);
    PositionTrackSize = in.ReadUWORD();
    PositionTrack = pool.AllocateType<C3DVectorWord>(PositionTrackSize);
    in.Read(PositionTrack, PositionTrackSize * sizeof(C3DVectorWord));
    PalettedPositionTrackSize = in.ReadUWORD();
    PalettedPositionTrack = pool.AllocateType<unsigned char>(PalettedPositionTrackSize);
    in.Read(PalettedPositionTrack, PalettedPositionTrackSize);

    SetupIdentityFlag();
}
