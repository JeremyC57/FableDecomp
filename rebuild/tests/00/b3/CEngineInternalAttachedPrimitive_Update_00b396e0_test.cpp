// Self-contained behavior fixture. Selected row and handle algorithms are embedded verbatim.
// Rendering, string allocation and virtual engine operations are controlled boundaries.
#include <stdio.h>
#include <assert.h>
#include <string.h>
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
#define FABLE_STATIC_ASSERT(x)
static int assignments,deleted,notified,updates,creates,adds,conversions,transforms;
static unsigned long last_flags;
static int events[32],event_count;
static void event(int value){assert(event_count<32);events[event_count++]=value;}
class CBaseClass{public:virtual ~CBaseClass(){}};
class CMatrix3x4{public:float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;};
class CCharString{public:int Text;CCharString():Text(0){}const CCharString& operator=(const CCharString& other){++assignments;Text=other.Text;return *this;}};
class CEnginePrimitiveHandle;
class CIVCountedPointeeBase {
public:
 CIVCountedPointeeBase():IVRefCount(1){}
 virtual ~CIVCountedPointeeBase(){}
 virtual void Delete()const{++deleted;event(1);}
 void AddRef()const{++IVRefCount;}
 long Release()const{return --IVRefCount;}
 long GetRefCount()const{return IVRefCount;}
 mutable unsigned long IVRefCount;
};
class CEnginePersistentPrimitiveBase:public CIVCountedPointeeBase {
 friend class CEnginePrimitiveHandle;
public:
 CEnginePersistentPrimitiveBase(long type):Type(type){}
 long GetType()const{return Type;}
 virtual void NotifyReleased(const CEnginePrimitiveHandle&){++notified;event(2);}
private:long Type;
};
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#ifndef FABLE_ENGINE_CIVCOUNTEDPOINTER_H
#define FABLE_ENGINE_CIVCOUNTEDPOINTER_H

template<class T> class CIVCountedPointer {
public:
    CIVCountedPointer() : Object(0) { }
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_FROM_RAW
    
    
    
    
    
    CIVCountedPointer(T* object) : Object(object) { }
#endif
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER
    
    
    
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
#endif
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT
    
    
    
    
    
    
    
    
    ~CIVCountedPointer() {
        if (Object) {
            CIVCountedPointeeBase* object = Object;
            if (object->Release() == 0) object->Delete();
        }
    }
#else
    ~CIVCountedPointer() { Release(); }
#endif
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER
    
    
    
    
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
    
    
    
    
    
    void Reset(T* object) { Release(); Object = object; }
#else
    void Reset(T* object) { Release(); Object = object; if (Object) Object->AddRef(); }
#endif
#else
    void Reset(T* object);
#endif
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
    
    
    
    CIVCountedPointer& operator=(const CIVCountedPointer& other) {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object) Object->AddRef();
        }
        return *this;
    }
#endif
    T* operator->() const { return static_cast<T*>(Object); }
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
    
    
    
    
    template<class U> CIVCountedPointer(const CIVCountedPointer<U>& other) : Object(other.InternalGetObject()) { if (Object) Object->AddRef(); }
    CIVCountedPointeeBase* InternalGetObject() const { return Object; }
    T& operator*() const { return *static_cast<T*>(Object); }
#endif
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object ? Object->GetRefCount() : 0; }
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
    
    
    
    
    void Release() {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
#elif defined(FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER)
    
    
    
    void Release() {
        if (Object) {
            CIVCountedPointeeBase* object = Object;
            if (object->Release() == 0) object->Delete();
            Object = 0;
        }
    }
#else
    void Release() {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (object->Release() == 0) object->Delete();
            Object = 0;
        }
    }
#endif
#else
    void Release() {
        CIVCountedPointeeBase* object = Object;
        if (object && object->Release() == 0) object->Delete();
        Object = 0;
    }
