#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __declspec(x)
#endif
#include <stdio.h>
#include <string.h>
#include <new>
// CEnginePrimitive2DText original descriptor: real C3DVector Pos and const-font reference.
// Leaf: ??0CEnginePrimitive2DText@@QAE@_KABVC2DVector@@W4EEngine2DLayer@@MABVCWideString@@ABVCFontBank@@ABVCRGBColour@@W4EEngineInterpolationSet@@@Z
// Flags:
#include <stddef.h>
#ifdef _MSC_VER
typedef unsigned __int64 PrimitiveIdentifier;
#else
typedef unsigned int PrimitiveIdentifier __attribute__((mode(DI)));
#endif
#pragma pack(push,1)
class C2DVector {public:float X,Y;C2DVector(float x,float y):X(x),Y(y){}C2DVector(const C2DVector&v):X(v.X),Y(v.Y){}};
class C3DVector {public:float X,Y,Z;C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}};
#pragma pack(pop)
class CWideStringData;class CWideString {public:CWideStringData*PStringData;CWideString();CWideString(const CWideString&);~CWideString();};
// Original accessed anonymous RGBA component projection. Other overlapping union
// alias members are not exported by DumpStructLayouts and are not invented here.
class CRGBColour {public:unsigned char B,G,R,A;void Initialise(unsigned char r,unsigned char g,unsigned char b,unsigned char a){R=r;G=g;B=b;A=a;}CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a){Initialise(r,g,b,a);}CRGBColour(const CRGBColour&c){Initialise(c.R,c.G,c.B,c.A);}};
class CFontBank;
enum EEnginePrimitiveType {};enum EEngine2DLayer {};enum EEngineInterpolationSet {};
class CEnginePrimitiveThemeChannelDesc {public:unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0){}};
class CEnginePrimitive {public:EEnginePrimitiveType Type;EEngineInterpolationSet InterpolationSet;CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;CEnginePrimitive(EEnginePrimitiveType type,PrimitiveIdentifier,EEngineInterpolationSet set):Type(type),InterpolationSet(set){}};
class CEnginePrimitive2DText:public CEnginePrimitive {public:C3DVector Pos;EEngine2DLayer Layer;float Scale;CRGBColour Colour;CWideString Text;const CFontBank*PFont;CEnginePrimitive2DText(PrimitiveIdentifier,const C2DVector&,EEngine2DLayer,float,const CWideString&,const CFontBank&,const CRGBColour&,EEngineInterpolationSet);CEnginePrimitive2DText(const CEnginePrimitive2DText&);};
CEnginePrimitive2DText::CEnginePrimitive2DText(PrimitiveIdentifier id,const C2DVector&position,EEngine2DLayer layer,float scale,const CWideString&text,const CFontBank&font,const CRGBColour&colour,EEngineInterpolationSet interpolation):CEnginePrimitive(static_cast<EEnginePrimitiveType>(0x27),id,interpolation),Pos(position.X,position.Y,0),Layer(layer),Scale(scale),Colour(colour),Text(text),PFont(&font){}

static const CWideString*expectedText;static unsigned copies,destroys;static bool bad;
CWideString::CWideString():PStringData(0){}CWideString::CWideString(const CWideString&v):PStringData(v.PStringData){++copies;if(&v!=expectedText)bad=true;}CWideString::~CWideString(){++destroys;}
int main(){if(sizeof(CEnginePrimitive)!=12||sizeof(CEnginePrimitive2DText)!=44||sizeof(C3DVector)!=12||sizeof(CRGBColour)!=4)return 1;
 C2DVector pos(1.25f,-2.75f);CRGBColour colour(0x11,0x22,0x33,0x44);CWideString text;int textAnchor=7;text.PStringData=reinterpret_cast<CWideStringData*>(&textAnchor);expectedText=&text;
 union FontStorage{double align;unsigned char bytes[16];}fontStorage;const CFontBank*font=reinterpret_cast<const CFontBank*>(fontStorage.bytes);
 union PrimitiveStorage{double align;unsigned char bytes[44];}storage;unsigned char first[44];
 PrimitiveIdentifier id=1;id<<=63;id|=0x7123;
 for(unsigned i=0;i<2;++i){memset(storage.bytes,0xa5,sizeof storage.bytes);CEnginePrimitive2DText*p=::new(static_cast<void*>(storage.bytes))CEnginePrimitive2DText(i?id:0,pos,static_cast<EEngine2DLayer>(3),1.75f,text,*font,colour,static_cast<EEngineInterpolationSet>(5));
  if(p->Type!=static_cast<EEnginePrimitiveType>(0x27)||p->InterpolationSet!=static_cast<EEngineInterpolationSet>(5)||p->ThemeChannelDesc.ChannelIndex0||p->ThemeChannelDesc.ChannelIndex1||p->ThemeChannelDesc.ChannelBlendFactor||storage.bytes[11]!=0xa5)return 2;
  if(p->Pos.X!=1.25f||p->Pos.Y!=-2.75f||p->Pos.Z!=0||p->Layer!=static_cast<EEngine2DLayer>(3)||p->Scale!=1.75f)return 3;
  if(p->Colour.R!=0x11||p->Colour.G!=0x22||p->Colour.B!=0x33||p->Colour.A!=0x44||p->Text.PStringData!=text.PStringData||p->PFont!=font||copies!=i+1||bad)return 4;
  if(i==0)memcpy(first,storage.bytes,sizeof first);else if(memcmp(first,storage.bytes,sizeof first))return 5;
  p->~CEnginePrimitive2DText();if(destroys!=i+1)return 6;
 }
 puts("00543910_TEST PASS");return 0;}