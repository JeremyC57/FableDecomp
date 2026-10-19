// NPlayerGui::CDrawQuestInfo::DrawOracleSymbols native64F6DC,288 bytes.
// Actual Gui/GuiDef/CIEngine/PersistentPrimitive headers. No public Sprite/PrepareInfo/Handle/DrawQuest header exists.
// Native list head40 and handle vector44: base vector shrinks4, list12->4 vs donor44/50.
// Definition OraclePos594/Offset59C vs donor5D0/5D8; globalGui13B8790 Alpha2A0 vs donor2B4.
// GFAnchorToTopRight643CA1 genuine fastcall class return: viewport width + X - 640, Y preserved (PDB name confirmed).
// Sprite ctor493AC2 exact 16-parameter signature donor49C7E3; full84byte PDB/native layout.
// Engine nativeVT12A0F3C slot23/+5C = B23BC0 SetPersistentPrimitive(handle&,constprimitive&,ulong), nonconst.
// Resize443F90; PeekGuiDef438570; alpha255 float1230014 and ftolBFEA70.
// Leaf: DrawOracleSymbols
#pragma optimize("y",off)
#pragma optimize("s",on)
#include "engine/CPlayerGui.h"
#include "engine/_quarantine/CPlayerGuiDef.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePersistentPrimitiveBase.h"
struct C2DVector {float X,Y;C2DVector(){}C2DVector(float x,float y):X(x),Y(y){}C2DVector&operator+=(const C2DVector&v){X+=v.X;Y+=v.Y;return *this;}};
struct C2DBoxF {float TLX,TLY,BRX,BRY;};
enum EEngine2DLayer {NativeOracleLayer=74};enum EEnginePrimitiveBlendMode {NativeSpriteBlend=2};enum EEngineInterpolationSet {NativeNoInterpolation=0};
struct CRGBColour {unsigned char Blue,Green,Red,Alpha;};
struct CTexture;
struct CEnginePrimitive {long Type;EEngineInterpolationSet InterpolationSet;unsigned char ThemeChannels[4];};
struct CEnginePrimitive2DSprite:CEnginePrimitive {C2DBoxF ClipBox;EEngine2DLayer Layer;unsigned long BankIndex,Frame;float StartAngle,EndAngle;EEnginePrimitiveBlendMode BlendMode;CRGBColour Colour;bool ClipBoxIsPos;unsigned long Flags;CTexture*Texture;C2DVector UVOffsetsStart,UVOffsetsEnd;
 CEnginePrimitive2DSprite(unsigned __int64,const C2DVector&,EEngine2DLayer,unsigned long,unsigned long,EEnginePrimitiveBlendMode,EEngineInterpolationSet,float,float,unsigned char,unsigned char,unsigned char,unsigned char,unsigned long,const C2DVector&,const C2DVector&);
};
struct CEnginePrimitiveHandle {int Unrecovered00;CEnginePersistentPrimitiveBase*Data;}; // actual PDB padding0 and CIVCountedPointer Data4
struct HandleArray {CEnginePrimitiveHandle*Start,*Finish,*Limit;void Resize(unsigned long);};
struct EngineMethods:CIEngine {typedef void(EngineMethods::*SetMethod)(CEnginePrimitiveHandle&,const CEnginePrimitive&,unsigned long);void SetPersistentPrimitive(CEnginePrimitiveHandle&h,const CEnginePrimitive&p,unsigned long f){(this->*static_cast<SetMethod*>(__vftable)[23])(h,p,f);}};
struct CEnginePrimitivePrepareInfo {unsigned char Unrecovered00[0x34];EngineMethods*Engine;};
struct PlayerGuiAlphaView {unsigned char Unrecovered00[0x2a0];float Alpha;};extern PlayerGuiAlphaView*GlobalPlayerGui;
struct GuiOracleDefView {unsigned char Unrecovered00[0x594];C2DVector OracleSymbolPos,OracleSymbolOffset;};GuiOracleDefView* __fastcall PeekGuiDef();C2DVector __fastcall GFAnchorToTopRight(const C2DVector&);
struct OracleNode {OracleNode*Next,*Previous;long Graphic;};
struct OracleList {OracleNode*Head;bool empty()const{return Head->Next==Head;}unsigned long size()const{unsigned long n=0;for(OracleNode*p=Head->Next;p!=Head;p=p->Next)++n;return n;}};
struct DrawQuestNative {void*VTable;void*PlayerGui;void*PlayerManager;void*World;void*Component;void*Bank;bool Drawing;unsigned char Unrecovered19[0x40-0x19];OracleList OracleSymbols;HandleArray OraclePrimitiveHandles;
 void DrawOracleSymbols(const CEnginePrimitivePrepareInfo&);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitive)==12);
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitive2DSprite)==84);
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveHandle)==8);
FABLE_STATIC_ASSERT(sizeof(EngineMethods::SetMethod)==4);
FABLE_STATIC_ASSERT(offsetof(DrawQuestNative,OracleSymbols)==0x40);
FABLE_STATIC_ASSERT(offsetof(DrawQuestNative,OraclePrimitiveHandles)==0x44);
#endif
void DrawQuestNative::DrawOracleSymbols(const CEnginePrimitivePrepareInfo&info){
 if(OracleSymbols.empty())return;Drawing=true;GuiOracleDefView*def=PeekGuiDef();OraclePrimitiveHandles.Resize(OracleSymbols.size());C2DVector position=GFAnchorToTopRight(def->OracleSymbolPos);OracleNode*p=OracleSymbols.Head->Next;if(p==OracleSymbols.Head)return;unsigned long i=0;const C2DVector uvEnd(0,0),uvStart(0,0);
 do{CEnginePrimitive2DSprite sprite(0,C2DVector(position),NativeOracleLayer,p->Graphic,0,NativeSpriteBlend,NativeNoInterpolation,0,0,(unsigned char)(GlobalPlayerGui->Alpha*255.0f),255,255,255,2,uvStart,uvEnd);info.Engine->SetPersistentPrimitive(OraclePrimitiveHandles.Start[i],sprite,0xc0);p=p->Next;position+=def->OracleSymbolOffset;++i;}while(p!=OracleSymbols.Head);
}