// C3DAnimationSequenceData::InitialiseFromFileData(C3DAnimFileSequenceChunk const &,CSimpleMemoryPool &) -- compresses rotation and translation tracks into the shared animation pool.
// Leaf: ?InitialiseFromFileData@C3DAnimationSequenceData@@QAE_NABVC3DAnimFileSequenceChunk@@AAVCSimpleMemoryPool@@@Z
// Grade: DIFFER(3475v3491) -- rotation-palette vector copy uses add/[ecx] where retail uses lea edx,[eax+ecx]; one x87 order in the scaling-identity magnitude (retail: mem*mem, st1*st1, st2*st2); alignment pads (lea esp,[esp] vs nop) shift every later jump
// Context: grades against the Codex row context $S/codex11_dl/x/rows/00a4efc0/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = Codex selected; 00a4efc0 probes in probe/00a4efc0.log
#define FABLE_RETAIL_ANIMATION_RETRY
#define FABLE_RETAIL_VECTOR_COPY_INITIALISE
#define FABLE_RETAIL_ANIMATION_SEQUENCE
#define FABLE_RETAIL_ANIMATION_SHARED
#include <math.h>
#include "engine/C3DKeyframe.h"
#include "engine/CArray.h"
#include "engine/C3DAnimationSequenceData.h"
#include "engine/C3DAnimFileSequenceChunk.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/GFMaths.h"
#pragma intrinsic(sqrt)
#pragma intrinsic(fabs)

namespace std {
template<> __declspec(noinline)
void vector<C3DAnimationSequenceData::C3DVectorWord>::_M_insert_overflow(
 pointer __position,const value_type& __x,const __false_type&,size_type __fill_len,bool __atend)
{
 const size_type __old_size=size();
 const size_type __len=__old_size+(max)(__old_size,__fill_len);
 pointer __new_start=this->_M_end_of_storage.allocate(__len);
 pointer __new_finish=__new_start;
 __new_finish=__uninitialized_copy(this->_M_start,__position,__new_start,__false_type());
 if(__fill_len==1){_Construct(__new_finish,__x);++__new_finish;}
 else __new_finish=__uninitialized_fill_n(__new_finish,__fill_len,__x,__false_type());
 if(!__atend)__new_finish=__uninitialized_copy(__position,this->_M_finish,__new_finish,__false_type());
 _M_clear();_M_set(__new_start,__new_finish,__new_start+__len);
}
}

namespace std {
template<> __declspec(noinline)
void vector<unsigned char>::_M_insert_overflow(pointer __position,const value_type& __x,const __true_type&,size_type __fill_len,bool __atend)
{
 const size_type __old_size=size();
 const size_type __len=__old_size+(max)(__old_size,__fill_len);
 pointer __new_start=this->_M_end_of_storage.allocate(__len);
 pointer __new_finish=(pointer)__copy_trivial(this->_M_start,__position,__new_start);
 __new_finish=fill_n(__new_finish,__fill_len,__x);
 if(!__atend)__new_finish=(pointer)__copy_trivial(__position,this->_M_finish,__new_finish);
 _M_clear();_M_set(__new_start,__new_finish,__new_start+__len);
}
}