#endif
private:
    CIVCountedPointeeBase* Object;
};
#endif
#ifndef FABLE_ENGINE_CENGINEPRIMITIVEHANDLE_H
#define FABLE_ENGINE_CENGINEPRIMITIVEHANDLE_H




class CEnginePrimitiveHandle : public CBaseClass {
public:
    CEnginePrimitiveHandle() { }
#if defined(FABLE_RETAIL_CENGINEPRIMITIVEHANDLE) && defined(FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR)
    
    
    FABLE_FORCEINLINE virtual ~CEnginePrimitiveHandle() { Clear(); }
#else
    virtual ~CEnginePrimitiveHandle() {
        if (!Data.IsNull() && Data->GetRefCount() == 1) Data->NotifyReleased(*this);
    }
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
    void SetData(CEnginePersistentPrimitiveBase* data){Clear();Data.Reset(data);}
    bool IsNull()const{return Data.IsNull();}
    long GetType()const{return Data->GetType();}
    CEnginePersistentPrimitiveBase& GetData()const{return *Data.operator->();}
#else
    void SetData(CEnginePersistentPrimitiveBase* data);
#endif
#ifdef FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
    
    const CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& other) {
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
        Clear();
#endif
        Data=other.Data;return *this;
    }
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
    
    
    
    void Clear() {
        if (Data.GetRefCount() == 1) Data->NotifyReleased(*this);
        Data.Reset(0);
    }
#else
    void Clear() {
        if (!Data.IsNull() && Data->GetRefCount() == 1) Data->NotifyReleased(*this);
        Data.Release();
    }
