#ifdef FABLE_RETAIL_C3DANIMATION2
#ifndef FABLE_ENGINE_C3DANIMATION2_H
#define FABLE_ENGINE_C3DANIMATION2_H
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DAnimationStats.h"
#include "engine/CMovableResource.h"
#include "engine/C3DAnimationSequence.h"
#ifdef FABLE_RETAIL_C3DANIMATIONINFO_BUILD
// Retail opt-in (Codex pass19): original Ego_r/FableWin types and donor calls; native animation rows 00a24d70/00a26ad0. See NAMING_EVIDENCE.md.
class C3DMesh2;
#endif
#ifdef FABLE_RETAIL_C3DANIMATION2_CALCLOCAL
class C3DMesh2;class C3DKeyframe;
#endif
#ifdef FABLE_RETAIL_C3DANIMATION2_CONVERTTIME_BODY
#include "engine/GFFloatToLongFloor.h"
#endif
#ifdef FABLE_RETAIL_C3DANIMATION2_INITSEQUENCES
class C3DMesh2;
#endif
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimation2:public C3DAnimationStats,public CMovableResource{
public:
C3DAnimation2(const C3DAnimation2&);
C3DAnimation2();
virtual ~C3DAnimation2();
#ifdef FABLE_RETAIL_C3DANIMATIONINFO_BUILD
// Retail opt-in (Codex pass19): original Ego_r/FableWin types and donor calls; native animation rows 00a24d70/00a26ad0. See NAMING_EVIDENCE.md.
void SetMemoryPool(CMovableResourceMemoryPool*);
bool InitialiseAnimationInstance(C3DAnimation2&,const C3DMesh2&,bool,float*);
#endif
#ifdef FABLE_RETAIL_C3DANIMATION2_CALCLOCAL
// Retail opt-in (chunk 2005, row 0x00A4D920): Ego_r.pdb ?ConvertTimeToFrames@C3DAnimation2@@QBEXMAAJAAM@Z (retail 0x00A52650) and
// ?CalcLocalBoneTransformationRelative@C3DAnimation2@@QBEXAAVC3DKeyframe@@KMABVC3DMesh2@@@Z (retail 0x00A4D920, `ret 10h`).
#ifdef FABLE_RETAIL_C3DANIMATION2_CONVERTTIME_BODY
// Retail opt-in (chunk 2044, rows 0x00AA0090 / 0x00A9F540): the body of ConvertTimeToFrames visible in the includer (landed 0x00A52650's
// text; it holds the asm helper GFFloatToLongFloor, so VC7.1 keeps the call out of line, but sees that frame / fraction are only
// written, not captured).
void ConvertTimeToFrames(float time,long& frame,float& fraction) const {
    float frames = time * SamplesPerSecond;
    long whole = GFFloatToLongFloor(frames);
    frame = whole;
    fraction = frames - whole;
    frame = (unsigned long)frame % FrameCount;
    frame = frame < 0 ? frame + FrameCount : frame;
}
#else
void ConvertTimeToFrames(float time,long& frame,float& fraction) const;
#endif
void CalcLocalBoneTransformationRelative(C3DKeyframe& keyframe,unsigned long bone,float time,const C3DMesh2& mesh) const;
#ifdef FABLE_RETAIL_C3DANIMATION2_CALCLOCALBONE
// Retail opt-in (chunk 3001, row 0x00A4D920): Ego_r.pdb / FableWin.pdb ?CalcLocalBoneTransformation@C3DAnimation2@@QBEXAAVC3DKeyframe@@KM@Z
// (retail out of line at 0x00A4D330, `ret 0Ch`, same TU ahead of the Relative form, which expands it into its local keyframe).
void CalcLocalBoneTransformation(C3DKeyframe& keyframe,unsigned long bone,float time) const;
#endif
#endif
#ifdef FABLE_RETAIL_C3DANIMATION2_INITSEQUENCES
// Retail opt-in (chunk 2014, row 0x00A50510): Ego_r.pdb / FableWin.pdb ?GetSamplesPerSecond@C3DAnimation2@@QBEMXZ and
// ?GetFrameCount@C3DAnimation2@@QBEKXZ (inline in retail: the members are read in place, 0x00A50640 / 0x00A50646) and the private
// ?InitialiseAnimationSequences@C3DAnimation2@@AAE_NAAV1@ABVC3DMesh2@@_NAAK@Z (retail 0x00A50510, `ret 10h`).
float GetSamplesPerSecond() const { return SamplesPerSecond; }
unsigned long GetFrameCount() const { return FrameCount; }
private:
bool InitialiseAnimationSequences(C3DAnimation2& shared_animation,const C3DMesh2& mesh,bool create_rigid_bones,unsigned long& build_result);
public:
#endif
#ifdef FABLE_RETAIL_C3DANIMATION2_MANAGER
// Retail opt-in (chunk 2044, rows 0x00AA0090 / 0x00A9F540): Ego_r.pdb / FableWin.pdb ?PeekSequence@C3DAnimation2@@QBEPBVC3DAnimationSequence@@K@Z
// and ?IsBoneEnabled@C3DAnimation2@@QBE_NK@Z, inline in retail (Sequences +0x5C indexed by 8, BoneMask +0x68 null-tested then indexed:
// 0x00AA0590..0x00AA059B `mov eax,[edx+68h] ; test eax,eax ; je ; cmp byte ptr [eax+esi],0 ; je`).
const C3DAnimationSequence* PeekSequence(unsigned long index) const { return &Sequences[index]; }
bool IsBoneEnabled(unsigned long bone) const { return BoneMask == 0 || BoneMask[bone]; }
#endif
#ifdef FABLE_RETAIL_C3DANIMATION2_NONLOOPINGDURATION
// Retail opt-in (chunk 2014, row 0x00A4E300 GetDeltaMovement): Ego_r.pdb / FableWin.pdb ?GetNonLoopingDuration@C3DAnimation2@@QBEMXZ,
// inline in retail (no out-of-line copy): the unsigned FrameCount converted to float (`fild` + the 2^32 fix-up), minus one frame,
// over SamplesPerSecond (0x00A4E31A..0x00A4E32D).
float GetNonLoopingDuration() const { return (FrameCount - 1.0f) / SamplesPerSecond; }
#endif
bool InitialiseSharedAnimation(const C3DAnimFile&,EAnimationType);
virtual C3DVector GetMovementOffsetAtTime(float) const;
virtual C3DVector GetMovementAtTime(float) const;
virtual bool HasRotation() const;
virtual CQuaternion GetRotationAtTime(float) const;
virtual void GetDeltaMovement(CMatrix3x4&,float,float) const;
virtual void RelocateData(long,void *,void *,long);
virtual bool CanRelocateData(long) const;
private:
void ReadCompressedFileData(const C3DAnimFileObjectChunk&,CSimpleMemoryPool&);

private:CCountedPointer<CSimpleMemoryPool> Allocator;CMovableResourceMemoryPool* MemoryPool;long MemoryHandle;
 CCountedPointer<C3DAnimationSequence> MovementTrack;CCountedPointer<C3DAnimationSequenceData> MovementTrackData;
 float SamplesPerSecond;unsigned long FrameCount,SequenceCount;C3DAnimationSequence* Sequences;
 unsigned long SequenceDataBufferSize;C3DAnimationSequenceData* SequenceDataBuffer;bool* BoneMask;bool* RigidBoneMask;
 C3DVector* RigidBonePositions;bool LockedFromMemMemoryRelocation;unsigned char _pad_75[3];unsigned char InstanceList[4];
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimation2)==0x7c);
#endif
#else
#ifndef FABLE_ENGINE_C3DANIMATION2_H
#define FABLE_ENGINE_C3DANIMATION2_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof C3DAnimation2 = 0x88.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof C3DAnimation2 = 0x7c.
// Retail evidence: 2 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class C3DAnimationSequence;
class C3DAnimationSequenceData;
class C3DVector;
class CCPPointerInfo;
class CMovableResourceMemoryPool;
class CSimpleMemoryPool;

