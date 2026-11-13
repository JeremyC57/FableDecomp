#ifndef FABLE_ENGINE_C3DANIMATIONSTATS_H
#define FABLE_ENGINE_C3DANIMATIONSTATS_H
#include "rebuild_abi.h"
#include "engine/CBaseObject.h"
#include "engine/EAnimationType.h"
#ifdef FABLE_RETAIL_C3DANIMATIONSTATS_MOVEMENTVECTOR
#include "engine/C3DVector.h"
#endif
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class C3DAnimationStats:public CBaseObject{public:
C3DAnimationStats(const C3DAnimationStats&);
C3DAnimationStats();
virtual ~C3DAnimationStats();
virtual C3DVector GetMovementOffsetAtTime(float) const=0;
virtual C3DVector GetMovementAtTime(float) const=0;
virtual bool HasRotation() const=0;
virtual CQuaternion GetRotationAtTime(float) const=0;
virtual void GetDeltaMovement(CMatrix3x4&,float,float) const=0;
void InitialiseStats(const C3DAnimFile&,EAnimationType);
#ifdef FABLE_RETAIL_C3DANIMATIONSTATS_ISRELATIVE
// Retail opt-in (chunk 2044, rows 0x00AA0090 / 0x00A9F540): Ego_r.pdb / FableWin.pdb ?IsRelative@C3DAnimationStats@@QBE_NXZ and
// ?IsPartial@C3DAnimationStats@@QBE_NXZ, inline in retail: the AnimType flag bits (ANIMTYPE_BASE_RELATIVE 4 / ANIMTYPE_COMBINATION_DELTA 5
// carry bit 2, ANIMTYPE_COMBINATION_PARTIAL 3 bit 1) read as bools from one load, `mov ecx,eax ; shr ecx,2 ; test cl,1` and
// `shr eax,1 ; test al,1` (0x00AA01F1 / 0x00AA0553; landed CombineBlendedBaseAnimations 0x00A9F344 has the same IsRelative shape).
bool IsRelative() const { return (AnimType & 4) != 0; }
bool IsPartial() const { return (AnimType & 2) != 0; }
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONSTATS_GETTYPE
// Retail opt-in (chunk 2014, row 0x00A50510): both PDBs' ?GetType@C3DAnimationStats@@QBE?AW4EAnimationType@@XZ, inline in retail (FableWin
// emits it at 0x032265C0, right after InitialiseAnimationSequences; 0x00A5051A reads [this+8] in place).
EAnimationType GetType() const { return AnimType; }
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONSTATS_MOVEMENTVECTOR
// Retail opt-in (chunk 2014, row 0x00A4E300 C3DAnimation2::GetDeltaMovement): Ego_r.pdb member MovementVector is a C3DVector (+0x12;
// needs FABLE_RETAIL_C3DVECTOR_PACKED: retail C3DVector is 1-aligned, so the offset is unchanged; the size assert below catches a miss) and both PDBs' ?GetMovementVector@C3DAnimationStats@@QBE?AVC3DVector@@XZ
// is inline in retail (no out-of-line copy; 0x00A4E866 / 0x00A4E872 / 0x00A4E87C read [this+12h] / [+16h] / [+1Ah] in place).
C3DVector GetMovementVector() const { return MovementVector; }
#ifdef FABLE_RETAIL_C3DANIMATIONSTATS_PROTECTED
// Retail opt-in (chunk 2014): C3DAnimation2::GetDeltaMovement 0x00A4E866..0x00A4E87C multiplies [this+12h] / [16h] / [1Ah] in place
// (`fld st(0) ; fmul [edi+12h]`), while the by-value getter always materialises its copy first (GetMovementAtTime 0x00A4E1A7
// `fld [ecx+12h] ; mov eax,[ecx+1Ah] ; fld [ecx+16h]`): the derived class reads the member directly. Access only, no layout change.
private:EAnimationType AnimType;float Duration;bool LoopFlag,MovementFlag;protected:C3DVector MovementVector;private:unsigned char _pad_1e[2];unsigned char TimingEvents[12];};
#else
private:EAnimationType AnimType;float Duration;bool LoopFlag,MovementFlag;C3DVector MovementVector;unsigned char _pad_1e[2];unsigned char TimingEvents[12];};
#endif
#else
private:EAnimationType AnimType;float Duration;bool LoopFlag,MovementFlag;unsigned char MovementVector[12];unsigned char _pad_1e[2];unsigned char TimingEvents[12];};
#endif
FABLE_STATIC_ASSERT(sizeof(C3DAnimationStats)==0x2c);
#endif