#endif
private:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveHandle) == 8);
#endif
#ifndef FABLE_ENGINE_PRIMITIVE_ENUMS_H
#define FABLE_ENGINE_PRIMITIVE_ENUMS_H
enum EEnginePrimitiveType {
 ENGINE_PRIMITIVE_NULL=0,
 ENGINE_PRIMITIVE_GROUP=1,
 ENGINE_PRIMITIVE_CHANGER=2,
 ENGINE_PRIMITIVE_FILLED_RECT=3,
 ENGINE_PRIMITIVE_OUTLINE_RECT=4,
 ENGINE_PRIMITIVE_TEXT=5,
 ENGINE_PRIMITIVE_LINE=6,
 ENGINE_PRIMITIVE_SPRITE=7,
 ENGINE_PRIMITIVE_STATIC_MESH=8,
 ENGINE_PRIMITIVE_ANIMATING_MESH=9,
 ENGINE_PRIMITIVE_MORPHED_STATIC_MESH=10,
 ENGINE_PRIMITIVE_MORPHED_ANIMATING_MESH=11,
 ENGINE_PRIMITIVE_STATIC_MESH_FROM_FILE=12,
 ENGINE_PRIMITIVE_ANIMATING_MESH_FROM_FILE=13,
 ENGINE_PRIMITIVE_TARGET=14,
 ENGINE_PRIMITIVE_LIGHT=15,
 ENGINE_PRIMITIVE_SHADOWED_SPOT_LIGHT=16,
 ENGINE_PRIMITIVE_FLARE_SPRITE=17,
 ENGINE_PRIMITIVE_GAME_MESH=18,
 ENGINE_PRIMITIVE_WEAPON_TRAIL=19,
 ENGINE_PRIMITIVE_SPRITE_TRAIL=20,
 ENGINE_PRIMITIVE_PARTICLE_LINE=21,
 ENGINE_PRIMITIVE_PARTICLE_SPRITE=22,
 ENGINE_PRIMITIVE_PARTICLE_MESH=23,
 ENGINE_PRIMITIVE_PARTICLE_GROUP=24,
 ENGINE_PRIMITIVE_PARTICLE_DECAL=25,
 ENGINE_PRIMITIVE_PARTICLE_LINE_GROUP=26,
 ENGINE_PRIMITIVE_PARTICLE_SPRITE_GROUP=27,
 ENGINE_PRIMITIVE_PARTICLE_MESH_GROUP=28,
 ENGINE_PRIMITIVE_PARTICLE_SPRITE_TRAIL_GROUP=29,
 ENGINE_PRIMITIVE_PARTICLE_DECAL_GROUP=30,
 ENGINE_PRIMITIVE_WEATHER_MASK=31,
 ENGINE_PRIMITIVE_2D_PARTICLE_GROUP=32,
 ENGINE_PRIMITIVE_2D_VIEWPORT=33,
 ENGINE_PRIMITIVE_2D_SPRITE=34,
 ENGINE_PRIMITIVE_2D_CLOCK_SPRITE=35,
 ENGINE_PRIMITIVE_2D_STATIC_MESH=36,
 ENGINE_PRIMITIVE_2D_ANIMATING_MESH=37,
 ENGINE_PRIMITIVE_2D_MORPHED_ANIMATING_MESH=38,
 ENGINE_PRIMITIVE_2D_TEXT=39,
 ENGINE_PRIMITIVE_DECAL=40,
 ENGINE_PRIMITIVE_MAX=41
};
enum EEngineInterpolationSet {
 ENGINE_INTERPOLATION_SET_0=0,
 ENGINE_INTERPOLATION_SET_1=1,
 ENGINE_INTERPOLATION_SET_2=2,
 ENGINE_INTERPOLATION_SET_NUM=3,
 ENGINE_INTERPOLATION_SET_ALWAYSUPDATE=0,
 ENGINE_INTERPOLATION_SET_PAUSED=1,
 ENGINE_INTERPOLATION_SET_BULLETTIME=2,
 ENGINE_INTERPOLATION_SET_DEFAULT=0
};
enum EEngineRenderLayer {
 RENDER_LAYER_NULL=0,
 RENDER_LAYER_SETUP_LIGHTS=1,
 RENDER_LAYER_SHADOW_BUFFER_RENDER=2,
 RENDER_LAYER_LANDSCAPE_BASE=4,
 RENDER_LAYER_MESHES_OCCLUSION=8,
 RENDER_LAYER_MESHES_NO_OCCLUSION=16,
 RENDER_LAYER_NON_SORTED_PRIMITIVES=32,
 RENDER_LAYER_LANDSCAPE_POST_SOLID_SCENE=64,
 RENDER_LAYER_OUTLINE_FILL=128,
 RENDER_LAYER_OUTLINE_STENCIL=256,
 RENDER_LAYER_OUTLINE_ALPHA_STENCIL_MASK=512,
 RENDER_LAYER_STENCIL_MESHES=1024,
 RENDER_LAYER_ALPHA_MESHES=2048,
 RENDER_LAYER_BOUNDING_BOXES=4096,
 RENDER_LAYER_BACKGROUND=8192,
 RENDER_LAYER_DECAL=16384,
 RENDER_LAYER_PREWATER_EFFECTS=32768,
 RENDER_LAYER_WATER_DISPLACEMENT=65536,
 RENDER_LAYER_WATER=131072,
 RENDER_LAYER_POSTWATER_MESHES=262144,
 RENDER_LAYER_POSTWATER_EFFECTS=524288,
 RENDER_LAYER_WATER_EFFECTS=1048576,
 RENDER_LAYER_EFFECTS=2097152,
 RENDER_LAYER_FLARE_EFFECTS=4194304,
 RENDER_LAYER_FLARE_VISIBILITY=8388608,
 RENDER_LAYER_COLOUR_FILTER=16777216,
 RENDER_LAYER_DEBUG=33554432,
 RENDER_LAYER_OUTLINE_APPLY_TO_SCREEN=67108864,
 RENDER_LAYER_DISPLACEMENT_BUFFER=134217728,
 RENDER_LAYER_DISPLACEMENT_STENCIL=268435456,
 RENDER_LAYER_SCREEN_EFFECTS=536870912,
 RENDER_LAYER_GRAPHICS_FRAME_END=1073741824,
 RENDER_LAYER_2D=-2147483648,
 RENDER_LAYER_MAX=-2147483647
};
#endif

