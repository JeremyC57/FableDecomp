// 00bc9800 CEnginePrimitiveManagerLines::RenderPrimitive
// Leaf: ?RenderPrimitive@CEnginePrimitiveManagerLines@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@AAVCEngineSubPrimitiveChain@@@Z
// Flags:
#include "fable_compiler.h"
#define CRenderStateManager CRenderStateManagerFields
#include "engine/CRenderStateManager.h"
#undef CRenderStateManager
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CBaseClassNonCopyable.h"
enum POLY_CULL_MODE{};enum ERendered{};enum EEngineRenderLayer{};enum EEnginePrimitiveType{};enum ERenderPassType{};
class CEngineSubPrimitiveChain;class CEngineSubPrimitiveBase;class CEnginePrimitiveTypeList;class CEngineScene;template<class T>class CArray;
// Layout: original shared primitive base72; line's public bool UseZBuffer120.
class CEngineInternalPrimitiveLine:public CEngineInternalPrimitiveBase{unsigned char unreadLine[0x78-0x48];public:bool UseZBuffer;};
// Layout: shared flattened render-state fields retained; original nested structs restored for typed inline methods.
class CRenderStateManager:public CRenderStateManagerFields{
 using CRenderStateManagerFields::StateStack;using CRenderStateManagerFields::StateUpdateList;using CRenderStateManagerFields::StateStackSize;using CRenderStateManagerFields::StateUpdateListSize;using CRenderStateManagerFields::CurrentBookmarkMask;using CRenderStateManagerFields::DeviceState;using CRenderStateManagerFields::BookmarkDummyState;
 struct CStateInfo{public:unsigned long CurrentState,DesiredState,BookmarkMask;unsigned char unreadParam[4];bool DirtyListFlag;unsigned char unreadType[3];};
 struct CStateStackElement{public:CStateInfo*State;unsigned long Value;};
 struct CDeviceState{public:unsigned char unread0[60];CStateInfo ZBufferWriteEnable,ZBufferEnable;unsigned char unread100[20];CStateInfo PolyCullMode;};
 static unsigned long D3DPolyCullModeTable[3];
 FABLE_FORCEINLINE void SetRenderState(unsigned long value,CStateInfo&state){
 if(state.DesiredState!=value){
 if((CurrentBookmarkMask&state.BookmarkMask)==0){state.BookmarkMask|=CurrentBookmarkMask;reinterpret_cast<CStateStackElement*>(StateStack)[StateStackSize].State=&state;reinterpret_cast<CStateStackElement*>(StateStack)[StateStackSize].Value=state.DesiredState;++StateStackSize;}
 state.DesiredState=value;if(!state.DirtyListFlag){state.DirtyListFlag=true;StateUpdateList[StateUpdateListSize]=reinterpret_cast< ::CStateInfo*>(&state);++StateUpdateListSize;}
 }}
 public:void RestoreCaptureBlock();
 FABLE_FORCEINLINE void StartCaptureBlock(){reinterpret_cast<CStateStackElement*>(StateStack)[StateStackSize++].State=reinterpret_cast<CStateInfo*>(BookmarkDummyState);CurrentBookmarkMask<<=1;}
 FABLE_FORCEINLINE void SetPolyCullMode(POLY_CULL_MODE value){SetRenderState(D3DPolyCullModeTable[value],reinterpret_cast<CDeviceState*>(DeviceState)->PolyCullMode);}
 FABLE_FORCEINLINE void SetZBufferEnable(bool value){SetRenderState(value?1:0,reinterpret_cast<CDeviceState*>(DeviceState)->ZBufferEnable);}
 FABLE_FORCEINLINE void SetZBufferWriteEnable(bool value){SetRenderState(value?1:0,reinterpret_cast<CDeviceState*>(DeviceState)->ZBufferWriteEnable);}
};
class CRenderManagerCore:public CRenderStateManager{};class CRenderManager2D:public CRenderManagerCore{};class CRenderManager:public CRenderManager2D{};
namespace NEngineInternal{extern CRenderManager*GPRenderManager;}
class CRenderStateCaptureBlock{bool Open;static CRenderStateManager*StateManager;public:
 FABLE_FORCEINLINE CRenderStateCaptureBlock():Open(false){Capture();}
 FABLE_FORCEINLINE ~CRenderStateCaptureBlock(){Restore();}
 FABLE_FORCEINLINE void Capture(){if(!Open){StateManager->StartCaptureBlock();Open=true;}}
 FABLE_FORCEINLINE void Restore(){if(Open){StateManager->RestoreCaptureBlock();Open=false;}}
};
class CEnginePrimitiveRenderUtil:public CBaseClassNonCopyable{};
// Layout: original polymorphic util4, SceneList reference4, private Enable8.
class CEnginePrimitiveManager:public CEnginePrimitiveRenderUtil{protected:CArray<CEngineScene*>&SceneList;private:bool Enable;public:
 bool GetEnable()const{return Enable;}
 virtual CEngineInternalPrimitiveBase*CreateInternalPrimitive(EEnginePrimitiveType)=0;
 virtual void GetSupportedTypes(CEnginePrimitiveTypeList&)const=0;
 virtual void FrameStart();virtual void FrameEnd();virtual void RenderSubPrimitive(CEngineSubPrimitiveBase&,ERenderPassType);virtual void RenderBoundingVolume(CEngineSubPrimitiveBase&);
 virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase&,EEngineRenderLayer,CEngineSubPrimitiveChain&)=0;
};
class CEnginePrimitiveManagerLines:public CEnginePrimitiveManager{public:void RenderLine(const CEngineInternalPrimitiveLine&);virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase&,EEngineRenderLayer,CEngineSubPrimitiveChain&);};
ERendered CEnginePrimitiveManagerLines::RenderPrimitive(CEngineInternalPrimitiveBase&primitive,EEngineRenderLayer layer,CEngineSubPrimitiveChain&chain){
 if(!GetEnable())return static_cast<ERendered>(0);
 CRenderStateCaptureBlock capture;
 const CEngineInternalPrimitiveLine&line=static_cast<const CEngineInternalPrimitiveLine&>(primitive);
 NEngineInternal::GPRenderManager->SetPolyCullMode(static_cast<POLY_CULL_MODE>(2));
 NEngineInternal::GPRenderManager->SetZBufferEnable(true);
 NEngineInternal::GPRenderManager->SetZBufferWriteEnable(line.UseZBuffer);
 RenderLine(line);return static_cast<ERendered>(2);
}