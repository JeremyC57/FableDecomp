// 0064ca15 NPlayerGui::CDrawWorldTime::Draw
// Leaf: ?Draw@CDrawWorldTime@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: /O1 /Oy-
#include "engine/CBaseClass.h"
#define CIEngine CIEngineFields
#include "engine/CIEngine.h"
#undef CIEngine
#define CWorld CWorldFields
#include "engine/CWorld.h"
#undef CWorld
#define CPlayerGui CPlayerGuiFields
#include "engine/CPlayerGui.h"
#undef CPlayerGui
#include "engine/_quarantine/CPlayerGuiDef.h"
class C2DBoxF;
class C2DBoxI;
class C2DVector;
class C3DAnimationInfo;
class C3DBoundingBox;
class C3DBoundingSphere;
class C3DVector;
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
// Layout: original CIEngine virtual prefix through slot23 restored; shared header preserves storage evidence.
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

};

// Layout: original C2DVector is two public floats; original copy/ToC2DCoordF methods preserved.
class C2DVector {public:float X,Y;C2DVector(){}C2DVector(float x,float y):X(x),Y(y){}C2DVector(const C2DVector&v):X(v.X),Y(v.Y){}C2DVector ToC2DCoordF()const{return C2DVector(X,Y);}};
class C2DBoxF {public:float TLX,TLY,BRX,BRY;C2DBoxF(const C2DVector&,const C2DVector&);};
class CRGBColour {public:unsigned char B,G,R,A;};
enum EEnginePrimitiveType{};enum EEngineInterpolationSet{};enum EEngine2DLayer{};enum EEnginePrimitiveBlendMode{};
// Layout: ThemeChannelDesc is one four-byte composite at base offset8.
class CEnginePrimitiveThemeChannelDesc {public:unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;};
class CEnginePrimitive {protected:EEnginePrimitiveType Type;EEngineInterpolationSet InterpolationSet;CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;};
class CEnginePrimitive2DSprite:public CEnginePrimitive {public:C2DBoxF ClipBox;EEngine2DLayer Layer;unsigned long BankIndex,Frame;float StartAngle,EndAngle;EEnginePrimitiveBlendMode BlendMode;CRGBColour Colour;bool ClipBoxIsPos;unsigned long Flags;CTexture*Texture;C2DVector UVOffsetsStart,UVOffsetsEnd;
CEnginePrimitive2DSprite(unsigned __int64,const C2DVector&,EEngine2DLayer,unsigned long,unsigned long,EEnginePrimitiveBlendMode,EEngineInterpolationSet,float,float,unsigned char,unsigned char,unsigned char,unsigned char,unsigned long,const C2DVector&,const C2DVector&);
};
// Layout: original handle derives CBaseClass; Data is a four-byte CIVCountedPointer at4.
class CEnginePrimitiveHandle:public CBaseClass {unsigned char Data[4];};
// Layout: original Interpolate occupies0..33, CIEngine reference is at34; unused tail is omitted.
class CEnginePrimitivePrepareInfo {public:unsigned char Interpolate[0x34];CIEngine&Engine;};
// Layout: original CGameTimeManager private Time at8, as loaded in target.
class CGameTimeManager {unsigned char _pad_0[8];float Time;public:float GetTime()const{return Time;}};
class CWorld:public CWorldFields {public:CGameTimeManager&GetGameTimeManager(){return *PGameTimeManager_ptr;}};
class CPlayerGui:public CPlayerGuiFields {public:bool IsMiniMapActive()const;unsigned char GetMiniMapRingAlpha()const;static const CPlayerGuiDef&__fastcall PeekGuiDef();};
namespace std {template<class T>class allocator{};template<class D,class T,class A>class _STLP_alloc_proxy:public A{public:D _M_data;};template<class T,class A>class _Vector_base{protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:T&operator[](unsigned int i){return *(this->_M_start+i);}};}
class CGraphicDataBank;
class C2DExtentsI {public:long X,Y;C2DExtentsI(const C2DExtentsI&v):X(v.X),Y(v.Y){} };
class CDisplayManager {public:const C2DExtentsI GetRenderTargetDimensions();};
extern CDisplayManager*GPDisplayManager;
namespace NPlayerGui {
C2DVector __fastcall GFAnchorToTopRight(const C2DVector&);
// Layout: original virtual prefix and protected reference members; retail 12-byte PrimitiveHandles at1c.
class CDrawBase {public:virtual void ClearPrimitivesIfNecessary();virtual void ClearPrimitives();unsigned char ScaleAlpha(unsigned char)const;protected:CPlayerGui&PlayerGui;CPlayerManager&PlayerManager;CWorld&World;const CMainGameComponent&Component;const CGraphicDataBank&Bank;bool Drawing;std::vector<CEnginePrimitiveHandle>PrimitiveHandles;};
class CDrawWorldTime:public CDrawBase {public:void Draw(const CEnginePrimitivePrepareInfo&);};
}


namespace NPlayerGui {
#pragma auto_inline(off)
C2DVector __fastcall GFAnchorToTopRight(const C2DVector&pos){return C2DVector((float)GPDisplayManager->GetRenderTargetDimensions().X+pos.X-640.0f,pos.Y);}
#pragma auto_inline(on)
void CDrawWorldTime::Draw(const CEnginePrimitivePrepareInfo&info){
 if(PlayerGui.IsMiniMapActive()){
  unsigned char alpha=PlayerGui.GetMiniMapRingAlpha();
  if(alpha>0){
   Drawing=true;const CPlayerGuiDef&def=CPlayerGui::PeekGuiDef();
   C2DVector ringPos=GFAnchorToTopRight(*reinterpret_cast<const C2DVector*>(&def.ClockRingPos_X));
   CEnginePrimitive2DSprite ring(0,ringPos.ToC2DCoordF(),(EEngine2DLayer)0x46,def.ClockRingGraphic,0,(EEnginePrimitiveBlendMode)2,(EEngineInterpolationSet)0,0.0f,0.0f,ScaleAlpha(alpha),255,255,255,0,C2DVector(0,0),C2DVector(0,0));
   info.Engine.SetPersistentPrimitive(PrimitiveHandles[0],ring,0xc0);
   C2DVector pos=GFAnchorToTopRight(*reinterpret_cast<const C2DVector*>(&def.ClockPos_X));
   float rotation=World.GetGameTimeManager().GetTime()+0.75f;
   CEnginePrimitive2DSprite face(0,pos.ToC2DCoordF(),(EEngine2DLayer)0x44,def.ClockFaceGraphic,0,(EEnginePrimitiveBlendMode)2,(EEngineInterpolationSet)0,rotation,rotation,ScaleAlpha(alpha),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
   info.Engine.SetPersistentPrimitive(PrimitiveHandles[1],face,0xc0);
   CEnginePrimitive2DSprite hand(0,pos.ToC2DCoordF(),(EEngine2DLayer)0x43,def.ClockHandGraphic,0,(EEnginePrimitiveBlendMode)2,(EEngineInterpolationSet)0,0.25f,0.25f,ScaleAlpha(alpha),255,255,255,2,C2DVector(0,0),C2DVector(0,0));
   info.Engine.SetPersistentPrimitive(PrimitiveHandles[2],hand,0xc0);
  }
 }
}
}