class CEnginePrimitive {
public:EEnginePrimitiveType Type;
 EEnginePrimitiveType GetType()const{return Type;}
};
class CEngineInternalPrimitiveBase:public CEnginePersistentPrimitiveBase {
public:
 CEngineInternalPrimitiveBase(long type):CEnginePersistentPrimitiveBase(type),Child(0),TransformOK(true),Enable(true),GroupCalls(0),GroupIndex(0){}
 CEnginePersistentPrimitiveBase* Child;
 bool TransformOK,Enable;
 int GroupCalls;unsigned long GroupIndex;
 virtual void Update(const CEnginePrimitive& src,unsigned long flags){++updates;event(4);last_flags=flags;assert(src.Type==GetType());}
 virtual CEnginePersistentPrimitiveBase* GetChildPrimitive(unsigned long index){++GroupCalls;GroupIndex=index;return Child;}
 virtual bool GetObjectTransform(CMatrix3x4& out)const{
  ++transforms;event(5);assert(!Enable);if(!TransformOK)return false;
  out.E11=1;out.E12=2;out.E13=3;out.E21=4;out.E22=5;out.E23=6;
  out.E31=7;out.E32=8;out.E33=9;out.E41=10;out.E42=11;out.E43=12;return true;
 }
 void SetEnable(bool enable){Enable=enable;}
};
#ifndef FABLE_ENGINE_CENGINEPRIMITIVEATTACHMENTINFO_H
#define FABLE_ENGINE_CENGINEPRIMITIVEATTACHMENTINFO_H

class CEnginePrimitiveAttachmentInfo {
public:
 CEnginePrimitiveAttachmentInfo();
 unsigned long Flags;
 CCharString ParentDummyName;
 long ParentDummyIndex;
 CCharString ChildDummyName;
 long ChildDummyIndex;
};
#endif
#ifndef FABLE_ENGINE_CENGINEINTERNALATTACHEDPRIMITIVE_H
#define FABLE_ENGINE_CENGINEINTERNALATTACHEDPRIMITIVE_H




inline CEngineInternalPrimitiveBase& FABLE_FASTCALL GFGetPrimitiveFromHandle(const CEnginePrimitiveHandle& handle){return static_cast<CEngineInternalPrimitiveBase&>(handle.GetData());}
class CEngineInternalAttachedPrimitive : public CEngineInternalPrimitiveBase {
public:
 enum EParentType {PARENT_TYPE_NULL,PARENT_TYPE_STATIC_MESH,PARENT_TYPE_ANIMATED_MESH};
 enum EChildType {CHILD_TYPE_NULL,CHILD_TYPE_STATIC_MESH};
 CEngineInternalAttachedPrimitive();
 virtual ~CEngineInternalAttachedPrimitive();
 bool Update(const CEnginePrimitive&,const CEnginePrimitiveAttachmentInfo&,const CEnginePrimitiveHandle&,long,unsigned long);
 EParentType GetParentType(long type)const{
  switch(type){
   case ENGINE_PRIMITIVE_ANIMATING_MESH:
   case ENGINE_PRIMITIVE_MORPHED_ANIMATING_MESH:
   case ENGINE_PRIMITIVE_ANIMATING_MESH_FROM_FILE:return PARENT_TYPE_ANIMATED_MESH;
  }
  return PARENT_TYPE_NULL;
 }
 CEnginePersistentPrimitiveBase* GetParentPrimitive(){
  if(ParentGroupIndex!=-1)return GFGetPrimitiveFromHandle(ParentHandle).GetChildPrimitive(ParentGroupIndex);
  return &GFGetPrimitiveFromHandle(ParentHandle);
 }
 void UndoMatrixConversion(CMatrix3x4&)const;
 void RedoMatrixConversion(CMatrix3x4&)const;
 CMatrix3x4 PrimitiveTransform;
 EParentType ParentType;
 EChildType ChildType;
 CEnginePrimitiveHandle ParentHandle;
 long ParentGroupIndex;
 CEnginePrimitiveHandle OutputPrimitive;
 CEnginePrimitiveAttachmentInfo AttachmentInfo;
 CMatrix3x4 ChildInverseTransform;
 bool ChildInverseTransformValid;
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalAttachedPrimitive)==220);
#endif

