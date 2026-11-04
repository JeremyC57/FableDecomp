// 00699b00 CGameCameraManager::PreloadScriptedCamera
// Leaf: ?PreloadScriptedCamera@CGameCameraManager@@AAEXV?$CCountedPointer@VCScriptModuleBase@NCameraModes@@@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags:
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"
#define CGameCameraManager CGameCameraManagerFields
#include "engine/CGameCameraManager.h"
#undef CGameCameraManager
#define CDisplayEngine CDisplayEngineFields
#include "engine/CDisplayEngine.h"
#undef CDisplayEngine
#define CIEngine CIEngineFields
#include "engine/CIEngine.h"
#undef CIEngine
class C2DBoxF;
class C2DBoxI;
class C2DVector;
class C3DAnimationInfo;
class C3DBoundingBox;
class C3DBoundingSphere;
#include "engine/C3DVector.h"
class CCamera;
class CCharString;
class CEngineInit;
class CEngineInterpolationInfo;
class CEnginePrimitive;
class CEnginePrimitiveAnimation;
class CEnginePrimitiveAttachmentInfo;
class CEnginePrimitiveHandle;
class CEnginePrimitivePosition;
class CEngineVideoOptions;
class CIDrawWorld;
class CRGBColour;
class CStaticMapGenerationVisibiilityInfo;
class CTexture;
class CWideString;
namespace std{template<class A,class B>struct pair;template<class T>struct less;template<class T>class allocator;template<class K,class V,class C,class A>class map;}
template<class T>class CArray;
enum EEnginePrimitiveStatIndex{};
class CInitBaseClass:public CBaseClass{bool Valid;public:virtual~CInitBaseClass();virtual void Validate()const;};
// Layout: original CIEngine virtual prefix through slot57 restored; shared header preserves storage evidence.
class CIEngine:public CInitBaseClass{public:virtual~CIEngine();
 virtual bool Initialise(const CEngineInit&);
 virtual void Release();
 virtual void SetCamera(const CCamera*);
 virtual void SetWindow(const C2DBoxF*);
 virtual void SetEnvironmentalEffectsAlwaysUpdate(bool);
 virtual void SetWorld(const CIDrawWorld*);
 virtual void Render(const CEngineInterpolationInfo&,unsigned long);
 virtual void WorldFrameUpdate(bool);
 virtual void PausedWorldFrameUpdate();
 virtual bool ConvertScreenPosToWorldPos(const C2DVector&,C3DVector&)const;
 virtual bool ConvertScreenPosToWorldPosAtViewDistance(float,const C2DVector&,C3DVector&)const;
 virtual bool ConvertWorldPosToScreenPos(const C3DVector&,C2DVector&)const;
 virtual bool SetVideoOptions(const CEngineVideoOptions&);
 virtual bool PreChangeResolution();
 virtual bool PostChangeResolution(unsigned long,unsigned long,unsigned long);
 virtual void Refresh();
 virtual bool CheckPrimitiveVisibility(unsigned __int64);
 virtual float GetPrimitiveDrawDistanceFactor()const;
 virtual void AddPrimitive(const CEnginePrimitive&);
 virtual bool ClipSphere(const C3DBoundingSphere&)const;
 virtual bool ClipBox(const C3DBoundingBox&)const;
 virtual void SetPersistentPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long);
 virtual void SetPrimitiveAnimation(CEnginePrimitiveHandle&,const CEnginePrimitiveAnimation&);
 virtual void SetPrimitivePosition(CEnginePrimitiveHandle&,const CEnginePrimitivePosition&);
 virtual unsigned long AddChildPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long);
 virtual void SetChildPrimitive(CEnginePrimitiveHandle&,unsigned long,CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long);
 virtual void SetChildPrimitive(CEnginePrimitiveHandle&,unsigned long,const CEnginePrimitive&,unsigned long);
 virtual void GetChildPrimitiveHandle(CEnginePrimitiveHandle&,unsigned long,CEnginePrimitiveHandle&);
 virtual void ClearChildPrimitive(CEnginePrimitiveHandle&,unsigned long);
 virtual void ClearAllChildPrimitives(CEnginePrimitiveHandle&);
 virtual void SetAttachedPrimitive(CEnginePrimitiveHandle&,const CEnginePrimitive&,const CEnginePrimitiveAttachmentInfo&,const CEnginePrimitiveHandle&,long,unsigned long);
 virtual void CollectPrimitiveStatsForArea(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,unsigned long> > >&,const C2DBoxF&,EEnginePrimitiveStatIndex,unsigned long)const;
 virtual void ProcessPrimitiveStats(const std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long&,CArray<CCharString>&)const;
 virtual const C3DAnimationInfo* GetEditorAnimation(const CWideString&)const;
 virtual unsigned long AddCustomMorphTexture(CTexture&,const CCharString&)const;
 virtual long GetStaticMapUpdatePassCount();
 virtual void StartStaticMapUpdatePass(const CWideString&,long,const CStaticMapGenerationVisibiilityInfo&,const CArray<CCharString>&);
 virtual bool IsStaticMapNeedingUpdate(long,const CCharString&,unsigned long)const;
 virtual void UpdateStaticMap(long,const CCharString&,unsigned long,long);
 virtual void CloseStaticMapFile();
 virtual void AddMap(long,long);
 virtual void RemoveMap(long);
 virtual void SetMapAsLoaded(long);
 virtual void SetMapAsUnloaded(long);
 virtual void SetMapVisibilityAllMaps(bool);
 virtual void SetMapVisibility(long,bool);
 virtual void AreaChanged(const C2DBoxI&);
 virtual void OnActivate();
 virtual void OnDeactivate();
 virtual bool IsActive()const;
 virtual void SetStaticMapFileForUse(const CWideString&);
 virtual void SetLandscapeDebugTintColour(long,CRGBColour);
 virtual void ReloadLocalDetailDefs();
 virtual void StartNewScene(unsigned long,float);
 virtual void CutscenePreload(bool,bool);
 virtual void CutsceneAddCameraAngle(const CCamera&);