bool C3DAnimationSequenceData::InitialiseFromFileData(const C3DAnimFileSequenceChunk& file_data,CSimpleMemoryPool& alloc)
{
 CCharString bone_name=file_data.GetBoneName();
 BoneName=alloc.New<char>(bone_name.GetLength()+1);
 strcpy(BoneName,bone_name.GetAsCharArray());
 BoneType=file_data.GetBoneType();SamplesPerSecond=file_data.GetSamplesPerSecond();FrameCount=(unsigned short)file_data.GetMatrixCount();AnimatedFlag=false;
 CArray<C3DKeyframe> keyframes(FrameCount);
 for(unsigned long c_frame=0;c_frame<FrameCount;++c_frame){const CMatrix3x4& matrix=*file_data.GetMatrix(c_frame);C3DKeyframe& keyframe=keyframes[c_frame];keyframe.InitialiseFromMatrix(matrix);keyframe.SetScaling(C3DVector(1,1,1));}
 PositionFactor=0.0f;ScalingFactor=0.0f;
 for(unsigned long c_frame=0;c_frame<FrameCount;++c_frame){const C3DKeyframe& keyframe=keyframes[c_frame];
  PositionFactor=(std::max)(PositionFactor,GetMaximumAbsoluteComponent(keyframe.GetPosition()));
  ScalingFactor=(std::max)(ScalingFactor,GetMaximumAbsoluteComponent(keyframe.GetScaling()));
 }
 PositionFactor*=1.0f/32767.0f;ScalingFactor*=1.0f/32767.0f;
 float position_normalise=PositionFactor>0.0f?1.0f/PositionFactor:0.0f;
 float scaling_normalise=ScalingFactor>0.0f?1.0f/ScalingFactor:0.0f;
 CArray<CQuaternion> rotation_track;
 CArray<C3DVectorWord> position_track,scaling_track;
 CArray<unsigned char> paletted_rotation_track,paletted_position_track,paletted_scaling_track;
 rotation_track.resize(FrameCount);
 paletted_rotation_track.clear();
 CArray<CQuaternion> unique_rotations;
 for(unsigned long c_frame=0;c_frame<FrameCount;++c_frame){const C3DKeyframe& keyframe=keyframes[c_frame];rotation_track[c_frame]=keyframe.GetRotation();long unique_index=-1;
  for(unsigned long c_unique=0;c_unique<unique_rotations.size();++c_unique){if(keyframe.Rotation.Equals(unique_rotations[c_unique])){unique_index=c_unique;break;}}
  if(unique_index==-1){unique_index=unique_rotations.size();unique_rotations.push_back(keyframe.Rotation);}
  paletted_rotation_track.push_back((unsigned char)unique_index);
 }
 if(unique_rotations.size()==1){if(unique_rotations[0].IsIdentity()){RotationTrackMode=TRACKMODE_IDENTITY;rotation_track.clear();paletted_rotation_track.clear();}else{RotationTrackMode=TRACKMODE_CONSTANT;rotation_track=unique_rotations;paletted_rotation_track.clear();}}
 else{AnimatedFlag=true;unsigned long compressed_size=unique_rotations.GetByteSize()+paletted_rotation_track.GetByteSize();unsigned long uncompressed_size=rotation_track.GetByteSize();if(compressed_size<uncompressed_size&&unique_rotations.size()<=255){RotationTrackMode=TRACKMODE_PALETTED;rotation_track=unique_rotations;}else{RotationTrackMode=TRACKMODE_NORMAL;paletted_rotation_track.clear();}}
 position_track.resize(FrameCount);paletted_position_track.resize(FrameCount);
 CArray<C3DVectorWord> unique_positions;
 for(unsigned long c_frame=0;c_frame<FrameCount;++c_frame){const C3DKeyframe& keyframe=keyframes[c_frame];long unique_index=-1;C3DVectorWord& position=position_track[c_frame];position=keyframe.GetPosition()*position_normalise;
  for(unsigned long c_unique=0;c_unique<unique_positions.size();++c_unique){if(position==unique_positions[c_unique]){unique_index=c_unique;break;}}
  if(unique_index==-1){unique_index=unique_positions.size();unique_positions.push_back(position);}paletted_position_track[c_frame]=(unsigned char)unique_index;
 }
 if(unique_positions.size()==1){const C3DKeyframe& keyframe=keyframes[0];if(!keyframe.GetPosition().IsNonZero()){PositionTrackMode=TRACKMODE_IDENTITY;position_track.clear();}else{PositionTrackMode=TRACKMODE_CONSTANT;position_track=unique_positions;}paletted_position_track.clear();}
 else{AnimatedFlag=true;unsigned long compressed_size=unique_positions.GetByteSize()+paletted_position_track.GetByteSize();unsigned long uncompressed_size=position_track.GetByteSize();if(compressed_size<uncompressed_size&&unique_positions.size()<=255){PositionTrackMode=TRACKMODE_PALETTED;position_track=unique_positions;}else{PositionTrackMode=TRACKMODE_NORMAL;paletted_position_track.clear();}}
 scaling_track.resize(FrameCount);paletted_scaling_track.resize(FrameCount);
 CArray<C3DVectorWord> unique_scalings;
 for(unsigned long c_frame=0;c_frame<FrameCount;++c_frame){const C3DKeyframe& keyframe=keyframes[c_frame];long unique_index=-1;C3DVectorWord& scaling=scaling_track[c_frame];scaling=keyframe.GetScaling()*scaling_normalise;
  for(unsigned long c_unique=0;c_unique<unique_scalings.size();++c_unique){if(scaling==unique_scalings[c_unique]){unique_index=c_unique;break;}}
  if(unique_index==-1){unique_index=unique_scalings.size();unique_scalings.push_back(scaling);}paletted_scaling_track[c_frame]=(unsigned char)unique_index;
 }
 if(unique_scalings.size()==1){const C3DKeyframe& keyframe=keyframes[0];if((keyframe.GetScaling()-C3DVector(1,1,1)).GetMagnitude()<DELTA_NOTIONAL_ZERO){ScalingTrackMode=TRACKMODE_IDENTITY;scaling_track.clear();}else{ScalingTrackMode=TRACKMODE_CONSTANT;scaling_track=unique_scalings;}paletted_scaling_track.clear();}
 else{AnimatedFlag=true;unsigned long compressed_size=unique_scalings.GetByteSize()+paletted_scaling_track.GetByteSize();unsigned long uncompressed_size=scaling_track.GetByteSize();if(compressed_size<uncompressed_size&&unique_scalings.size()<=255){ScalingTrackMode=TRACKMODE_PALETTED;scaling_track=unique_scalings;}else{ScalingTrackMode=TRACKMODE_NORMAL;paletted_scaling_track.clear();}}
 RotationTrackSize=(unsigned short)rotation_track.size();PositionTrackSize=(unsigned short)position_track.size();PalettedRotationTrackSize=(unsigned short)paletted_rotation_track.size();PalettedPositionTrackSize=(unsigned short)paletted_position_track.size();
 RotationTrack=alloc.New<CQuaternion>(RotationTrackSize);PositionTrack=alloc.New<C3DVectorWord>(PositionTrackSize);PalettedRotationTrack=alloc.New<unsigned char>(PalettedRotationTrackSize);PalettedPositionTrack=alloc.New<unsigned char>(PalettedPositionTrackSize);
 std::copy(rotation_track.begin(),rotation_track.end(),RotationTrack);std::copy(position_track.begin(),position_track.end(),PositionTrack);std::copy(paletted_rotation_track.begin(),paletted_rotation_track.end(),PalettedRotationTrack);std::copy(paletted_position_track.begin(),paletted_position_track.end(),PalettedPositionTrack);
 if(RotationTrackMode==TRACKMODE_IDENTITY&&PositionTrackMode==TRACKMODE_IDENTITY&&ScalingTrackMode==TRACKMODE_IDENTITY)IdentityFlag=true;else IdentityFlag=false;
 return true;
}

float C3DAnimationSequenceData::GetMaximumAbsoluteComponent(const C3DVector& vect)
{
 return ((float)fabs(vect.X)>(float)fabs(vect.Y)?(float)fabs(vect.X):(float)fabs(vect.Y))>(float)fabs(vect.Z)?((float)fabs(vect.X)>(float)fabs(vect.Y)?(float)fabs(vect.X):(float)fabs(vect.Y)):(float)fabs(vect.Z);
}