class CEnginePrimitiveManager {
public:
 CEngineInternalPrimitiveBase* Created;
 virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type){++creates;event(3);assert(Created&&Created->GetType()==type);return Created;}
 void AddPrimitive(CEngineInternalPrimitiveBase& primitive,unsigned long flags){++adds;assert(&primitive==Created&&flags==0);event(6);}
};
class CEnginePrimitiveTypeInfo {
public:
 CEnginePrimitiveManager* PrimitiveManagers[47];
 CEnginePrimitiveManager* GetPrimitiveManager(long type)const{return PrimitiveManagers[type];}
};
CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;
CEnginePrimitiveAttachmentInfo::CEnginePrimitiveAttachmentInfo():Flags(0),ParentDummyIndex(0),ChildDummyIndex(0){}
CEngineInternalAttachedPrimitive::CEngineInternalAttachedPrimitive():CEngineInternalPrimitiveBase(0),ParentType(PARENT_TYPE_NULL),ChildType(CHILD_TYPE_NULL),ParentGroupIndex(-1),ChildInverseTransformValid(true){}
CEngineInternalAttachedPrimitive::~CEngineInternalAttachedPrimitive(){}
void CEngineInternalAttachedPrimitive::UndoMatrixConversion(CMatrix3x4& m)const{
 ++conversions;event(7);
 m.E31*=100;m.E32*=100;m.E33*=100;
 m.E11*=-100;m.E12*=-100;m.E13*=-100;m.E21*=-100;m.E22*=-100;m.E23*=-100;
}
bool CEngineInternalAttachedPrimitive::Update(const CEnginePrimitive& src,const CEnginePrimitiveAttachmentInfo& attachment_info,const CEnginePrimitiveHandle& parent,long group_index,unsigned long flags)
{
 AttachmentInfo=attachment_info;
 ChildInverseTransformValid=false;
 ParentHandle=parent;
 ParentGroupIndex=group_index;
 if(!GetParentPrimitive())return false;
 ParentType=GetParentType(GetParentPrimitive()->GetType());
 if(ParentType==PARENT_TYPE_NULL)return false;
 EEnginePrimitiveType type=src.GetType();
 if(OutputPrimitive.IsNull()||OutputPrimitive.GetType()!=type){
  CEnginePrimitiveManager* prim_manager=g_pEnginePrimitiveTypeInfo->GetPrimitiveManager(type);
  if(!prim_manager)return false;
  CEngineInternalPrimitiveBase* internal_prim=prim_manager->CreateInternalPrimitive(type);
  OutputPrimitive.SetData(internal_prim);
  prim_manager->AddPrimitive(*internal_prim,0);
 }
 CEngineInternalPrimitiveBase& output_prim=GFGetPrimitiveFromHandle(OutputPrimitive);
 output_prim.Update(src,flags);
 output_prim.SetEnable(false);
 if(!output_prim.GetObjectTransform(PrimitiveTransform))return false;
 UndoMatrixConversion(PrimitiveTransform);
 return true;
}

