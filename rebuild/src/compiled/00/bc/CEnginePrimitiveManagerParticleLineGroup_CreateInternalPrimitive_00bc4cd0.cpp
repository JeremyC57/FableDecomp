// CEnginePrimitiveManagerParticleLineGroup::CreateInternalPrimitive(type) -- allocate either the particle group or its individual primitive.
// Leaf: ?CreateInternalPrimitive@CEnginePrimitiveManagerParticleLineGroup@@UAEPAVCEngineInternalPrimitiveBase@@W4EEnginePrimitiveType@@@Z
// Identity: proven; own manager RTTI virtualslot1 and original PDB exact CreateInternalPrimitive(EEnginePrimitiveType) API; own created RTTI CEngineInternalPrimitiveParticleLineGroup / CEngineInternalPrimitiveParticleLine with original CIVCountedPointeeBase/CEnginePersistentPrimitiveBase/InternalPrimitiveBase/ParticleGroup chains; group/small IDs26/21 and flags0x88000/0x188000, actual allocation sizes188/104; external original CEngineInternalPrimitiveBase(long,ulong)00B8FF00 exact PDB JK (not enum prototype). Group header/list/vector/named sphere/default ctor store evidence and original typed members.
// Layout: shared base/group/manager/quarantined derived headers retained unchanged as evidence; local real original bases and typed member definitions correct flattened data. Genuine STLport list<CEnginePrimitiveHandle>/list<long> sentinels16/12 and vector-based CArray<CPrimitiveIterator>. Actual C3DBoundingSphere default zeros only its four floats; C3DVector default leaves scalar fields untouched. Unused enum member constants descriptive. Full actual PC sizes compiler-checked; no raw new-size substitute.
#define CEngineInternalPrimitiveBase CEngineInternalPrimitiveBase_HeaderLayout
#include "engine/CEngineInternalPrimitiveBase.h"
#undef CEngineInternalPrimitiveBase
#define CEngineInternalPrimitiveParticleGroup CEngineInternalPrimitiveParticleGroup_HeaderLayout
#include "engine/CEngineInternalPrimitiveParticleGroup.h"
#undef CEngineInternalPrimitiveParticleGroup
#define CEnginePrimitiveManagerParticleLineGroup CEnginePrimitiveManagerParticleLineGroup_HeaderLayout
#include "engine/CEnginePrimitiveManagerParticleLineGroup.h"
#undef CEnginePrimitiveManagerParticleLineGroup
#define CEngineInternalPrimitiveParticleLineGroup CEngineInternalPrimitiveParticleLineGroup_HeaderLayout
#include "engine/_quarantine/CEngineInternalPrimitiveParticleLineGroup.h"
#undef CEngineInternalPrimitiveParticleLineGroup
#define CEngineInternalPrimitiveParticleLine CEngineInternalPrimitiveParticleLine_HeaderLayout
#include "engine/_quarantine/CEngineInternalPrimitiveParticleLine.h"
#undef CEngineInternalPrimitiveParticleLine
#ifndef _MSC_VER
#define __forceinline inline
#endif
#ifdef _MSC_VER
extern "C" void* __cdecl malloc(unsigned int);extern "C" void __cdecl free(void*);
#else
#include <stdlib.h>
#endif
class CBaseClass{public:virtual ~CBaseClass(){} };class CBaseClassNonCopyable:public CBaseClass{};class CIVCountedPointeeBase{public:virtual ~CIVCountedPointeeBase(){}unsigned long IVRefCount;};class CEnginePersistentPrimitiveBase:public CIVCountedPointeeBase{public:long Type;};template<class T>class CIVCountedPointer{public:CIVCountedPointeeBase* Object;};class CEnginePrimitiveHandle:public CBaseClass{public:CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;};
class C3DVector{public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};class C2DVector{public:float X,Y;};class C3DBoundingSphere{public:C3DVector Center;float Radius;C3DBoundingSphere():Center(0,0,0),Radius(0){}};class CMatrix3x4{public:C3DVector Rows[4];};class CRGBColour{public:unsigned char R,G,B,A;};class CRightHandedSet{public:C3DVector Up,Forward;};enum EEnginePrimitiveType{TYPE_SMALL=21,TYPE_GROUP=26};enum EEngineInterpolationSet{INTERPOLATION_UNNAMED};enum EEngineEffectWaterStage{WATER_STAGE_UNNAMED};enum EEnginePrimitiveBlendMode{BLEND_MODE_UNNAMED};enum EEnginePrimitiveBlendOp{BLEND_OP_UNNAMED};
class CEngineSceneGridCell;class CEngineInternalPrimitiveBase:public CEnginePersistentPrimitiveBase{public:
    unsigned long                 EnginePrimitiveFlags;      // +0x0c
    float                         AlphaMipBias;              // +0x10
    float                         FadeDistance_FadeBeginDistance; // +0x14 CEngineFadeDistance::FadeBeginDistance
    float                         FadeDistance_MaxDrawDistance; // +0x18 CEngineFadeDistance::MaxDrawDistance
    unsigned long                 OcclusionLayerBitMask;     // +0x1c
    CEngineSceneGridCell*         GridCell;                  // +0x20
    signed char                   GridPos_Level;             // +0x24 CEngineSceneGridPos::Level
    unsigned char                 GridPos_X;                 // +0x25 CEngineSceneGridPos::X
    unsigned char                 GridPos_Y;                 // +0x26 CEngineSceneGridPos::Y
    unsigned char                 Alpha;                     // +0x27
    unsigned char                 AlphaRef;                  // +0x28
    unsigned char                 PreloadStat;               // +0x29
    unsigned char                 _pad_0x2a[0x2];            // +0x2a
    EEngineInterpolationSet        InterpolationSet;     // +0x2c EEngineInterpolationSet
    unsigned long                 RenderLayerMask;           // +0x30
    unsigned char                 PostRenderEnable[0x2];     // +0x34 bitfield group: uchar:1
    unsigned short                LastFrameRendered;         // +0x36
    CEngineInternalPrimitiveBase* NextPrimitive;             // +0x38
    CEngineInternalPrimitiveBase** RefPrimitive;              // +0x3c CEngineInternalPrimitiveBase * *
    CEngineInternalPrimitiveBase* NextLayerMask;             // +0x40
    CEngineInternalPrimitiveBase** RefLayerMask;              // +0x44 CEngineInternalPrimitiveBase * *
 CEngineInternalPrimitiveBase(long,unsigned long);};
