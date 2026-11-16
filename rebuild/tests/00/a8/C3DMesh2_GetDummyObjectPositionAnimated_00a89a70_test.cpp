// Self-contained full-body test: real shared class models and controlled external operations.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp

#endif
#include "engine/EPlayerMode.h"
#define private public
#define protected public

#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DMESH2_BUILDMESH
#define FABLE_RETAIL_C3DMESH2_DTOR_PASS23
#define FABLE_RETAIL_C3DMESHSTATS_LIFETIME_PASS23
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL_COMPLETE_ENUM_PASS23
// C3DMesh2 original dummy animation query; shared full class models.
// Leaf: ?GetDummyObjectPositionAnimated@C3DMesh2@@UBEXABVC3DDummyObject@@AAV2@ABVC3DAnimationBlendState@@AAVC3DAnimationManager@@_N@Z
// Flags: /d2inlT800
#include "fable_compiler.h"
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS
#define FABLE_RETAIL_C3DMESH2_HELPERPOINTS
#define FABLE_RETAIL_C3DMESH2_BONES
#define FABLE_RETAIL_C3DBONE2_PEEK
#define FABLE_RETAIL_C3DDUMMYOBJECT_MESH_FRIEND_PASS23
#define FABLE_RETAIL_C3DANIMATIONMANAGER_MESHTRANSFORMS
#define FABLE_RETAIL_C3DANIMATIONMANAGER_BONETRANSFORM
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRAILING_DATA_PASS23
#define FABLE_RETAIL_CMATRIX3X4_DATA_UNION
#define FABLE_RETAIL_CMATRIX3X4_PLAIN_INLINE_PASS23
#define FABLE_RETAIL_CMATRIX3X4_PRESCALE_VECTOR_PASS23
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_GETSCALED
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DDummyObject.h"
#include "engine/C3DBone2.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DAnimationManager.h"
void C3DMesh2::GetDummyObjectPositionAnimated(const C3DDummyObject& dummy, C3DDummyObject& dest, const C3DAnimationBlendState& anim_state, C3DAnimationManager& anim_manager,bool use_morph_scale)const
{
    dest.BoneIndex=dummy.BoneIndex;
    if(dummy.GetBoneIndex()!=-1) {
        long bone_index=dummy.GetBoneIndex();
        const C3DBone2* bone=GetBone(bone_index);
        CMatrix3x4& target=dest.Transform;
        target.SetMultiply(dummy.Transform,bone->PeekLocalizationMatrix());
        CMatrix3x4 bone_transform;
        C3DVector morph_scale;
        anim_manager.CalcBoneTransformation(anim_state,*this,bone_index,bone_transform,morph_scale);
        if(use_morph_scale) dest.Transform.PreScale(morph_scale);
        else dest.Transform.SetTranslateOnly(dest.Transform.GetTranslation().GetScaled(morph_scale));
        dest.Transform.PostMultiply(bone_transform);
    }else dest.Transform=dummy.Transform;
}

#include "engine/C3DAnimationBlendState.h"


CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CCharString::CCharString():PStringData(0){}CCharString::~CCharString(){assert(!PStringData);}
C3DMeshStats::C3DMeshStats():MemoryPool(0),StatsMemoryHandle(-1),PackedNames(0),PackedNamesSize(0),AnimatedFlag(false){}
C3DMeshStats::~C3DMeshStats(){}
C3DMesh2::C3DMesh2():MemoryHandle(-1),PrimMemoryHandle(-1),BoneNameMemoryHandle(-1),MaterialCount(0),Materials(0),PrimitiveCount(0),Primitives(0),BoneCount(0),Bones(0),BoneKeyframes(0),BoneNameIndices(0),BoneNameData(0),BoneNameDataSize(0),BoneSpaceTransforms(0),ClothFlag(false),LockedFromMemoryRelocation(false),MeshID(0){}
C3DMesh2::~C3DMesh2(){assert(!Materials&&!Primitives&&MemoryHandle==-1&&PrimMemoryHandle==-1&&BoneNameMemoryHandle==-1);}
CSimpleMemoryPool::CSimpleMemoryPool(unsigned long c,unsigned long g):FirstMemoryBlock(0),Capacity(c),GrowthGranularity(g){}
void CSimpleMemoryPool::ClearMemoryBlocks(){assert(!FirstMemoryBlock);}
C3DAnimationManager::C3DAnimationManager():MeshBank(0),MemoryPool(0,0x1000),MemoryPool2(0,0x1000){}
C3DAnimationManager::~C3DAnimationManager(){}
C3DAnimationBlendState::C3DAnimationBlendState():Data(0){}C3DAnimationBlendState::~C3DAnimationBlendState(){assert(!Data);}
C3DDummyObject::C3DDummyObject(){Transform.E11=Transform.E22=Transform.E33=1;Transform.E12=Transform.E13=Transform.E21=Transform.E23=Transform.E31=Transform.E32=Transform.E41=Transform.E42=Transform.E43=0;}
static void ReadMatrix(const CMatrix3x4&m,float v[12]){v[0]=m.E11;v[1]=m.E12;v[2]=m.E13;v[3]=m.E21;v[4]=m.E22;v[5]=m.E23;v[6]=m.E31;v[7]=m.E32;v[8]=m.E33;v[9]=m.E41;v[10]=m.E42;v[11]=m.E43;}
static void WriteMatrix(CMatrix3x4&m,const float v[12]){m.E11=v[0];m.E12=v[1];m.E13=v[2];m.E21=v[3];m.E22=v[4];m.E23=v[5];m.E31=v[6];m.E32=v[7];m.E33=v[8];m.E41=v[9];m.E42=v[10];m.E43=v[11];}
// Independent array oracle; all arithmetic cases use exact binary fractions below2^24.
static void Product(const float a[12],const float b[12],float out[12]){for(int row=0;row<4;++row)for(int col=0;col<3;++col){float sum=0;for(int k=0;k<3;++k)sum+=a[row*3+k]*b[k*3+col];if(row==3)sum+=b[9+col];out[row*3+col]=sum;}}
static void EqualMatrix(const CMatrix3x4&m,const float v[12]){float actual[12];ReadMatrix(m,actual);for(int i=0;i<12;++i)assert(actual[i]==v[i]);}
static C3DAnimationManager*ExpectedManager;static C3DMesh2*ExpectedMesh;static C3DAnimationBlendState*ExpectedFirst,*ExpectedSecond;static unsigned long ExpectedBone;static float ExpectedWeight;static int Calls;
static CMatrix3x4*ObservedOutput;static C3DDummyObject*ObservedDummy;static float BeforeCallback[12];static CMatrix3x4 AnimatedTransform;static C3DVector AnimatedScale;
static void FillAnimated(C3DAnimationManager*m,const C3DMesh2&mesh,unsigned long bone,CMatrix3x4&transform,C3DVector&scale){assert(m==ExpectedManager&&&mesh==ExpectedMesh&&bone==ExpectedBone&&!Calls);EqualMatrix(*ObservedOutput,BeforeCallback);if(ObservedDummy)assert(ObservedDummy->BoneIndex==(long)ExpectedBone);transform=AnimatedTransform;scale=AnimatedScale;++Calls;}
void C3DAnimationManager::CalcBoneTransformation(const C3DAnimationBlendState&a,const C3DMesh2&mesh,unsigned long bone,CMatrix3x4&transform,C3DVector&scale){assert(&a==ExpectedFirst);FillAnimated(this,mesh,bone,transform,scale);}

bool CMovableResource::CanRelocateData(long) const{abort();}
unsigned long CMovableResource::Verify() const{abort();}
void C3DMesh2::RelocateData(long, void*, void*, long){abort();}
C3DVector C3DMesh2::GetHelperPointPositionAnimated(const C3DHelperPoint&,const C3DAnimationBlendState&,C3DAnimationManager&)const{abort();}
C3DVector C3DMesh2::GetHelperPointPositionAnimated(const C3DHelperPoint&,const C3DAnimationBlendState&,const C3DAnimationBlendState&,float,C3DAnimationManager&)const{abort();}
void C3DMesh2::GetDummyObjectPositionAnimated(const C3DDummyObject&,C3DDummyObject&,const C3DAnimationBlendState&,const C3DAnimationBlendState&,float,C3DAnimationManager&,bool)const{abort();}
void C3DMesh2::GetDummyObjectTransformAnimated(const C3DDummyObject&,CMatrix3x4&,const C3DAnimationBlendState&,C3DAnimationManager&,bool)const{abort();}
void C3DMesh2::GetDummyObjectTransformAnimated(const C3DDummyObject&,CMatrix3x4&,const C3DAnimationBlendState&,const C3DAnimationBlendState&,float,C3DAnimationManager&,bool)const{abort();}