private:bool Active;};
class CDisplayEngine:public CDisplayEngineFields{public:CIEngine&Get3DEngine(){return *Engine3D;}};
namespace boost{template<class T>class scoped_ptr{T*ptr;public:T&operator*()const{return *ptr;}};}
// Layout: original MainGameComponent PDisplayEngine at28; raw original type is scoped_ptr.
class CMainGameComponent{protected:static CMainGameComponent*PMainGameComponent;unsigned char _pad_0[0x28];boost::scoped_ptr<CDisplayEngine>PDisplayEngine;public:static CMainGameComponent*__fastcall Get(){return PMainGameComponent;}CDisplayEngine&GetDisplayEngine(){return *PDisplayEngine;}};
class CRightHandedSet{protected:C3DVector Up,Forward;};
// Layout: original camera56-byte object; flags at24/25, FOVFlags28, floats2c..34.
class CCamera{C3DVector WorldPosition;CRightHandedSet RHSet;bool HeightLocked,ViewVecZLocked;unsigned long FOVFlags;float HorizontalFOV,VerticalFOV,Zoom;public:CCamera():HeightLocked(false),ViewVecZLocked(false){}CCamera(const CCamera&);};
class CGameCameraManager;class CCameraModeDef;
namespace NCameraModes{
class CScriptModuleBase{public:virtual ~CScriptModuleBase();};
// Layout: native CBase prefix is a4: donor vector shrinks4 and two physics-state list pairs shrink16 each.
class CBase{unsigned char _pad_0x04[0xa0];public:virtual~CBase();CBase(CGameCameraManager&,const CDefPointer<const CCameraModeDef>&);void TransferPersistantBaseData(const CBase&);void Init();};
class CScriptedControl:public CBase{CCountedPointer<CScriptModuleBase>PScriptModule;float ObservedThingMeshHeightOnInit;CCamera InitCamera;public:CScriptedControl(CGameCameraManager&,const CDefPointer<const CCameraModeDef>&,CCountedPointer<CScriptModuleBase>&);};
}
namespace {extern CDefPointer<const CCameraModeDef>DEFAULT_CAMERA_MODE_DEF;}
// Layout: the actual receiver storage is the supplied header; original method is private.
class CGameCameraManager:public CGameCameraManagerFields{void PreloadScriptedCamera(CCountedPointer<NCameraModes::CScriptModuleBase>);};
FABLE_STATIC_ASSERT(sizeof(CCamera)==0x38);
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CBase)==0xa4);
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CScriptedControl)==0xe8);

#pragma auto_inline(off)
NCameraModes::CScriptedControl::CScriptedControl(CGameCameraManager&manager,const CDefPointer<const CCameraModeDef>&def,CCountedPointer<CScriptModuleBase>&module):CBase(manager,def),PScriptModule(module),ObservedThingMeshHeightOnInit(1.0f){}
#pragma auto_inline(on)

void CGameCameraManager::PreloadScriptedCamera(CCountedPointer<NCameraModes::CScriptModuleBase> module){
 CCamera&camera=*reinterpret_cast<CCamera*>(Camera);
 CCamera savedCamera(camera);
 CCamera&oldCamera=*reinterpret_cast<CCamera*>(OldCamera);
 CCamera savedOldCamera(oldCamera);
 CCountedPointer<NCameraModes::CBase>mode;
 mode.Reset(new NCameraModes::CScriptedControl(*this,DEFAULT_CAMERA_MODE_DEF,module));
 if(CurrentMode_Data)mode->TransferPersistantBaseData(*reinterpret_cast<NCameraModes::CBase*>(CurrentMode_Data));
 mode->Init();CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().CutsceneAddCameraAngle(camera);
 camera=savedCamera;oldCamera=savedOldCamera;
}