int main(){
 int cases=0;
 for(int parent_type=0;parent_type<=14;++parent_type)for(int grouped=0;grouped<2;++grouped)
 for(int old_mode=0;old_mode<3;++old_mode)for(int manager_ok=0;manager_ok<2;++manager_ok)for(int transform_ok=0;transform_ok<2;++transform_ok){
  CEngineInternalPrimitiveBase parent(parent_type),group(1),old_output(old_mode==1?8:6),new_output(8);
  group.Child=&parent;new_output.TransformOK=old_output.TransformOK=transform_ok!=0;
  CEnginePrimitiveHandle parent_handle;parent_handle.SetData(grouped?&group:&parent);
  CEnginePrimitiveTypeInfo info;memset(info.PrimitiveManagers,0,sizeof(info.PrimitiveManagers));g_pEnginePrimitiveTypeInfo=&info;
  CEnginePrimitiveManager manager;manager.Created=&new_output;if(manager_ok)info.PrimitiveManagers[8]=&manager;
  CEngineInternalAttachedPrimitive object;CEnginePrimitive src;src.Type=ENGINE_PRIMITIVE_STATIC_MESH;
  if(old_mode)object.OutputPrimitive.SetData(&old_output);
  CEnginePrimitiveAttachmentInfo attachment;attachment.Flags=0x1234;attachment.ParentDummyName.Text=17;attachment.ChildDummyName.Text=23;attachment.ParentDummyIndex=11;attachment.ChildDummyIndex=13;
  assignments=deleted=notified=updates=creates=adds=conversions=transforms=event_count=0;
  bool accepted=parent_type==9||parent_type==11||parent_type==13;
  bool need_new=old_mode!=1;bool can_update=accepted&&(!need_new||manager_ok);
  bool result=object.Update(src,attachment,parent_handle,grouped?3:-1,0x4567);
  assert(result==(can_update&&transform_ok));assert(assignments==2&&!object.ChildInverseTransformValid);
  assert(object.AttachmentInfo.Flags==attachment.Flags&&object.AttachmentInfo.ParentDummyName.Text==17&&object.AttachmentInfo.ChildDummyName.Text==23);
  assert(object.AttachmentInfo.ParentDummyIndex==11&&object.AttachmentInfo.ChildDummyIndex==13);
  assert(object.ParentGroupIndex==(grouped?3:-1));assert((grouped?group.IVRefCount:parent.IVRefCount)==2);
  assert(group.GroupCalls==(grouped?2:0));if(grouped)assert(group.GroupIndex==3);
  assert(creates==(can_update&&need_new));assert(adds==creates);assert(updates==can_update&&transforms==can_update);assert(conversions==result);
  if(can_update){assert(last_flags==0x4567);CEngineInternalPrimitiveBase& output=GFGetPrimitiveFromHandle(object.OutputPrimitive);assert(!output.Enable);}
  if(can_update&&need_new&&old_mode){assert(deleted==1&&notified==1&&old_output.IVRefCount==0);}
  else assert(deleted==0&&notified==0);
  if(result){assert(object.PrimitiveTransform.E11==-100&&object.PrimitiveTransform.E23==-600&&object.PrimitiveTransform.E31==700&&object.PrimitiveTransform.E33==900&&object.PrimitiveTransform.E41==10);}
  // Release the handles while the controlled stack pointees still exist.
  object.OutputPrimitive.Clear();object.ParentHandle.Clear();parent_handle.Clear();
  ++cases;
 }
 // A group with no child must exit before type or manager access.
 {
  CEngineInternalPrimitiveBase group(1);CEnginePrimitiveHandle parent;parent.SetData(&group);CEngineInternalAttachedPrimitive object;
  CEnginePrimitive src;src.Type=ENGINE_PRIMITIVE_STATIC_MESH;CEnginePrimitiveAttachmentInfo info;event_count=0;creates=updates=0;
  assert(!object.Update(src,info,parent,12,0));assert(group.GroupCalls==1&&!creates&&!updates);object.ParentHandle.Clear();parent.Clear();
 }
 assert(cases==360);puts("00b396e0_TEST PASS");return 0;
}