int main(){C3DMesh2 mesh;C3DBone2 bones[4];mesh.BoneCount=4;mesh.Bones=bones;C3DAnimationManager manager;C3DAnimationBlendState first,second;ExpectedManager=&manager;ExpectedMesh=&mesh;ExpectedFirst=&first;ExpectedSecond=&second;C3DDummyObject input,output;CMatrix3x4 outputMatrix;unsigned cases=0;static const float scales[]={-2.0f,-0.5f,0.0f,0.5f,1.0f,2.0f};
 for(int seed=0;seed<16384;++seed){input.BoneIndex=seed%5-1;output.BoneIndex=-77;ExpectedBone=(unsigned long)input.BoneIndex;ExpectedWeight=(float)(seed%5)*0.25f;Calls=0;bool apply=seed/5%2!=0;float inputValues[12],boneValues[12],animatedValues[12],localized[12],expected[12],sentinel[12];
  for(int i=0;i<12;++i){inputValues[i]=(float)((seed*(i+3)+i*7)%25-12)*0.25f;animatedValues[i]=(float)((seed*(i+7)+i*11)%17-8)*0.5f;sentinel[i]=91.0f+(float)i;}WriteMatrix(input.Transform,inputValues);WriteMatrix(output.Transform,sentinel);WriteMatrix(outputMatrix,sentinel);WriteMatrix(AnimatedTransform,animatedValues);AnimatedScale=C3DVector(scales[seed/7%6],scales[seed/11%6],scales[seed/13%6]);
  for(int n=0;n<4;++n){bones[n].NameCRC=100+n;bones[n].Parent=n-1;bones[n].OriginalNoChildren=0;for(int i=0;i<12;++i)boneValues[i]=(float)((seed*(i+5)+n*13+i*3)%21-10)*0.25f;WriteMatrix(bones[n].LocalizationMatrix,boneValues);}
  if(input.BoneIndex==-1){for(int i=0;i<12;++i)expected[i]=inputValues[i];}else{ReadMatrix(bones[input.BoneIndex].LocalizationMatrix,boneValues);Product(inputValues,boneValues,localized);for(int i=0;i<12;++i)BeforeCallback[i]=localized[i];float scaled[12];for(int i=0;i<12;++i)scaled[i]=localized[i];if(apply){for(int row=0;row<3;++row){float scale=row==0?AnimatedScale.X:row==1?AnimatedScale.Y:AnimatedScale.Z;for(int col=0;col<3;++col)scaled[row*3+col]*=scale;}}else{scaled[9]*=AnimatedScale.X;scaled[10]*=AnimatedScale.Y;scaled[11]*=AnimatedScale.Z;}Product(scaled,animatedValues,expected);}
  ObservedOutput=&output.Transform;ObservedDummy=&output;
  mesh.GetDummyObjectPositionAnimated(input,output,first,manager,apply);
  EqualMatrix(output.Transform,expected);assert(Calls==(input.BoneIndex!=-1));assert(output.BoneIndex==input.BoneIndex);++cases;
 }
 // The no-bone branch copies every float bit unchanged, including special values, without consulting animation.
 unsigned patterns[]={0,0x80000000u,0x7fc12345u,0x7f800000u,0xff800000u};
 for(int n=0;n<5;++n){float values[12];for(int i=0;i<12;++i)memcpy(&values[i],&patterns[(n+i)%5],4);WriteMatrix(input.Transform,values);input.BoneIndex=-1;Calls=0;bool apply=false;mesh.GetDummyObjectPositionAnimated(input,output,first,manager,apply);float actual[12];ReadMatrix(output.Transform,actual);for(int i=0;i<12;++i)assert(memcmp(&actual[i],&values[i],4)==0);assert(!Calls);}
 assert(cases==16384);puts("00a89a70_TEST PASS");return 0;}