#pragma pack(push, 1)
class C3DAnimation2 {
public:
    void*                       __vftable;                   // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char               _pad_0x04[0x4];              // +0x04
    unsigned char               _pad_0x08[0x4];              // +0x08
    unsigned char               _pad_0x0c[0x4];              // +0x0c
    unsigned char               _pad_0x10[0x4];              // +0x10
    unsigned char               _pad_0x14[0x4];              // +0x14
    unsigned char               _pad_0x18[0x4];              // +0x18
    unsigned char               _pad_0x1c[0x2];              // +0x1c
    unsigned char               _pad_0x1e[0x12];             // +0x1e
    CSimpleMemoryPool*          Allocator_Data;              // +0x30 CCountedPointer<CSimpleMemoryPool>::Data
    CCPPointerInfo*             Allocator_Info;              // +0x34 CCountedPointer<CSimpleMemoryPool>::Info
    CMovableResourceMemoryPool* MemoryPool;                  // +0x38
    long                        MemoryHandle;                // +0x3c
    C3DAnimationSequence*       MovementTrack_Data;          // +0x40 CCountedPointer<C3DAnimationSequence>::Data
    CCPPointerInfo*             MovementTrack_Info;          // +0x44 CCountedPointer<C3DAnimationSequence>::Info
    C3DAnimationSequenceData*   MovementTrackData_Data;      // +0x48 CCountedPointer<C3DAnimationSequenceData>::Data
    CCPPointerInfo*             MovementTrackData_Info;      // +0x4c CCountedPointer<C3DAnimationSequenceData>::Info
    float                       SamplesPerSecond;            // +0x50
    unsigned long               FrameCount;                  // +0x54
    unsigned long               SequenceCount;               // +0x58
    C3DAnimationSequence*       Sequences;                   // +0x5c
    unsigned long               SequenceDataBufferSize;      // +0x60
    C3DAnimationSequenceData*   SequenceDataBuffer;          // +0x64
    void*                       BoneMask;                    // +0x68 bool *
    void*                       RigidBoneMask;               // +0x6c bool *
    C3DVector*                  RigidBonePositions;          // +0x70
    bool                        LockedFromMemMemoryRelocation; // +0x74
    unsigned char               _pad_0x75[0x1];              // +0x75
    unsigned char               _pad_0x76[0x2];              // +0x76
    unsigned char               InstanceList[0x4];           // +0x78 list<CIntelligentPointer<C3DAnimation2>,std::allocator<CIntelligentPointer<C3DAnimation2>_>_>
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(C3DAnimation2) == 0x7c);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, Allocator_Data) == 0x30);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, Allocator_Info) == 0x34);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, MemoryPool) == 0x38);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, MemoryHandle) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, MovementTrack_Data) == 0x40);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, MovementTrack_Info) == 0x44);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, MovementTrackData_Data) == 0x48);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, MovementTrackData_Info) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, SamplesPerSecond) == 0x50);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, FrameCount) == 0x54);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, SequenceCount) == 0x58);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, Sequences) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, SequenceDataBufferSize) == 0x60);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, SequenceDataBuffer) == 0x64);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, BoneMask) == 0x68);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, RigidBoneMask) == 0x6c);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, RigidBonePositions) == 0x70);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, LockedFromMemMemoryRelocation) == 0x74);
FABLE_STATIC_ASSERT(offsetof(C3DAnimation2, InstanceList) == 0x78);

#endif // FABLE_ENGINE_C3DANIMATION2_H

#endif
