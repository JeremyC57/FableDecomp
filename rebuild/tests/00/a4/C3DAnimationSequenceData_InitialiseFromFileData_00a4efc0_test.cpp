// 00a4efc0 C3DAnimationSequenceData::InitialiseFromFileData(const C3DAnimFileSequenceChunk& file_data, CSimpleMemoryPool& alloc) -- compress one bone's file matrices into identity / constant / normal / paletted tracks
// Leaf: ?InitialiseFromFileData@C3DAnimationSequenceData@@QAE_NABVC3DAnimFileSequenceChunk@@AAVCSimpleMemoryPool@@@Z
// Flags: /Gr
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_COPY
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_GETBYTESIZE_PASS23
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CARRAY_DEBUGGER_BOUNDS_PASS27
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF

#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_ADDMATRIX
#define FABLE_RETAIL_C3DANIMFILESEQUENCECHUNK_GETTERS
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_FROMFILEDATA
#define FABLE_RETAIL_C3DANIMATIONSEQUENCEDATA_VECTORWORD_FROMVECTOR
#define FABLE_RETAIL_C3DKEYFRAME_FROMMATRIX
#define FABLE_RETAIL_CQUATERNION_EQUALS
#define FABLE_RETAIL_STLP_VECTOR_ORIGINAL_RESIZE_INSERT_CHAIN_PASS22
#define FABLE_RETAIL_STLP_ORIGINAL_COPY_TRIVIAL_BACKWARD_PASS22
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_STLP_PASS22_COPY_BACKWARD_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_ORIGINAL_SCOPE_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#include <string.h>
#include <math.h>
#define private public
#define protected public
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CArray.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DVector.h"
#include "engine/C4DVector.h"
#include "engine/CQuaternion.h"
#include "engine/C3DKeyframe.h"
#include "engine/CChunkedFileChunk.h"
#include "engine/CChunkedFileSuperChunk.h"
#include "engine/C3DAnimFileSequenceChunk.h"
#include "engine/C3DAnimationSequenceData.h"
#undef private
#undef protected

#define FABLE_RETAIL_STLP_ANIMATION_BYTE_VECTOR_OWNERS
#include "stlport/fable_animation_byte_vector_owners.h"

