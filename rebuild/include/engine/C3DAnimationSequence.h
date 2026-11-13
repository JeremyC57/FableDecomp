#ifndef FABLE_ENGINE_C3DANIMATIONSEQUENCE_H
#define FABLE_ENGINE_C3DANIMATIONSEQUENCE_H
#include "rebuild_abi.h"
#include "engine/C3DAnimationSequenceData.h"
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE
#include "fable_compiler.h"
#endif
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimationSequence{
public:C3DAnimationSequence():Data(0),ParentIndex(-1),Shared(false){}
 unsigned long GetFrameCount()const{return Data->GetFrameCount();}float GetSamplesPerSecond()const{return Data->GetSamplesPerSecond();}
 bool InitialiseFromFileData(const C3DAnimFileSequenceChunk&,C3DAnimationSequenceData*,CSimpleMemoryPool&);
 void ReadCompressedFromFile(CDataInputStream&,unsigned long,C3DAnimationSequenceData*,CSimpleMemoryPool&);
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE
 // Retail opt-in (chunk 2005, row 0x00A4D920): FableWin.pdb ?InlineGetKeyframeInterpolate@C3DAnimationSequence@@QBEXAAVC3DKeyframe@@KM@Z
 // (38-byte debug forwarder to the sequence data's InlineGetKeyframeInterpolate; expanded in retail).
 FABLE_FORCEINLINE void InlineGetKeyframeInterpolate(C3DKeyframe& keyframe,unsigned long frame,float fraction)const{Data->InlineGetKeyframeInterpolate(keyframe,frame,fraction);}
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCE_FRAMEKEYFRAME
 // Retail opt-in (chunk 2044, rows 0x00AA0090 / 0x00A9F540): Ego_r.pdb / FableWin.pdb ?IsIdentity@C3DAnimationSequence@@QBE_NXZ and
 // ?GetKeyframeInterpolate@C3DAnimationSequence@@QBEXAAVC3DKeyframe@@KM@Z, inline in retail: the caller loads Data and reads its
 // IdentityFlag / calls the data's out-of-line GetKeyframeInterpolate 0x00A4C1F0 itself (0x00AA0247..0x00AA025E). Needs
 // FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_ISIDENTITY and FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_INTERPOLATE.
 bool IsIdentity() const { return Data->IsIdentity(); }
 void GetKeyframeInterpolate(C3DKeyframe& keyframe, unsigned long frame, float fraction) const { Data->GetKeyframeInterpolate(keyframe, frame, fraction); }
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCE_GETKEYFRAME
 // Retail opt-in (chunk 2014, row 0x00A4E300): Ego_r.pdb / FableWin.pdb ?GetKeyframeInterpolate@C3DAnimationSequence@@QBEXAAVC3DKeyframe@@M@Z,
 // inline in retail (the caller loads the sequence's Data and calls the data's out-of-line copy 0x00A4DEA0 itself, 0x00A4E3B7).
 void GetKeyframeInterpolate(C3DKeyframe& keyframe,float time)const{Data->GetKeyframeInterpolate(keyframe,time);}
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONSEQUENCE_INITIALISE
 // Retail opt-in (chunk 2014, row 0x00A50510 C3DAnimation2::InitialiseAnimationSequences): Ego_r.pdb / FableWin.pdb
 // ?SetParentIndex@C3DAnimationSequence@@QAEXJ@Z (the low word stored, 0x00A50621), ?InitialiseShared@C3DAnimationSequence@@QAEXABV1@@Z
 // (the other sequence's data, shared, no parent: 0x00A5062F..0x00A50638) and
 // ?InitialiseAsConstant@C3DAnimationSequence@@QAEXABVCMatrix3x4@@KMPAVC3DAnimationSequenceData@@AAVCSimpleMemoryPool@@@Z (own data,
 // not shared, then the data's out-of-line InitialiseAsConstant 0x00A4FD70: 0x00A50677..0x00A5067D); all inline in retail.
 void SetParentIndex(long parent){ParentIndex=(short)parent;}
 void InitialiseShared(const C3DAnimationSequence& other){Data=other.Data;Shared=true;ParentIndex=-1;}
 // InitialiseAsConstant is an ordinary member of the TU (retail out-of-line copy 0x00A4FE30, `ret 14h`, between
 // C3DAnimationSequenceData::InitialiseAsConstant 0x00A4FD70 and C3DAnimation2::InitialiseSharedAnimation 0x00A4FE60); the row defines it.
 // InitialiseShared is a header inline (its COMDAT 0x00A4C140 sits in the TU's inline run).
 void InitialiseAsConstant(const CMatrix3x4& matrix,unsigned long frame_count,float samples_per_second,C3DAnimationSequenceData* data,CSimpleMemoryPool& memory_pool);
#endif
private:C3DAnimationSequenceData* Data;short ParentIndex;bool Shared;
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimationSequence)==0x8);
#endif
