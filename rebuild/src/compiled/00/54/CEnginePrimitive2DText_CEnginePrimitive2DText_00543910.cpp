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