bool C3DAnimationSequenceData::InitialiseFromFileData(const C3DAnimFileSequenceChunk& file_data, CSimpleMemoryPool& alloc)
{
    CCharString bone_name = file_data.GetBoneName();
    BoneName = alloc.AllocateType<char>(bone_name.GetLength() + 1);
    strcpy(BoneName, bone_name.GetAsCharArray());
    BoneType = file_data.GetBoneType();
    SamplesPerSecond = file_data.GetSamplesPerSecond();
    FrameCount = (unsigned short)file_data.GetMatrixCount();
    AnimatedFlag = false;
    bool status = true;
    CArray<C3DKeyframe> keyframes(FrameCount);
    if (false) {} else for (unsigned long c_frame = 0; c_frame < FrameCount; ++c_frame) {
        C3DKeyframe& keyframe = keyframes[c_frame];
        const CMatrix3x4& matrix = *file_data.GetMatrix(c_frame);
        keyframe.InitialiseFromMatrix(matrix);
        keyframe.SetScaling(C3DVector(1, 1, 1));
    }
    PositionFactor = 0.0f;
    ScalingFactor = 0.0f;
    if (false) {} else for (unsigned long c_frame = 0; c_frame < FrameCount; ++c_frame) {
        const C3DKeyframe& keyframe = keyframes[c_frame];
        PositionFactor = (std::max)(PositionFactor, GetMaximumAbsoluteComponent(keyframe.GetPosition()));
        ScalingFactor = (std::max)(ScalingFactor, GetMaximumAbsoluteComponent(keyframe.GetScaling()));
    }
    PositionFactor *= 1.0f / 32767.0f;
    ScalingFactor *= 1.0f / 32767.0f;
    float position_normalise = PositionFactor > 0.0f ? 1.0f / PositionFactor : 0.0f;
    float scaling_normalise = ScalingFactor > 0.0f ? 1.0f / ScalingFactor : 0.0f;
    CArray<CQuaternion> rotation_track;
    CArray<C3DVectorWord> position_track;
    CArray<C3DVectorWord> scaling_track;
    CArray<unsigned char> paletted_rotation_track;
    CArray<unsigned char> paletted_position_track;
    CArray<unsigned char> paletted_scaling_track;
    rotation_track.resize(FrameCount);
    paletted_rotation_track.clear();
    CArray<CQuaternion> unique_rotations;
    if (false) {} else for (unsigned long c_frame = 0; c_frame < FrameCount; ++c_frame) {
        const C3DKeyframe& keyframe = keyframes[c_frame];
        rotation_track[c_frame] = keyframe.Rotation;
        long unique_index = -1;
        if (false) {} else for (unsigned long c_unique = 0; c_unique < unique_rotations.size(); ++c_unique) {
            if (keyframe.Rotation.Equals(unique_rotations[c_unique])) {
                unique_index = c_unique;
                break;
            }
        }
        if (unique_index == -1) {
            unique_index = unique_rotations.size();
            unique_rotations.push_back(keyframe.Rotation);
        }
        paletted_rotation_track.push_back((unsigned char)unique_index);
    }
    if (unique_rotations.size() == 1) {
        if (unique_rotations[0].IsIdentity()) {
            RotationTrackMode = TRACKMODE_IDENTITY;
            rotation_track.clear();
            paletted_rotation_track.clear();
        } else {
            RotationTrackMode = TRACKMODE_CONSTANT;
            rotation_track = unique_rotations;
            paletted_rotation_track.clear();
        }
    } else {
        AnimatedFlag = true;
        unsigned long compressed_size = unique_rotations.GetByteSize() + paletted_rotation_track.GetByteSize();
        unsigned long uncompressed_size = rotation_track.GetByteSize();
        if (compressed_size < uncompressed_size && unique_rotations.size() <= 255) {
            RotationTrackMode = TRACKMODE_PALETTED;
            rotation_track = unique_rotations;
        } else {
            RotationTrackMode = TRACKMODE_NORMAL;
            paletted_rotation_track.clear();
        }
    }
    position_track.resize(FrameCount);
    paletted_position_track.resize(FrameCount);
    CArray<C3DVectorWord> unique_positions;
    if (false) {} else for (unsigned long c_frame = 0; c_frame < FrameCount; ++c_frame) {
        const C3DKeyframe& keyframe = keyframes[c_frame];
        C3DVectorWord& position = position_track[c_frame];
        position = keyframe.GetPosition() * position_normalise;
        long unique_index = -1;
        if (false) {} else for (unsigned long c_unique = 0; c_unique < unique_positions.size(); ++c_unique) {
            if (position == unique_positions[c_unique]) {
                unique_index = c_unique;
                break;
            }
        }
        if (unique_index == -1) {
            unique_index = unique_positions.size();
            unique_positions.push_back(position);
        }
        paletted_position_track[c_frame] = (unsigned char)unique_index;
    }
    if (unique_positions.size() == 1) {
        const C3DKeyframe& keyframe = keyframes[0];
        if (!keyframe.GetPosition().IsNonZero()) {
            PositionTrackMode = TRACKMODE_IDENTITY;
            position_track.clear();
        } else {
            PositionTrackMode = TRACKMODE_CONSTANT;
            position_track = unique_positions;
        }
        paletted_position_track.clear();
    } else {
        AnimatedFlag = true;
        unsigned long compressed_size = unique_positions.GetByteSize() + paletted_position_track.GetByteSize();
        unsigned long uncompressed_size = position_track.GetByteSize();
        if (compressed_size < uncompressed_size && unique_positions.size() <= 255) {
            PositionTrackMode = TRACKMODE_PALETTED;
            position_track = unique_positions;
        } else {
            PositionTrackMode = TRACKMODE_NORMAL;
            paletted_position_track.clear();
        }
    }
    scaling_track.resize(FrameCount);
    paletted_scaling_track.resize(FrameCount);
    CArray<C3DVectorWord> unique_scalings;
    if (false) {} else for (unsigned long c_frame = 0; c_frame < FrameCount; ++c_frame) {
        const C3DKeyframe& keyframe = keyframes[c_frame];
        C3DVectorWord& scaling = scaling_track[c_frame];
        scaling = keyframe.GetScaling() * scaling_normalise;
        long unique_index = -1;
        if (false) {} else for (unsigned long c_unique = 0; c_unique < unique_scalings.size(); ++c_unique) {
            if (scaling == unique_scalings[c_unique]) {
                unique_index = c_unique;
                break;
            }
        }
        if (unique_index == -1) {
            unique_index = unique_scalings.size();
            unique_scalings.push_back(scaling);
        }
        paletted_scaling_track[c_frame] = (unsigned char)unique_index;
    }
    if (unique_scalings.size() == 1) {
        const C3DKeyframe& keyframe = keyframes[0];
        if ((keyframe.GetScaling() - C3DVector(1, 1, 1)).GetMagnitude() < DELTA_NOTIONAL_ZERO) {
            ScalingTrackMode = TRACKMODE_IDENTITY;
            scaling_track.clear();
        } else {
            ScalingTrackMode = TRACKMODE_CONSTANT;
            scaling_track = unique_scalings;
        }
        paletted_scaling_track.clear();
    } else {
        AnimatedFlag = true;
        unsigned long compressed_size = unique_scalings.GetByteSize() + paletted_scaling_track.GetByteSize();
        unsigned long uncompressed_size = scaling_track.GetByteSize();
        if (compressed_size < uncompressed_size && unique_scalings.size() <= 255) {
            ScalingTrackMode = TRACKMODE_PALETTED;
            scaling_track = unique_scalings;
        } else {
            ScalingTrackMode = TRACKMODE_NORMAL;
            paletted_scaling_track.clear();
        }
    }
    RotationTrackSize = (unsigned short)rotation_track.size();
    PositionTrackSize = (unsigned short)position_track.size();
    PalettedRotationTrackSize = (unsigned short)paletted_rotation_track.size();
    PalettedPositionTrackSize = (unsigned short)paletted_position_track.size();
    RotationTrack = alloc.AllocateType<CQuaternion>(RotationTrackSize);
    PositionTrack = alloc.AllocateType<C3DVectorWord>(PositionTrackSize);
    PalettedRotationTrack = alloc.AllocateType<unsigned char>(PalettedRotationTrackSize);
    PalettedPositionTrack = alloc.AllocateType<unsigned char>(PalettedPositionTrackSize);
    std::copy(rotation_track.begin(), rotation_track.end(), RotationTrack);
    std::copy(position_track.begin(), position_track.end(), PositionTrack);
    std::copy(paletted_rotation_track.begin(), paletted_rotation_track.end(), PalettedRotationTrack);
    std::copy(paletted_position_track.begin(), paletted_position_track.end(), PalettedPositionTrack);
    SetupIdentityFlag();
    return status;
}