namespace std{template<class T>class allocator{};template<class V,class T,class Al>class _STLP_alloc_proxy:public Al{public:V _M_data;};template<class T,class Al>class _Vector_base{public:T* _M_start;T* _M_finish;_STLP_alloc_proxy<T*,T,Al> _M_end_of_storage;_Vector_base():_M_start(0),_M_finish(0){_M_end_of_storage._M_data=0;}~_Vector_base(){if(_M_start)free(_M_start);}};template<class T,class Al=allocator<T> >class vector:public _Vector_base<T,Al>{};struct _List_node_base{_List_node_base* _M_next;_List_node_base* _M_prev;};template<class T>struct _List_node:public _List_node_base{T _M_data;};template<class T,class Al=allocator<T> >class list{public:_List_node_base* _M_node;list():_M_node(0){_List_node<T>* p=(_List_node<T>*)malloc(sizeof(_List_node<T>));p->_M_next=p;p->_M_prev=p;_M_node=p;}~list(){free(_M_node);}};}
template<class T>class CArray:public std::vector<T>{};class CEngineInternalPrimitiveParticleGroup:public CEngineInternalPrimitiveBase{public:class CPrimitiveIterator;std::list<CEnginePrimitiveHandle> Primitives;CArray<CPrimitiveIterator> PrimitiveIDToIterator;std::list<long> FreePrimitiveIDs;C3DBoundingSphere BSphere;float DistanceFromCam;CMatrix3x4 WorldMat;bool ScreenDisplacement,ReadZBuffer;EEngineEffectWaterStage WaterStage;bool ZBufferWritable,ClipEffectToWeatherMask,EnableDithering;__forceinline CEngineInternalPrimitiveParticleGroup(long type,unsigned long flags):CEngineInternalPrimitiveBase(type,flags){}};
class CEngineInternalPrimitiveParticleLineGroup:public CEngineInternalPrimitiveParticleGroup{public:unsigned long BankIndex;unsigned short Flags;EEnginePrimitiveBlendMode BlendMode;EEnginePrimitiveBlendOp BlendOp; __forceinline CEngineInternalPrimitiveParticleLineGroup():CEngineInternalPrimitiveParticleGroup(TYPE_GROUP,0x88000){}};
class CEngineInternalPrimitiveParticleLine:public CEngineInternalPrimitiveBase{public:C3DVector Start,End;float Width;CRGBColour Colour; __forceinline CEngineInternalPrimitiveParticleLine():CEngineInternalPrimitiveBase(TYPE_SMALL,0x188000){}};
class CEnginePrimitiveRenderUtil:public CBaseClassNonCopyable{};class CEnginePrimitiveManager:public CEnginePrimitiveRenderUtil{public:virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType)=0;};class CEnginePrimitiveManagerParticleLineGroup:public CEnginePrimitiveManager{public:virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType);};CEngineInternalPrimitiveBase* CEnginePrimitiveManagerParticleLineGroup::CreateInternalPrimitive(EEnginePrimitiveType type){if(type==TYPE_GROUP)return new CEngineInternalPrimitiveParticleLineGroup;if(type==TYPE_SMALL)return new CEngineInternalPrimitiveParticleLine;return 0;}
#ifdef _MSC_VER
typedef char CorrectBaseSize[(sizeof(CEngineInternalPrimitiveBase)==0x48)?1:-1];typedef char CorrectGroupBase[(sizeof(CEngineInternalPrimitiveParticleGroup)==0xAC)?1:-1];typedef char CorrectGroupSize[(sizeof(CEngineInternalPrimitiveParticleLineGroup)==188)?1:-1];typedef char CorrectSmallSize[(sizeof(CEngineInternalPrimitiveParticleLine)==104)?1:-1];
#endif