inline void C3DAnimationSequenceData::SetupIdentityFlag()
{
    if (RotationTrackMode == TRACKMODE_IDENTITY && PositionTrackMode == TRACKMODE_IDENTITY && ScalingTrackMode == TRACKMODE_IDENTITY)
        IdentityFlag = true;
    else
        IdentityFlag = false;
}

inline float C3DAnimationSequenceData::GetMaximumAbsoluteComponent(const C3DVector& vect)
{
    return ((float)fabs(vect.X) > (float)fabs(vect.Y) ? (float)fabs(vect.X) : (float)fabs(vect.Y)) > (float)fabs(vect.Z) ?
        ((float)fabs(vect.X) > (float)fabs(vect.Y) ? (float)fabs(vect.X) : (float)fabs(vect.Y)) : (float)fabs(vect.Z);
}

#include <stdio.h>
#include <stdlib.h>

// The input and result are real engine objects. Only external services have fixture bodies.
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CCharString::CCharString():PStringData(0) {}
CCharString::CCharString(const CCharString& s):PStringData(s.PStringData) {}
CCharString::~CCharString() {}
const char CCharString::NullChar=0;
const float DELTA_NOTIONAL_ZERO=0.0001f;
CChunkedFileChunk::CChunkedFileChunk(const char*) {}
CChunkedFileChunk::~CChunkedFileChunk() {}
CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char* s):CChunkedFileChunk(s) {}
CChunkedFileSuperChunk::~CChunkedFileSuperChunk() {}
C3DAnimFileSequenceChunk::C3DAnimFileSequenceChunk():CChunkedFileSuperChunk("test") {}
const CCharString CChunkedFileChunk::GetChunkDescription() const {return CCharString();}
const CCharString CChunkedFileChunk::GetChunkInfo() const {return CCharString();}
const CCharString C3DAnimFileSequenceChunk::GetChunkDescription() const {return CCharString();}
const CCharString C3DAnimFileSequenceChunk::GetChunkInfo() const {return CCharString();}
int CChunkedFileChunk::IsSuperChunk() const {return 0;}
int CChunkedFileChunk::IsUnknownType() const {return 0;}
int CChunkedFileSuperChunk::IsSuperChunk() const {return 1;}
CChunkedFileSuperChunk* CChunkedFileChunk::GetAsSuperChunk(){return 0;}
const CChunkedFileSuperChunk* CChunkedFileChunk::GetAsSuperChunk() const{return 0;}
CChunkedFileSuperChunk* CChunkedFileSuperChunk::GetAsSuperChunk(){return this;}
const CChunkedFileSuperChunk* CChunkedFileSuperChunk::GetAsSuperChunk() const{return this;}
CChunkedFileChunk* CChunkedFileChunk::InitNewInstance(CChunkedFileChunk* p)const{return p;}
CChunkedFileChunk* CChunkedFileSuperChunk::InitNewInstance(CChunkedFileChunk* p)const{return p;}
CChunkedFileChunk* C3DAnimFileSequenceChunk::CreateNewInstance() const{return 0;}
void CChunkedFileSuperChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&) {}
void C3DAnimFileSequenceChunk::ReadFromFile(CDataInputStream&,unsigned long,CChunkedFileLoader&) {}
void CChunkedFileSuperChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const {}
void C3DAnimFileSequenceChunk::WriteToFile(CDataOutputStream&,CChunkedFileSaver&) const {}
CSimpleMemoryPool::CSimpleMemoryPool(unsigned long,unsigned long) {}
void CSimpleMemoryPool::ClearMemoryBlocks(){}
void* CSimpleMemoryPool::Allocate(unsigned long n){return malloc(n);}
bool CQuaternion::IsIdentity() const{return X==0 && Y==0 && Z==0 && W==1;}
bool CQuaternion::Equals(const CQuaternion& q) const {return X==q.X && Y==q.Y && Z==q.Z && W==q.W;}
static unsigned decoded;
bool C3DKeyframe::InitialiseFromMatrix(const CMatrix3x4& m){
    ++decoded;
    Rotation.X=m.E11; Rotation.Y=0; Rotation.Z=0; Rotation.W=1;
    Position.X=m.E41; Position.Y=m.E42; Position.Z=0; Position.W=0;
    Scaling.X=2; Scaling.Y=4; Scaling.Z=8; Scaling.W=0;
    return true;
}
static unsigned expectedMode(unsigned n,unsigned distinct,unsigned width,bool identity){
    if(distinct==1) return identity?0:1;
    return distinct<=255 && distinct*width+n<n*width?3:2;
}
static bool check(unsigned n,unsigned rq,unsigned pq,bool zeroRot,bool zeroPos,bool named){
    C3DAnimFileSequenceChunk input;
    input.BoneType=(EBoneType)3; input.SamplesPerSecond=32;
    char name[]="Spine"; CCharStringData stringData;
    stringData.Data.PData=name; stringData.Data.StringLength=5;
    input.BoneName.PStringData=named?&stringData:0;
    for(unsigned i=0;i<n;++i){
        CMatrix3x4 m; memset(&m,0,sizeof(m));
        m.E11=float(i%rq+(zeroRot?0:1));
        m.E41=zeroPos?0:float(i%pq+1); m.E42=zeroPos?0:32767.0f;
        input.Matrices.push_back(m);
    }
    CSimpleMemoryPool pool(0,0); C3DAnimationSequenceData out;
    decoded=0;
    if(!out.InitialiseFromFileData(input,pool) || decoded!=n)return false;
    unsigned r=expectedMode(n,n?rq:0,16,zeroRot),p=expectedMode(n,n?pq:0,6,zeroPos),s=n?0:2;
    unsigned rs=r==0?0:r==2?n:rq,ps=p==0?0:p==2?n:pq;
    bool ok=out.FrameCount==n && out.SamplesPerSecond==32 && out.BoneType==3
        && strcmp(out.BoneName,named?name:"")==0 && out.RotationTrackMode==r
        && out.PositionTrackMode==p && out.ScalingTrackMode==s
        && !!out.AnimatedFlag==(r>=2||p>=2||s>=2) && !!out.IdentityFlag==(r==0&&p==0&&s==0)
        && out.RotationTrackSize==rs && out.PositionTrackSize==ps
        && out.PalettedRotationTrackSize==(r==3?n:0) && out.PalettedPositionTrackSize==(p==3?n:0)
        && !!out.RotationTrack==!!rs && !!out.PositionTrack==!!ps
        && !!out.PalettedRotationTrack==(r==3) && !!out.PalettedPositionTrack==(p==3)
        && out.PositionFactor==(n&&!zeroPos?1.0f:0.0f);
    for(unsigned i=0;i<n && ok;++i){
        unsigned ri=r==3?out.PalettedRotationTrack[i]:r==1?0:i;
        unsigned pi=p==3?out.PalettedPositionTrack[i]:p==1?0:i;
        if(r==3 && ri!=i%rq || p==3 && pi!=i%pq) ok=false;
        if(rs && (ri>=rs || out.RotationTrack[ri].X!=float(i%rq+(zeroRot?0:1))
            || out.RotationTrack[ri].W!=1.0f)) ok=false;
        if(ps && (pi>=ps || out.PositionTrack[pi].X!=int(i%pq+1)
            || out.PositionTrack[pi].Y!=32767 || out.PositionTrack[pi].Z!=0)) ok=false;
    }
    free(out.BoneName); free(out.RotationTrack); free(out.PositionTrack);
    free(out.PalettedRotationTrack); free(out.PalettedPositionTrack);
    return ok;
}
int main(){
    const unsigned cases[][3]={{0,1,1},{1,1,1},{2,1,1},{2,2,2},{6,2,5},{6,5,2},
      {16,15,2},{16,2,15},{64,3,4},{64,1,2},{64,2,1},{512,255,255},{512,256,256}};
    for(unsigned i=0;i<sizeof(cases)/sizeof(cases[0]);++i)
        for(unsigned r=0;r<(cases[i][1]==1?2U:1U);++r)
            for(unsigned p=0;p<(cases[i][2]==1?2U:1U);++p)
                for(unsigned name=0;name!=2;++name)
                    if(!check(cases[i][0],cases[i][1],cases[i][2],r!=0,p!=0,name!=0)) return 1;
    puts("00a4efc0_TEST PASS");return 0;
}